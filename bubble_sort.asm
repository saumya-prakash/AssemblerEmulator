; outline of bubble sort program
        ldc 0x1000
        a2sp
        adj -1
        ldc   10
        stl   0
        ldc   array
        call  sort
        HALT
;
; Areg= return address
; Breg= address of elements
; SP+0= number of elements
sort: 
        adj -1     ; space for storing return address
        stl 0      ; return address now on stack, reg A=reg B =array address

        adj -1
        stl 0      ; array address saved on stack

        ldl 2      ; get number of elements
        adc -1          ; decrement by 1
        stl 2           ; store it -> now it will serve as number of outer-loop passes required

        adj -2

        lp1:    adj 2
                ldl 2
                brz done
                brlz done       ; sorting done -> prepare to RETURN

                adj -1
                stl 0      ; number of iterations in current pass required stored

                ldl 0
                adc -1
                stl 3      ; 'i--' and stored on stack

                adj -1
                ldl 2
                stl 0      ; storing current element address

                lp2:    ldl 1
                        brz lp1         ; begin next outer loop trip
                        brlz lp1        ; <0 -> begin next outer loop
                        adc -1          ; 'j--'
                        stl 1           ; stored back on the stack

                        ldl 0           ; get current element address
                        ldnl 0          ; get that data from memory
                        ldc 0           ; tranfer it to B
                        ldl 0           ; get current element address
                        ldnl 1          ; get data one ahead in A

                        sub             ; A=B-A
                        brlz fine
                        brz fine
                                        ; swapping required
                        ldl 0           ; get current element address
                        ldnl 1          ; get one ahead data element
                        adj -1
                        stl 0           ; store it temporarily on stack

                        ldl 1           ; get current element address
                        ldnl 0          ; get current data element
                        ldc 0           ; move it to B
                        
                        ldl 1           ; get current element address
                        stnl 1          ; put B in correct place

                        ldl 0           ; get other data element
                        ldc 0           ; move it to B
                        ldl 1           ; get current element address
                        stnl 0          ; put B in right place

                        adj 1           ; pop this temporary saved value


                        fine:   ldl 0   ; get current element address
                                adc 1   ; move to next address
                                stl 0   ; put it back on stack
                        
                        br lp2          ; continue this inner loop


        done:   adj 1   ; point to return address
                ldl 0   ; put return address in A
                adj 1   ; point to position that was there when called

                return  ; return 


;ok, you're on your own now
      return

array:  data    -559038737
	data    1592639189
      	data    -1058161986
	data    1588527317
        data    -1420028582
	data    -899895827
	data    -17958194
	data    -1061097459
	data    -556882451
	data    1358610133

