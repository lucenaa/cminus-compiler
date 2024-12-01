#ifndef _ANALYZE_H_
#define _ANALYZE_H_

/* Tamanho máximo para strings de escopo */
#define MAX_SCOPE_SIZE 256
#define MAX_LEN 100

/* Função para reportar erros semânticos
 * t = nó da árvore onde ocorreu o erro
 * message = mensagem de erro base
 * aux = string auxiliar para formatação (nome de variável, etc)
 */
void semanticError(TreeNode * t, char * message, char * aux);

/* Function buildSymtab constrói a tabela de símbolos */
void buildSymtab(TreeNode * syntaxTree);

/* Procedure typeCheck realiza verificação de tipos */
void typeCheck(TreeNode * syntaxTree);

/* Procedure mainCheck verifica se função main existe */
void mainCheck(TreeNode * syntaxTree);

#endif