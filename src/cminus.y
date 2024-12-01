/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#ifndef _PARSE_H_
#define _PARSE_H_

TreeNode* parse(void);

#endif

static char * savedName; /* for use in assignments */
static int savedLineNo;
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);
int yyerror(char *s);
%}

%union {
    int val;
    char *name;
    TokenType token;
    TreeNode* node;
    ExpType type;
}

%token <name> ID
%token <val> NUM
%token <type> INT VOID
%token IF ELSE RETURN WHILE ERROR
%token PLUS MINUS TIMES OVER
%token LT LEQ GT GEQ EQ INEQ
%token EQEQ
%token SEMI COMMA
%token LPAREN RPAREN LBRACKETS RBRACKETS LCBRACES RCBRACES

/* Type declarations */
%type <node> program decl_list decl var_decl var fun_decl
%type <node> params param_list param comp_decl loc_decl
%type <node> stmt_list stmt exp_decl sel_decl while_stmt return_stmt
%type <node> exp simple_exp term factor call args arg_list sum_exp
%type <token> sum mult relational
%type <type> type_spec

%left PLUS MINUS
%left TIMES OVER
%nonassoc LT LEQ GT GEQ
%nonassoc EQEQ INEQ
%right EQ
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE




%% /* Grammar for Cminus */

program     : decl_list
                {
                    savedTree = $1;
                }
            ;

decl_list    : decl_list decl {
                TreeNode *t = $1;
                if (t != NULL) {
                  while (t->sibling != NULL) {
                    t = t->sibling;
                  }
                  t->sibling = $2;
                  $$ = $1;
                } else { $$ = $2; }
            }
            | decl  { $$ = $1; }
            ;

decl        : var_decl { $$ = $1; }
            | fun_decl { $$ = $1; }
            ;

var_decl    : type_spec ID SEMI{ 
                $$ = newStmtNode(VarK);
                $$->attr.name = $2;
                $$->type = $1;
              }
            | type_spec ID LBRACKETS NUM RBRACKETS SEMI{
                $$ = newStmtNode(VarK);
                $$->attr.name = $2;
                $$->type = $1;
                $$->child[0] = newExpNode(ConstK);
                $$->child[0]->attr.val = $4;
            }
            ; 

type_spec   : INT {
                $$ = Integer;
            }
            | VOID {
                $$ = Void;
            }
            ;

fun_decl    : type_spec ID LPAREN params RPAREN comp_decl {
                  $$ = newStmtNode(FuncK);
                  $$->attr.name = $2;
                  $$->type = $1;
                  $$->child[0] = $4;
                  $$->child[1] = $6;
                }
            ;
            
params      : param_list 
                { 
                $$ = $1; 
                }
            | VOID {
                $$ = NULL;
            }
            ;

param_list  : param_list COMMA param {
                TreeNode* t = $1;
                if (t != NULL) {
                    while (t->sibling) 
                        t = t->sibling;
                    t->sibling = $3;
                    $$ = $1;
                } else { 
                    $$ = $3; 
                }
            }
            | param 
                {$$ = $1; }
            ;
            
param       : type_spec ID {
                $$ = newStmtNode(ParamK);
                $$->attr.name = $2;
                $$->type = $1;
                $$->isArray=0;
            }
            | type_spec ID LBRACKETS RBRACKETS {
                $$ = newStmtNode(ParamK);
                $$->attr.name = $2;
                $$->type=$1;
                $$->isArray=1;
            }
            ;

comp_decl   : LCBRACES loc_decl stmt_list RCBRACES {
                TreeNode* t = $2;
                if (t != NULL) {
                  while (t->sibling != NULL) {
                    t = t->sibling;
                  }
                  t->sibling = $3;
                  $$ = $2;
                } else { $$ = $3; }
            }
            ;

loc_decl    : loc_decl var_decl {
                TreeNode* t = $1;
                if (t != NULL) {
                  while (t->sibling != NULL) {
                    t = t->sibling;
                  }
                  t->sibling = $2;
                  $$ = $1;
                } else { $$ = $2; }
            }
            | %empty { $$ = NULL; }
            ;

