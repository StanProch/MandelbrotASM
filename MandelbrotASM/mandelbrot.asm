.data
    ; Define floating-point constants in memory
    const2 REAL4 2.0
    const4 REAL4 4.0

.code

; extern "C" int IterateMandelbrot(float c_real, float c_imag);
; Windows x64 ABI passes first two float arguments in XMM0 and XMM1.
; The integer return value must go in EAX.
IterateMandelbrot PROC
    
    ; Initialize variables
    ; The RISC-V code sets Z_0 = C to save an iteration
    movaps xmm2, xmm0               ; Z_real (ft0) = C_real (fa0)
    movaps xmm3, xmm1               ; Z_imag (ft1) = C_imag (fa1)

    mov eax, 80                     ; Loop counter (a0) = 80
    movss xmm6, dword ptr [const2]  ; Load 2.0 (ft5)
    movss xmm7, dword ptr [const4]  ; Load 4.0 (for escape check)

again:
    ; Calculate squares: Z_real^2 and Z_imag^2
    movaps xmm4, xmm2
    mulss xmm4, xmm4                ; xmm4 (ft2) = Z_real^2
    
    movaps xmm5, xmm3
    mulss xmm5, xmm5                ; xmm5 (ft3) = Z_imag^2

    ; Calculate new Z_real = Z_real^2 - Z_imag^2 + C_real
    movaps xmm8, xmm4               
    subss xmm8, xmm5                ; xmm8 = Z_real^2 - Z_imag^2 (ft4)
    addss xmm8, xmm0                ; xmm8 = temp_real + C_real

    ; Calculate new Z_imag = 2 * Z_real * Z_imag + C_imag
    movaps xmm9, xmm2               
    mulss xmm9, xmm3                ; xmm9 = Z_real * Z_imag (ft6)
    mulss xmm9, xmm6                ; xmm9 *= 2.0 (ft7)
    addss xmm9, xmm1                ; xmm9 += C_imag

    ; Update Z variables for the next loop
    movaps xmm2, xmm8               ; Z_real = new Z_real
    movaps xmm3, xmm9               ; Z_imag = new Z_imag

    ; Decrement counter
    dec eax                         ; a0 = a0 - 1
    jz ret_end                      ; if (a0 == 0) return

    ; Escape check: Z_real^2 + Z_imag^2 <= 4.0
    ; We recalculate squares since Z just changed
    movaps xmm4, xmm2
    mulss xmm4, xmm4                ; new Z_real^2
    
    movaps xmm5, xmm3
    mulss xmm5, xmm5                ; new Z_imag^2
    
    addss xmm4, xmm5                ; xmm4 = magnitude squared

    ; Compare Magnitude Squared with 4.0
    ; (RISC-V converted this to an int, but proper x64 uses COMISS)
    comiss xmm7, xmm4               ; Compare 4.0 (xmm7) with Mag^2 (xmm4)
    jae again                       ; Jump to 'again' if 4.0 >= Mag^2

ret_end:
    ; The return value is currently in EAX (the remaining iterations).
    ; It will be 0 if trapped, or > 0 if it escaped early.
    ret
IterateMandelbrot ENDP

END