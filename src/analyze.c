#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

static int location = 0;
static char escopo_atual[MAX_SCOPE_SIZE] = "";
static int found_main = 0;

void semanticError(TreeNode * t, char * message, char * aux) {
    static int lastLine = -1;
    static char lastMessage[256] = "";
    
    if (t->lineno != lastLine || strcmp(message, lastMessage) != 0) {
        if (strcmp(message, "undefined main") == 0) {
            pc("Semantic error: undefined reference to 'main'\n");
        }
        else if (strcmp(message, "already declared in this scope") == 0) {
            pc("Semantic error at line %d: '%s' was already declared as a variable\n", 
               t->lineno, aux);
        }
        else if (strcmp(message, "variable declared void") == 0) {
            pc("Semantic error at line %d: variable declared void\n", t->lineno);
        }
        else if (strcmp(message, "not declared in this scope") == 0) {
            pc("Semantic error at line %d: '%s' was not declared in this scope\n", 
               t->lineno, aux);
        }
        else if (strcmp(message, "function not declared") == 0) {
            pc("Semantic error at line %d: '%s' was not declared in this scope\n", 
               t->lineno, aux);
        }
        else if (strcmp(message, "invalid use of void type") == 0) {
            pc("Semantic error at line %d: invalid use of void expression\n", t->lineno);
        }
        else if (strcmp(message, "test expression") == 0) {
            pc("Semantic error at line %d: test expression must be integer\n", t->lineno);
        }
        else if (strcmp(message, "type mismatch") == 0) {
            pc("Semantic error at line %d: type mismatch\n", t->lineno);
        }
        else if (strcmp(message, "void function return value") == 0) {
            pc("Semantic error at line %d: void function cannot return a value\n", t->lineno);
        }
        else if (strcmp(message, "non-void function return") == 0) {
            pc("Semantic error at line %d: non-void function must return a value\n", t->lineno);
        }
        else {
            pc("Semantic error at line %d: %s\n", t->lineno, message);
        }

        lastLine = t->lineno;
        strcpy(lastMessage, message);
        Error = TRUE;
    }
}

static void traverse(TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *)) { 
    if (t != NULL) { 
        preProc(t);
        for (int i = 0; i < MAXCHILDREN; i++)
            traverse(t->child[i], preProc, postProc);
        
        postProc(t);
        
        if (t->nodekind == StmtK && t->kind.stmt == FuncK) {
            strcpy(escopo_atual, "");
        }
        
        traverse(t->sibling, preProc, postProc);
    }
}

static void nullProc(TreeNode * t) {
    if (t == NULL) return;
}

static void insertBuiltInFunctions(void) {
    st_insert("input", FuncK, "", Integer, FALSE, 0, 0);
    st_insert("output", FuncK, "", Void, FALSE, 1, 0);
}

