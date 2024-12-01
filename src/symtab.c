#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "symtab.h"
#include "util.h"

#define SIZE 211
#define SHIFT 4

typedef struct BucketListRec {
    char *name;
    char *scope;
    ExpType type;
    StmtKind idType;
    bool isArray;
    int numParams;
    LineList lines;
    ParamList params;
    struct BucketListRec *next;
} *BucketList;

static BucketList hashTable[SIZE];

static int hash(char *key) {
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

static char *makeKey(char *name, char *scope) {
    if (name == NULL) return NULL;
    if (scope == NULL) scope = "";
    
    char *key = (char *)malloc(strlen(name) + strlen(scope) + 2);
    if (key == NULL) return NULL;
    
    strcpy(key, name);
    strcat(key, "@");
    strcat(key, scope);
    return key;
}

void printLines(LineList t) {
    if (!t) {
        pc("\n");
        return;
    }
    pc(" ");
    while (t != NULL) {
        if (t->lineno != 0) {
            pc("%d", t->lineno);
            if (t->next != NULL) pc(" ");
        }
        t = t->next;
    }
    pc("\n");
}

static void insertLineOrdered(LineList *head, int lineno) {
    if (lineno == 0) return;
    
    // Verificar se a linha já existe
    LineList current = *head;
    while (current != NULL) {
        if (current->lineno == lineno) return;
        current = current->next;
    }

    // Criar novo nó
    LineList new_line = (LineList)malloc(sizeof(struct LineListRec));
    new_line->lineno = lineno;
    new_line->next = NULL;

    // Lista vazia ou inserção no início
    if (*head == NULL || (*head)->lineno > lineno) {
        new_line->next = *head;
        *head = new_line;
        return;
    }

    // Encontrar posição correta
    current = *head;
    while (current->next != NULL && current->next->lineno < lineno) {
        current = current->next;
    }

    new_line->next = current->next;
    current->next = new_line;
}

void st_insert(char *name, StmtKind idType, char *scope, ExpType type, bool isArray, int paramCount, int lineno) {
    if (name == NULL) return;
    if (scope == NULL) scope = "";

    char *key = makeKey(name, scope);
    if (key == NULL) return;

    int h = hash(key);
    BucketList l = hashTable[h];

    while ((l != NULL) && (strcmp(name, l->name) != 0 || strcmp(scope, l->scope) != 0))
        l = l->next;

    if (l == NULL) {
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = copyString(name);
        l->scope = copyString(scope);
        l->type = type;
        l->idType = idType;
        l->isArray = isArray;
        l->numParams = paramCount;
        l->lines = NULL;
        l->params = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    }
    insertLineOrdered(&(l->lines), lineno);
    free(key);
}

int st_lookup(char *name, char *scope) {
    if (name == NULL) return -1;
    if (scope == NULL) scope = "";

    char *key = makeKey(name, scope);
    if (key == NULL) return -1;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(name, l->name) == 0 && strcmp(scope, l->scope) == 0) {
            free(key);
            return 1;
        }
        l = l->next;
    }
    free(key);
    return -1;
}

ExpType st_gettype(char *name, char *scope) {
    if (name == NULL) return Void;
    if (scope == NULL) scope = "";

    char *key = makeKey(name, scope);
    if (key == NULL) return Void;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(name, l->name) == 0 && strcmp(scope, l->scope) == 0) {
            free(key);
            return l->type;
        }
        l = l->next;
    }
    free(key);
    return Void;
}

StmtKind st_getidtype(char *name, char *scope) {
    if (name == NULL) return -1;
    if (scope == NULL) scope = "";

    char *key = makeKey(name, scope);
    if (key == NULL) return -1;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(name, l->name) == 0 && strcmp(scope, l->scope) == 0) {
            free(key);
            return l->idType;
        }
        l = l->next;
    }
    free(key);
    return -1;
}

