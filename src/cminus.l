%{
#include "globals.h"
#include "util.h"
#include "scan.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

static void printCurrentToken(TokenType token, const char* tokenString) {
    pc("\t%d: ", lineno);
    printToken(token, tokenString);
}

%}

%option noyywrap

DIGIT           [0-9]
LETTER          [a-zA-Z]
IDENTIFIER      {LETTER}+
NUMBER          {DIGIT}+
NEWLINE         \n|\r\n
WHITESPACE      [ \t]+
COMMENT         "/*"([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+"/"

%%

{COMMENT}       { /* Processar cada newline no comentário para contar linhas */
                  char *s;
                  for (s = yytext; *s; s++) {
                      if (*s == '\n') {
                          lineno++;
                          printLine();
                      }
                  }
                }

"if"            { return IF; }
"else"          { return ELSE; }
"int"           { return INT; }
"return"        { return RETURN; }
"void"          { return VOID; }
"while"         { return WHILE; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return OVER; }
"<"             { return LT; }
"<="            { return LEQ; }
">"             { return GT; }
">="            { return GEQ; }
"=="            { return EQEQ; }
"!="            { return INEQ; }
"="             { return EQ; }
";"             { return SEMI; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"["             { return LBRACKETS; }
"]"             { return RBRACKETS; }
"{"             { return LCBRACES; }
"}"             { return RCBRACES; }

{NUMBER}        {
                    yylval.val = atoi(yytext);
                    return NUM;
                }
{IDENTIFIER}    {
                    yylval.name = copyString(yytext);
                    return ID;
                }
{NEWLINE}       { 
                    lineno++; 
                    printLine();
                }
{WHITESPACE}    { /* pular espaços em branco */ }
.               { return ERROR; }

%%

TokenType getToken(void)
{
    static int firstTime = TRUE;
    TokenType currentToken;
    
    if (firstTime)
    {
        firstTime = FALSE;
        yyin = source;
        yyout = listing;
        lineno++;
        printLine();
    }
    
    currentToken = yylex();
    strncpy(tokenString, yytext, MAXTOKENLEN);
    
    if (TraceScan) {
        printCurrentToken(currentToken, tokenString);
    }
    
    return currentToken;
}