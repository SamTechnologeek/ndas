%{
#include <string.h>
#include "tokens.h"
#include "output.h"
static int get_constant(void);
int linecount = 0;
extern union value lvalue;

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
dir1		org|align
dir2		equ|fill

%%

:{symbol}		{ lvalue.string = yytext + 1; return LABEL; }
{symbol}:		{
					lvalue.string = yytext;
					yytext[strlen(yytext) - 1] = 0;
					return LABEL;
			}
0x{hexdigit}+		return get_constant();
{digit}+		return get_constant();
{gpreg}			{ lvalue.integer = str2reg(yytext); return GPREG; }
{op2}|{op2_lc}		{ lvalue.integer = str2opcode(yytext); return OPCODE2; }
{op1}|{op1_lc}		{ lvalue.integer = str2opcode(yytext); return OPCODE1; }
{symbol}		{ lvalue.string = yytext; return SYMBOL; }
\"(\\.|[^\\"])*\"	{ lvalue.string = yytext; return STRING; }

[ \t\r]+		;	/* ignore whitespace and DOS line endings */
;.*			;	/* comment */
.			;	/* do nothing about the rest, screw you */
\n		linecount++;

%%

static int get_constant(void)
{
	lvalue.integer = (int)strtol(yytext, NULL, 0);
	return NUMBER;
}

int yywrap(void)
{
	return 1;
}
