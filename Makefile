CC=gcc
OPT=-Wall -ansi -g
OUT=ndas

all: $(OUT)

$(OUT): *.o
	$(CC) -o $(OUT) *.o $(OPT)

*.o: *.c
	$(CC) -c *.c $(OPT)

*.c: *.h

clean:
	rm -f *.o
	rm -f $(OUT)
