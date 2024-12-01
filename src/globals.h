#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "log.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 6

/* TokenType declaration */
typedef int TokenType;

/* File globals.h */
extern FILE *source;
extern FILE *listing;
extern FILE *code;
extern FILE *redundant_source;
/* Global variables */
extern int lineno;

typedef enum {StmtK, ExpK} NodeKind;
typedef enum {
    IfK,
    WhileK,   
    AssignK,
    ReturnK,    
    ParamK,
    VarK,
    FuncK
} StmtKind;

typedef enum {
    OpK,
    ConstK,
    IdK,
    CallK
} ExpKind;

typedef enum {
    Void,
    Integer,
    Boolean
} ExpType;

#define MAXCHILDREN 3

/* Tree node structure */
typedef struct treeNode {
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodekind;
    union {
        StmtKind stmt;
        ExpKind exp;
    } kind;
    union {
        TokenType op;
        int val;
        char *name;
    } attr;
    ExpType type;
    int isArray;
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;

#ifndef YYPARSER
#include "parser.h"
#define ENDFILE 0
#endif

#endif