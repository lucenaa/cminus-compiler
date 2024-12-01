// symtab.h
#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"
#include <stdbool.h>

typedef struct ParamListRec {
    char *name;
    ExpType type;
    bool isArray;
    struct ParamListRec *next;
} *ParamList;

typedef struct LineListRec {
    int lineno;
    struct LineListRec *next;
} *LineList;

LineList st_getlines(char *name, char *scope);
void st_insert(char *name, StmtKind idType, char *scope, ExpType type, 
               bool isArray, int paramCount, int lineno);
int st_lookup(char *name, char *scope);
ExpType st_gettype(char *name, char *scope);
int st_getlen(char *name, char *scope);
bool st_is_array(char *name, char *scope);
StmtKind st_getidtype(char *name, char *scope);
void st_add_params(char *funcName, TreeNode *paramList);
ParamList st_getparams(char *funcName);
void st_add_lineno(char *name, char *scope, int lineno);
void printSymTab(void);

#endif