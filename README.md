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


