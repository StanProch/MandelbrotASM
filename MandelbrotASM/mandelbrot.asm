.code

; W architekturze x64 pierwsze dwa argumenty ca³kowite przekazywane s¹ w rejestrach RCX i RDX
; Wartoœæ zwracana z funkcji trafia do rejestru RAX
CalculateMath PROC
    mov rax, rcx    ; Przenieœ pierwszy argument do RAX
    add rax, rdx    ; Dodaj drugi argument do RAX
    ret             ; Wróæ do C++
CalculateMath ENDP

END