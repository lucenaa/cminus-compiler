#ifndef _UTIL_H_
#define _UTIL_H_
#include "globals.h"
/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char* tokenString);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode* newStmtNode(StmtKind kind);

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode* newExpNode(ExpKind kind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char* copyString(char* s);

/* Procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode* tree);

/* Procedure printLine prints a source line from
 * the source file for reference in lexical analysis
 */
void printLine();

const char* ExpTypeToString(ExpType type);

#endif