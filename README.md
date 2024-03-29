# AssemblerEmulator

A 2-pass assembler for a simple instruction set, along with an emulator to run the assembled programs.

If assembly is successful, a listing file and an object file is produced. A log file is also produced to show warnings.

The object file generated is executable on the included Emulator only.

There is no support for linking multiple object-files. Every program written in this instruction set should be in one program file only. 
 

## Assembly Language

The assembly language is for a machine with 4 registers:

* 2 registers, A and B, arranged as an internal stack
* A program counter, PC
* A stack pointer, SP

* The registers are _32-bits_ in size.
* Instructions have either no operands or a single opeand. The operand, if present, is a signed 2's complement value. An operand can be a label as well, or a number in decimal, octal or hexadecimal form. The encoding usess **bottom 8 bits** for the **opcoe** and the **upper 24 bits** for the **operand**.

* The language is line-based (one statement per line), like most other assembly languages.
* Comments begin with a **;** and anything on the line after **;** is ignored.
* Blank lines, spaces and tabs are allowed.
* **label definition** - A label name followed by a **:** and an optinal statement. A label name is an alphanumeric string starting with a letter. A label use is just the label name. For branch instructions, label use calculates the displacement and for other instructions, the label value is used directly.

## The Instructions

**_NOTE:_** The instructions semantics do not show the incrementing of the _PC_ to the next instruction. This is implicitly performed by each instruction before the actions of the instruction are done.


| Mnemonic | Opcode | Operand | Formal Specification | Description |
|----------|--------|---------|----------------------|-------------|
| data | | value | | Reserves a memory location initialized to the value specified |
| ldc | 0 | value | B:=A; A:=value; | Load accumulator with the value specified |
| adc | 1 | value | A := A + value; | Add the value specified to the accumulator |
| ldl | 2 | offset | B := A; A := memory[SP+offset]; | Load local |
| stl | 3 | offset | memory[SP+offset] := A; A := B | Store local |
| ldnl | 4 | offset | A := memory[A+offset]; | Load non-local |
| stnl | 5 | offset | memory[A+offset] := B; | Store non-local |
| add | 6 | | A := B+A; | Addition |
| sub | 7 | | A := B-A; | Subtraction |
| shl | 8 | | A := B<<A; | Shift left |
| shr | 9 | | A := B>>A; | Shift right |
| adj | 10 | value | SP := SP+value; | Adjust SP |
| a2sp | 11 | | SP:=A; A:=B; | Transfer A to SP |
| sp2a | 12 | | B:=A; A:=SP; | Transfer SP to A |
| call | 13 | offset | B:=A; A:=PC; PC:=PC+offset; | Call procedure |
| return | 14 | | PC:=A; A := B; | Return from procedure |
| brz | 15 | offset | if A == 0 then PC:=PC+offset; | If accumulator is zero, branch to specified offset |
| brlz | 16 | offset | if A < 0 then PC:=PC+offset;| If accumulator is less than , branch to specified offset |
| br | 17 | offset | PC:=PC+offset; | Branch to specified offset |
| HALT | 18 | | | Stop the emulator. This is not a real instruction, but needed to tell the emulator when to finish. |
| SET | | value | | Set the label on this line to the specified value, rather than the PC. |