static void insertNode(TreeNode * t) {
    if (t == NULL) return;
    
    switch (t->nodekind) {
        case StmtK:
            switch (t->kind.stmt) {
                case FuncK:
                    if (t->attr.name == NULL) break;
                    
                    if (st_lookup(t->attr.name, "") >= 0) {
                        semanticError(t, "already declared in this scope", t->attr.name);
                    } else {
                        int paramCount = 0;
                        TreeNode *param = t->child[0];
                        while (param != NULL) {
                            paramCount++;
                            param = param->sibling;
                        }
                        st_insert(t->attr.name, t->kind.stmt, "", t->type, FALSE, paramCount, t->lineno);
                        st_add_params(t->attr.name, t->child[0]);
                        strcpy(escopo_atual, t->attr.name);
                        
                        if (strcmp(t->attr.name, "main") == 0) {
                            if (t->type != Void)
                                semanticError(t, "main function must return void", NULL);
                            found_main = 1;
                        }
                    }
                    break;

                case VarK:
                    if (t->attr.name == NULL) break;
                    
                    if (st_lookup(t->attr.name, escopo_atual) >= 0) {
                        semanticError(t, "already declared in this scope", t->attr.name);
                    } else if (st_lookup(t->attr.name, "") >= 0 && 
                             st_getidtype(t->attr.name, "") == FuncK) {
                        semanticError(t, "already declared as a function", t->attr.name);
                    } else {
                        if (t->type == Void)
                            semanticError(t, "variable declared void", NULL);
                        else {
                            int size = (t->child[0] != NULL) ? t->child[0]->attr.val : 0;
                            st_insert(t->attr.name, VarK, escopo_atual, t->type, size > 0, size, t->lineno);
                        }
                    }
                    break;

                case ParamK:
                    if (t->attr.name == NULL) break;
                    
                    if (st_lookup(t->attr.name, escopo_atual) >= 0) {
                        semanticError(t, "already declared in this scope", t->attr.name);
                    } else {
                        st_insert(t->attr.name, ParamK, escopo_atual, t->type, 
                                t->isArray, t->isArray ? 1 : 0, t->lineno);
                    }
                    break;

                case AssignK:
                    if (t->child[0] != NULL && t->child[0]->nodekind == ExpK && 
                        t->child[0]->kind.exp == IdK) {
                        char *varName = t->child[0]->attr.name;
                        if (varName != NULL) {
                            if (st_lookup(varName, escopo_atual) < 0 && 
                                st_lookup(varName, "") < 0) {
                                semanticError(t->child[0], "not declared in this scope", varName);
                            } else {
                                char *scope = st_lookup(varName, escopo_atual) >= 0 ? 
                                            escopo_atual : "";
                                st_add_lineno(varName, scope, t->child[0]->lineno);
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
            break;

        case ExpK:
            switch (t->kind.exp) {
                case CallK:
                    if (t->attr.name != NULL) {
                        if (st_lookup(t->attr.name, "") < 0) {
                            semanticError(t, "function not declared", t->attr.name);
                        } else {
                            st_add_lineno(t->attr.name, "", t->lineno);
                            t->type = st_gettype(t->attr.name, "");
                        }
                    }
                    break;

                case IdK:
                    if (t->attr.name == NULL) break;
                    
                    if (st_lookup(t->attr.name, escopo_atual) < 0 && 
                        st_lookup(t->attr.name, "") < 0) {
                        semanticError(t, "not declared in this scope", t->attr.name);
                    } else {
                        char *scope = st_lookup(t->attr.name, escopo_atual) >= 0 ? 
                                    escopo_atual : "";
                        st_add_lineno(t->attr.name, scope, t->lineno);
                        t->type = st_gettype(t->attr.name, scope);
                        
                        if (t->child[0] != NULL && !st_is_array(t->attr.name, scope)) {
                            semanticError(t, "subscripted value is not an array", NULL);
                        }
                    }
                    break;

                case OpK:
                    if (t->child[0] != NULL && t->child[1] != NULL) {
                        // Apenas definir o tipo como Integer sem verificações adicionais
                        t->type = Integer;
                    }
                    break;

                case ConstK:
                    t->type = Integer;
                    break;
            }
            break;
    }
}

static void checkNode(TreeNode * t) {
    if (t == NULL) return;
    
    switch (t->nodekind) {
        case ExpK:
            switch (t->kind.exp) {
                case OpK:
                    if (t->child[0] != NULL && t->child[1] != NULL) {
                        if (t->child[0]->type != Integer || t->child[1]->type != Integer) {
                            semanticError(t, "type mismatch", NULL);
                        }
                        t->type = Integer;
                    }
                    break;

                case CallK:
                    if (t->attr.name != NULL) {
                        TreeNode *arg = t->child[0];
                        ParamList params = st_getparams(t->attr.name);
                        
                        while (arg != NULL && params != NULL) {
                            if (arg->type == Void) 
                                semanticError(arg, "invalid use of void type", NULL);
                            if (arg->type != params->type)
                                semanticError(arg, "parameter type mismatch", NULL);
                            arg = arg->sibling;
                            params = params->next;
                        }
                    }
                    break;

                case IdK:
                    if (t->child[0] != NULL) {
                        if (t->child[0]->type != Integer) {
                            semanticError(t, "array index must be integer", NULL);
                        }
                    }
                    break;
            }
            break;

        case StmtK:
            switch (t->kind.stmt) {
                case IfK:
                case WhileK:
                    if (t->child[0] != NULL) {
                        if (t->child[0]->type == Void)
                            semanticError(t, "test expression must be integer", NULL);
                    }
                    break;

                case AssignK:
                    if (t->child[0] != NULL && t->child[1] != NULL) {
                        if (t->child[1]->type == Void) {
                            semanticError(t, "invalid use of void type", NULL);
                        }
                        else if (t->child[0]->type != t->child[1]->type) {
                            semanticError(t, "type mismatch", NULL);
                        }
                    }
                    break;

                case ReturnK:
                    if (strcmp(escopo_atual, "") != 0) {
                        ExpType funcType = st_gettype(escopo_atual, "");
                        if (!t->child[0] && funcType != Void) {
                            semanticError(t, "non-void function return", NULL);
                        }
                        else if (t->child[0] && funcType == Void) {
                            semanticError(t, "void function return value", NULL);
                        }
                        else if (t->child[0] && t->child[0]->type != funcType) {
                            semanticError(t, "type mismatch", NULL);
                        }
                    }
                    break;
            }
            break;
    }
}

void buildSymtab(TreeNode * syntaxTree) {
    found_main = 0;
    escopo_atual[0] = '\0';
    insertBuiltInFunctions();
    traverse(syntaxTree, insertNode, nullProc);
    if (TraceAnalyze) {
        pc("\nSymbol table:\n\n");
        printSymTab();
    }
}

void typeCheck(TreeNode * syntaxTree) {
    traverse(syntaxTree, nullProc, checkNode);
}

void mainCheck(TreeNode * syntaxTree) {
    if (!found_main) {
        semanticError(syntaxTree, "undefined main", NULL);
        exit(1);
    }
}