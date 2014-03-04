%{

int linecount = 0;

%}

%%

\n		linecount++;
\t		printf("status: %d\n", linecount);
[a-zA-Z]	printf("nice character!\n");

%%

int yywrap(void)
{
	return 1;
}
