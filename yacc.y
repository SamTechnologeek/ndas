%{
#include <stdio.h>
int yylex();
%}

%union {
	int integer;
	char *string;
}

%token <string> SYMBOL LABEL STRING
%token <integer> NUMBER
%token <integer> REG
%token <integer> OP1 OP2

%defines "yacc.h"

%%

program:
	program line '\n'			{ /*printf("line\n");*/ }
	| program '\n'				/* empty line or comment */
	| program error '\n'		{ yyerrok; }
	|
	;

line:
	statement
	| label
	| label statement
	;							

label:
	LABEL						{ printf("label definition\n"); }
	;

statement:
	instr
	;

instr:
	OP2 operand ',' operand				{ printf("OP2\n"); }
	| OP1 operand					{ printf("OP1\n"); }
	;

operand:
	op_expr
	| '[' op_expr ']'			{ printf("indirect\n"); }
	;

op_expr:
	REG							{ printf("reg\n"); }
	| expr						{ printf("expression\n"); }
	| REG expr  /* PICK n */	{ printf("PICK n\n"); }
	| expr '+' REG				{ printf("expr + reg\n"); }
	| REG '+' expr				{ printf("reg + expr\n"); }
	;

expr:
	NUMBER						{ printf("number\n"); }
	| symbol					{ printf("symbol\n"); }
	;

symbol:
	SYMBOL						{ printf("SYMBOL I SAYYY\n"); }
	;

%%
