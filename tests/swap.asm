; A program to value of two data locations

ldc a       ; get address of a
ldnl 0      ; store a in A

ldc b       ; get address of b ; shift A's into B
ldnl 0      ; store b in A

adj -1      ; make space on stack
stl 0       ; put A's on stack

ldc b       ; get address of b
stnl 0      ; store value of a at b

ldl 0       ; get value from stack
ldc a       ; get address of a
stnl 0      ; store value of b at a

adj 1       ; put SP to original location

HALT        ; end the program


a:  data 23      ; first data
b:  data 17      ; second data