stmt_list   : stmt_list stmt {
                TreeNode* t = $1;
                if (t != NULL) {
                  while (t->sibling != NULL) {
                    t = t->sibling;
                  }
                  t->sibling = $2;
                  $$ = $1;
                } else { $$ = $2; }
            }
            | %empty { $$ = NULL; }
            ;

stmt        : exp_decl { $$ = $1; }
            | comp_decl { $$ = $1; }
            | sel_decl { $$ = $1; }
            | while_stmt { $$ = $1; }
            | return_stmt { $$ = $1; } 
            ;

exp_decl    : exp SEMI { $$ = $1; }
            | SEMI { $$ = NULL; }
            ;

sel_decl    : IF LPAREN exp RPAREN stmt %prec LOWER_THAN_ELSE {
                $$ = newStmtNode(IfK);
                $$->child[0] = $3;
                $$->child[1] = $5;
              }
            | IF LPAREN exp RPAREN stmt ELSE stmt {
              $$ = newStmtNode(IfK);
              $$->child[0] = $3;
              $$->child[1] = $5;
              $$->child[2] = $7;
            }
            ;

while_stmt: 
    WHILE LPAREN exp RPAREN stmt {
        $$ = newStmtNode(WhileK);
        $$->child[0] = $3;
        $$->child[1] = $5;
    }
    ;

return_stmt:
    RETURN SEMI {
        $$ = newStmtNode(ReturnK);
    }
    | RETURN exp SEMI {
        $$ = newStmtNode(ReturnK);
        $$->child[0] = $2;
    }
    ;

exp         : var EQ exp {
                $$ = newStmtNode(AssignK);
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
            | simple_exp {  $$ = $1; }
            ;

var         : ID {
                  $$ = newExpNode(IdK);
                  $$->attr.name = $1;
              }
            | ID LBRACKETS exp RBRACKETS {
                $$ = newExpNode(IdK);
                $$->attr.name = $1;
                $$->child[0]=$3;
              }
            ;

simple_exp  : sum_exp relational sum_exp {
              $$ = newExpNode(OpK);
              $$->child[0] = $1;
              $$->child[1] = $3;
              $$->attr.op = $2;
            }
            | sum_exp { $$ = $1;}
            ;

relational  : LEQ {
                $$ = LEQ;
            }
            | LT {
                $$ = LT;
            }
            | GT {
                $$ = GT;
            }
            | GEQ {
                $$ = GEQ;
            }
            | EQEQ {
                $$ = EQEQ;
            }
            | INEQ {
                $$ = INEQ;
            }
            ;

sum_exp     : sum_exp sum term {
              $$ = newExpNode(OpK);
              $$->child[0] = $1;
              $$->child[1] = $3;
              $$->attr.op = $2;
            }
            | term { $$ = $1; }
            ;

sum         : PLUS {
                $$ = PLUS;
            }
            | MINUS {
                $$ = MINUS;
             }
            ;

term        : term mult factor {
                $$ = newExpNode(OpK);
                $$->child[0] = $1;
                $$->child[1] = $3;
                $$->attr.op = $2;
            }
            | factor { $$ = $1; }
            ;

mult        : TIMES {
                $$ = TIMES;
            }
            | OVER {
                $$ = OVER;
            }
            ;

factor      : LPAREN exp RPAREN { $$ = $2; }
            | var { $$ = $1; }
            | call { $$ = $1; }
            | NUM {
              $$ = newExpNode(ConstK);
              $$->attr.val = $1;
            }
            ;

call        : ID LPAREN args RPAREN {
                $$ = newExpNode(CallK);
                $$->attr.name = $1;
                $$->child[0] = $3;
            }
            ;

args        : arg_list {
                 $$ = $1;
              }
            | %empty { $$ = NULL; }
            ;

arg_list    : arg_list COMMA exp {
                TreeNode *t = $1;
                if (t != NULL) {
                  while (t->sibling != NULL) {
                    t = t->sibling;
                  }
                  t->sibling = $3;
                  $$ = $1;
                } else {
                  $$ = $3;
                }
              }
            | exp {  $$ = $1;  }
            ;
%%

int yyerror(char * message)
{ pce("Syntax error at line %d: %s\n",lineno,message);
  pce("Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ 
    yyparse();
    return savedTree;
}