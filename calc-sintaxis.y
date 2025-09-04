%{
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

/* Prototipos para evitar warnings */
int yylex(void);
void yyerror(const char *s);

%}

/* Tokens desde el lexer */
%token <ival> INT
%token <sval> ID
%token TMENOS
%token TINT TBOOL TVOID TMAIN TRETURN
%token CTRUE CFALSE
%token TASSIGN

%union {
    int ival;
    char* sval;
    ASTNode* node;
}

/* Asociaciones */
%type <node> programa declaraciones declaracion sentencias sentencia asignacion retorno expr VALOR
%type <sval> tipo

/* Precedencias */
%left '+' TMENOS 
%left '*'

%%

/* programa: tipo main() { declaraciones sentencias } */
programa: tipo TMAIN '(' ')' '{' declaraciones sentencias '}'
            {
                root = create_program_node($1, $6, $7);
                $$ = root;
            }
    	;

/* Tipos permitidos para main: int | bool | void */
tipo: TINT  { $$ = strdup("int"); }
    | TBOOL { $$ = strdup("bool"); }
    | TVOID { $$ = strdup("void"); }
    ;

/* Cero o más declaraciones */
declaraciones
    : /* vacío */   { $$ = create_statements_node(); }
    | declaraciones declaracion {
        if (!$1) $1 = create_statements_node();
        append_statement($1, $2);
        $$ = $1;
    }
    ;

/* Cada declaración termina en ';' */ 
declaracion
    : TINT ID ';'   { $$ = create_decl_node("int", $2); }
    | TBOOL ID ';'  { $$ = create_decl_node("bool", $2); }
    ;

/* Cero o más sentencias */
sentencias
    : /* vacío */   { $$ = create_statements_node(); }
    | sentencias sentencia  {
        if (!$1) $1 = create_statements_node();
        append_statement($1, $2);
        $$ = $1;
    }
    ;

/* Sentencias: asignación o return (con/sin expresión) */
sentencia
    : asignacion    { $$ = $1; }
    | retorno       { $$ = $1; }
    ;

/* Asignación: ID = expr ; */
asignacion
    : ID TASSIGN expr ';'   { $$ = create_assign_node($1, $3); }
    ;

/* Return: 'return ;' o 'return expr ;' */
retorno
    : TRETURN ';'       { $$ = create_return_node(NULL); }
    | TRETURN expr ';'  { $$ = create_return_node($2); }
    ;

/* Expresiones aritméticas/lógicas */
expr
    : VALOR             { $$ = $1; }
    | expr '+' expr     { $$ = create_binop_node(OP_ADD, $1, $3); }
    | expr '*' expr     { $$ = create_binop_node(OP_MUL, $1, $3); }
    | expr TMENOS expr  { $$ = create_binop_node(OP_SUB, $1, $3); }
    | '(' expr ')'      { $$ = $2; }
    ;

/* Valores: enteros, identificadores o constantes lógicas */
VALOR
    : INT       { $$ = create_int_node($1); }
    | ID        { $$ = create_id_node($1); }
    | CTRUE     { $$ = create_bool_node(1); }
    | CFALSE    { $$ = create_bool_node(0); }
    ;

%%

