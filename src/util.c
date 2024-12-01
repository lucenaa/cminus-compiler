/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the C- compiler                             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 4
#define UNINDENT indentno -= 4

/* printSpaces indents by printing spaces */
static void printSpaces(void) {
    int i;
    for (i = 0; i < indentno; i++)
        pc(" ");
}



void printToken(TokenType token, const char* tokenString)
{
    switch (token)
    {
        case IF:
        case ELSE:
        case INT:
        case RETURN:
        case VOID:
        case WHILE:
            pc("reserved word: %s\n", tokenString);
            break;
        case PLUS: pc("+\n"); break;
        case MINUS: pc("-\n"); break;
        case TIMES: pc("*\n"); break;
        case OVER: pc("/\n"); break;
        case LT: pc("<\n"); break;
        case LEQ: pc("<=\n"); break;
        case GT: pc(">\n"); break;
        case GEQ: pc(">=\n"); break;
        case EQEQ: pc("==\n"); break;
        case INEQ: pc("!=\n"); break;
        case EQ: pc("=\n"); break;
        case SEMI: pc(";\n"); break;
        case COMMA: pc(",\n"); break;
        case LPAREN: pc("(\n"); break;
        case RPAREN: pc(")\n"); break;
        case LBRACKETS: pc("[\n"); break;
        case RBRACKETS: pc("]\n"); break;
        case LCBRACES: pc("{\n"); break;
        case RCBRACES: pc("}\n"); break;
        case ENDFILE: pc("EOF\n"); break;
        case NUM: pc("NUM, val= %s\n", tokenString); break;
        case ID: pc("ID, name= %s\n", tokenString); break;
        case ERROR: pce("ERROR: %s\n", tokenString); break;
        default:
            pce("Unknown token: %d\n", token);
    }
}


/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode* newStmtNode(StmtKind kind)
{
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        pce("Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}


/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode* newExpNode(ExpKind kind)
{
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        pce("Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char* copyString(char* s) {
    int n;
    char* t;
    if (s == NULL) return NULL;
    n = strlen(s) + 1;
    t = malloc(n);
    if (t == NULL) {
        pce("Out of memory error at line %d\n", lineno);
    } else {
        strcpy(t, s);
    }
    return t;
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode* tree)
{
    int i;
    while (tree != NULL)
    {
        printSpaces();
        
        if (tree->nodekind == StmtK)
        {
            switch (tree->kind.stmt)
            {
                case IfK:
                    pc("Conditional selection\n");
                break;
                case WhileK:
                    pc("Iteration (loop)\n");
                break;
                case AssignK:{
                    const TreeNode *node = tree->child[0];
                    if(node->nodekind == ExpK && node->kind.exp == IdK) {
                        if(node->child[0] != NULL) {
                            pc("Assign to array: %s\n", node->attr.name);
                            INDENT;
                            if(node->child[0]->attr.val != 0) {
                                printTree(node->child[0]);
                            }
                            UNINDENT;
                        }
                        else {
                            pc("Assign to var: %s\n", node->attr.name);
                        }
                    }
                    else{
                        pc("Assign to: (unknown)\n");
                    }
                    break;}
                case ReturnK:
                    pc("Return\n");
                break;
                case ParamK:
                    pc("Function param (%s %s): %s\n", ExpTypeToString(tree->type), (tree->isArray ? "array" : "var"), tree->attr.name);
                break;
                case VarK:
                    pc("Declare %s %s: %s\n", ExpTypeToString(tree->type), tree->child[0] ? "array":"var", tree->attr.name);
                break;
                case FuncK:
                    pc("Declare function (return type \"%s\"): %s\n", ExpTypeToString(tree->type), tree->attr.name);
                break;
                default:
                    pce("Unknown StmtNode kind\n");
                break;
            }
        }
        else if (tree->nodekind == ExpK)
        {
            switch (tree->kind.exp)
            {
                case OpK:
                    pc("Op: ");
                printToken(tree->attr.op, "\0");
                break;
                case ConstK:
                    pc("Const: %d\n", tree->attr.val);
                break;
                case IdK:
                    pc("Id: %s\n", tree->attr.name);
                break;
                case CallK:
                    pc("Function call: %s\n", tree->attr.name);
                break;
                default:
                    pce("Unknown ExpNode kind\n");
                break;
            }
        }
        else
        {
            pce("Unknown node kind\n");
        }

        for (i = 0; i < MAXCHILDREN; i++){
            if(tree->nodekind == StmtK && tree->kind.stmt == AssignK && i == 0) {
                continue;
            }
            INDENT;
            printTree(tree->child[i]);
            UNINDENT;
        }

        tree = tree->sibling;
    }
}

const char *ExpTypeToString(ExpType type) {
    switch(type) {
        case Void: return "void";
        case Integer: return "int";
        default: return "unknown";
    }
}

void printLine() {
    char line[1024];
    static int current = 0;
    static int first = 1;
    if(first){
        rewind(redundant_source);
        first = 0;
    }
    const char *ret = fgets(line, sizeof(line), redundant_source);
    if(ret){
        current++;
        pc("%d: %s", current, line);
    
        if (feof(redundant_source)) {
            if(line[strlen(line) - 1] != '\n') {
                pc("\n");
            }
        }
    }
}