bool st_is_array(char *name, char *scope) {
    if (name == NULL) return false;
    if (scope == NULL) scope = "";

    char *key = makeKey(name, scope);
    if (key == NULL) return false;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(name, l->name) == 0 && strcmp(scope, l->scope) == 0) {
            free(key);
            return l->isArray;
        }
        l = l->next;
    }
    free(key);
    return false;
}

static ParamList newParam(TreeNode *paramNode) {
    if (paramNode == NULL) return NULL;

    ParamList p = (ParamList)malloc(sizeof(struct ParamListRec));
    p->name = copyString(paramNode->attr.name);
    p->type = paramNode->type;
    p->isArray = paramNode->isArray;
    p->next = NULL;
    return p;
}

void st_add_params(char *funcName, TreeNode *paramList) {
    if (funcName == NULL) return;
    char *key = makeKey(funcName, "");
    if (key == NULL) return;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(funcName, l->name) == 0 && l->scope[0] == '\0') {
            ParamList *last = &(l->params);
            while (paramList != NULL) {
                *last = newParam(paramList);
                last = &((*last)->next);
                paramList = paramList->sibling;
            }
            break;
        }
        l = l->next;
    }
    free(key);
}

ParamList st_getparams(char *funcName) {
    if (funcName == NULL) return NULL;
    char *key = makeKey(funcName, "");
    if (key == NULL) return NULL;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(funcName, l->name) == 0 && l->scope[0] == '\0') {
            free(key);
            return l->params;
        }
        l = l->next;
    }
    free(key);
    return NULL;
}

void st_add_lineno(char *name, char *scope, int lineno) {
    if (name == NULL || lineno == 0) return;
    if (scope == NULL) scope = "";

    char *key = makeKey(name, scope);
    if (key == NULL) return;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(name, l->name) == 0 && strcmp(scope, l->scope) == 0) {
            insertLineOrdered(&(l->lines), lineno);
            break;
        }
        l = l->next;
    }
    free(key);
}

void printSymTab(void) {
    pc("Variable Name  Scope     ID Type  Data Type  Line Numbers\n");
    pc("-------------  --------  -------  ---------  -------------------------\n");
    
    // Local variables first (scoped variables)
    for (int i = 0; i < SIZE; ++i) {
        BucketList l = hashTable[i];
        while (l != NULL) {
            if (l->scope[0] != '\0' && l->idType != FuncK) {
                pc("%-14s %-9s %-8s %-9s", 
                   l->name, l->scope,
                   l->isArray ? "array" : "var",
                   l->type == Integer ? "int" : "void");
                printLines(l->lines);
            }
            l = l->next;
        }
    }

    // Then functions (except input/output)
    for (int i = 0; i < SIZE; ++i) {
        BucketList l = hashTable[i];
        while (l != NULL) {
            if (l->scope[0] == '\0' && l->idType == FuncK && 
                strcmp(l->name, "input") != 0 && strcmp(l->name, "output") != 0) {
                pc("%-14s %-9s %-8s %-9s", 
                   l->name, "",
                   "fun",
                   l->type == Integer ? "int" : "void");
                printLines(l->lines);
            }
            l = l->next;
        }
    }

    // Built-in functions last
    pc("input          %-9s %-8s %-9s", "", "fun", "int");
    LineList lines = st_getlines("input", "");
    printLines(lines);

    pc("output         %-9s %-8s %-9s", "", "fun", "void");
    lines = st_getlines("output", "");
    printLines(lines);
}

LineList st_getlines(char *name, char *scope) {
    if (name == NULL) return NULL;
    if (scope == NULL) scope = "";

    char *key = makeKey(name, scope);
    if (key == NULL) return NULL;

    int h = hash(key);
    BucketList l = hashTable[h];

    while (l != NULL) {
        if (strcmp(name, l->name) == 0 && strcmp(scope, l->scope) == 0) {
            free(key);
            return l->lines;
        }
        l = l->next;
    }
    free(key);
    return NULL;
}