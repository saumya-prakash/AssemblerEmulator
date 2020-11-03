CC = g++
# FLAGS = 


all:	asm.o Assembler.o
			$(CC) asm.o Assembler.o -o asm


asm.o:	asm.cpp Assembler.h
			$(CC) -c asm.cpp


Assembler.o:	Assembler.cpp Assembler.h
					$(CC) -c Assembler.cpp


clean:		
		rm asm *.o

