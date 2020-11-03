lp1:
lp2:
lp3:
lp4:

	lp5:	ldc 20

label:	; an unused label
	ldc 0
	ldc -5
	ldc +5

loop:
 		br loop ; an infinite loop
br next	;offset should be zero
next:
    	ldc loop ; load code address
	ldc var1 ; forward ref
var1:data 0 ; a variable

SET 0x33


