%{
#include <stdio.h>
#include "yacc.h"
%}

ws			[ \t]
letter		[A-Za-z]
digit		[0-9]
hexdigit	[0-9a-fA-F]
symbolchar	{letter}|[\.$_]
symbol		{symbolchar}({symbolchar}|{digit})*
gpreg		[ABCXYZIJabcxyzij]
xreg		SP|PC|EX|POP|PEEK|PUSH|PICK|sp|pc|ex|pop|peek|push|pick
op2			SET|ADD|SUB|MUL|MLI|DIV|DVI|MOD|MDI|AND|[BX]OR|SH[LR]|ASR|IF[BCENGALU]|ADX|SBX|ST[ID]
op2_lc		set|add|sub|mul|mli|div|dvi|mod|mdi|and|[bx]or|sh[lr]|asr|if[bcengalu]|adx|sbx|st[id]
op1			JSR|HCF|INT|RFI|IA[GSQ]|HW[NQI]
op1_lc		jsr|hcf|int|rfi|ia[gsq]|hw[nqi]


%%

:{symbol}			{ return LABEL; }
{symbol}:			{ return LABEL; }
0x{hexdigit}+			{ return NUMBER; }
{digit}+			{ return NUMBER; }
{gpreg}|{xreg}			{ return REG; }
{op2}|{op2_lc}			{ return OP2; }
{op1}|{op1_lc}			{ return OP1; }
{symbol}			{ return SYMBOL; }
\"(\\.|[^\\"])*\"		{ return STRING; }

[-,+*/|&^()~\[\]\n]	return *yytext;

[ \t\r]+	;		/* ignore whitespace and DOS line endings */
;.*			;		/* comment */
.					printf("lex error\n");

%%

int yywrap(void)
{
	return -1;
}
