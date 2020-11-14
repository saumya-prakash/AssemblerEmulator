CC = g++
FLAGS = -O3


target:	asm emu


asm:	asm.o Assembler.o	
			$(CC) $(FLAGS) asm.o Assembler.o -o asm

emu:	emu.o	Emulator.o
			$(CC) $(FLAGS) emu.o Emulator.o -o emu



asm.o:	asm.cpp Assembler.h
			$(CC) $(FLAGS) -c asm.cpp

Assembler.o:	Assembler.cpp Assembler.h
					$(CC) $(FLAGS) -c Assembler.cpp



emu.o:	emu.cpp Emulator.h
			$(CC) $(FLAGS) -c emu.cpp

Emulator.o:	Emulator.h Emulator.cpp
				$(CC) $(FLAGS) -c Emulator.cpp



clean:		
		rm -f asm emu a.out *.o *.l *.log

