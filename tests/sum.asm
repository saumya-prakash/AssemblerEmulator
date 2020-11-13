; A program to calculate and store the sum of two given numbers

ldc a       ; get address of a
ldnl 0      ; store a in reg A

ldc b       ; get address of b; A's shifted to B
ldnl 0      ; store b in reg A

add         ;  A = B+A

ldc result  ; get result address; A's shifted to B
stnl 0      ; store B's

HALT        ; end the program



a:  data 4
b:  data 5

result:    data 0   ; resut will be stored here