ldc num     ; get address of num
ldnl 0      ; get value of num in A

ldc 1       ; multiplying by 2, so 1 left-shift required   
shl         ; multiply by 2 by shifting-left

ldc num     ; get address of num       

stnl 0      ; store the result

HALT        ; end the program


num:  data 17   ; variable num initialised to 17