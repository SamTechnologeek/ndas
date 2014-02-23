CC=gcc
OPT=-O3
PROG=ndas

all: main.o
	@$(CC) -o $(PROG) main.o $(OPT)
	@echo " LINK $(PROG)"

main.o: main.c opt.h def.h
	@$(CC) -c main.c $(OPT)
	@echo " CC   $<"

clean:
	rm -f $(PROG)
	rm -f *.o
