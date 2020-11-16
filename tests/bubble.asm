; outline of bubble sort program
        ldc 0x1000
        a2sp
        adj -1
        ldc   0xa
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
        stl 0      ; return address now on stack, A == B == array address

        adj -1
        stl 0      ; array address saved on stack, A == B == array address

        ldl 2      ; get number of elements
        adc -1          ; decrement by 1
        stl 2           ; store it -> now it will serve as number of outer-loop passes required

        adj -2          ; allocate space for outer loop operations

        lp1:
            adj 1            ; adjustment required
            ldl 3       ; get current value
            brz done
            brlz done

            stl 0       ; store temporarily for inner loop

            ldl 0
            adc -1
            stl 3       ; decrement outer counter by 1 and store it

            adj -1
            ldl 2
            stl 0   ; store array address for inner loop's use

            lp2:        ; represents inner loop
                ldl 1
                brz lp1

                adc -1
                stl 1   ; decrease count and store

                ldl 0   ; current element address

                ldnl 0  ; get current array element

                ldl 0
                adc 1
                stl 0

                ldl 0
                ldnl 0      ; pointer incremented , A contains arr[K+1] , B contains arr[K]

                adj -2          ; store these 2 elements on stack
                stl 1           ; arr[k+1]
                stl 0           ; arr[k]

                ldl 1
                ldl 0           ; now, A contains arr[k], B contains arr[k+1]
                                
                ;                 ; a better comparator required
                ; sub             ; A = B-A
                ; brlz    swap
                ;                 ;;;;;;;;;;

                brlz cs1
                ldl 1
                br swap
                ldl 1
                ldl 0
                sub
                brlz swap

             
                                ; else, everything is good
        back:   adj 2          ; adjust stack to previous condition
                br lp2


        done:   adj 2   ; point to return address
                ldl 0   ; put return address in A
                adj 1   ; point to position that was there when called

                return  ; return 


        swap:   ldl 0
                ldl 2
                stnl 0

                ldl 1
                ldl 2
                stnl -1

                br back

        cs1:    ldl 1
                brlz sub11
                br back
        
        sub11:  ldl 1
                ldl 0
                sub
                brlz swap
                br back


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

