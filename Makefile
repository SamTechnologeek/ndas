CC=gcc
OPT=-g
PROG=ndas

all: $(PROG)

$(PROG): main.o token.o
	@$(CC) -o $(PROG) main.o token.o $(OPT)
	@echo " LINK $(PROG)"

main.o: main.c opt.h def.h token.h
	@$(CC) -c main.c $(OPT)
	@echo " CC   $<"

token.o: token.c def.h token.h
	@$(CC) -c token.c $(OPT)
	@echo " CC   $<"

clean:
	rm -f $(PROG)
	rm -f *.o
