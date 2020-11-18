ldc num
ldnl 0
call recur
HALT


recur:  adj -1
        stl 0   ; storing return address on the stack

        call recur

        ldl 0   ; get return address
        adj 1   ; restore SP

        return


num:    data 1000