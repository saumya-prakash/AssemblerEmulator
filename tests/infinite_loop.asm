; This program demonstrates that the emulator stops after executing many instructions 
; and raises an infinite loop warning

lp: ldc 0   ; load 0 in A
br lp       ; go back to previous instruction


HALT    ; end the program