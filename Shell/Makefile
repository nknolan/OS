CC = gcc

all: ish

ish: ish.o custom.o lex.yy.o
	$(CC) -o ish ish.o custom.o lex.yy.o -lfl
ish.o: ish.c
	$(CC) -Wall -g -c ish.c
custom.o: custom.c
	$(CC) -Wall -g -c custom.c
lex.yy.o: lex.yy.c
	$(CC) -Wall -g -c lex.yy.c
lex.yy.c: lex.c
	flex lex.c
clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f ish
