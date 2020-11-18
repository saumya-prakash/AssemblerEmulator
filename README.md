# AssemblerEmulator

A 2-pass assembler for a simple instruction set, along with an emulator to run the assembly programs.

The assembler flags errors and raises warnings. If assembly is successful, a listing file and an object file is produced. A log file is also produced to show warnings.

The object file generated is executable on the included Emulator only.

There is no linking multiple object-files functionality available. Every program has to be contained in a single source file only.


A program consists of code segment, followed by data directives. Functions and procedures should be declared after the 'main' part. There is no separate directive for function declaration.
 

