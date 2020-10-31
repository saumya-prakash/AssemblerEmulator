CC = gcc

FLAGS = -std=c89 -pedantic -W -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes



asm:	assembler.o tables.o
			$(CC) $(FLAGS) assembler.o tables.o -o asm


assembler.o:	assembler.c
					$(CC) -c $(FLAGS) assembler.c

tables.o:	tables.h tables.c
			$(CC) -c $(FLAGS) tables.c


clean:	
		rm  a.out asm tables.o assembler.o