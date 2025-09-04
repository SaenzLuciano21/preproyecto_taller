%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "calc-sintaxis.tab.h"
#include "eval.h"
#include "codegen.h"

%}

%option noyywrap
%option yylineno

letra [a-zA-Z]      
digito [0-9]   

%%

"int"			{ return TINT; }
"bool"			{ return TBOOL; }
"void"			{ return TVOID; }
"main"			{ return TMAIN; }
"return"		{ return TRETURN; }
"true"			{ return CTRUE; }
"false"			{ return CFALSE; }

"="			{ return TASSIGN; }
"-"                     { return TMENOS; }    
[+*;(){}]               { return *yytext;}

{digito}+                     { yylval.ival = atoi(yytext); return INT; }
{letra}({letra}|{digito})*    { yylval.sval = strdup(yytext); return ID; }

[ \t\r]+        ;
"//".*          ;
"/*"([^*]|\*+[^/])*"*/"   ;

.|\n                          ;

%%

void yyerror(const char *s){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]){
    ++argv,--argc;
    if (argc > 0)
	yyin = fopen(argv[0],"r");
    else
	yyin = stdin;

    yyparse();
    printf("====== AST ======\n");
    if(root)
    	print_ast(root, 0);
	if (yyin != stdin) fclose(yyin);
	
	printf("\n====== EVAL ======\n");
	Value res = eval_program(root);
	if (res.type == VAL_INT)  printf("Resultado: %d\n", res.i);
	if (res.type == VAL_BOOL) printf("Resultado: %s\n", res.b ? "true" : "false");
	if (res.type == VAL_VOID) printf("Resultado: <void>\n");

	// Generar pseudo-assembly
	printf("\n====== ASM ======\n");
	gen_program(root, stdout);
	  
    return 0;

}
