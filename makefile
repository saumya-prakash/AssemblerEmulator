CC = gcc

FLAGS = -std=c89 -pedantic -W -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes



asm:	assembler.c tables.o
			$(CC) $(FLAGS) assembler.c tables.o -o asm


tables.o:	tables.h tables.c
			$(CC) -c $(FLAGS) tables.c


clean:	
		rm -i ./asm