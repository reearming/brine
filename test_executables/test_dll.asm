default rel
global main

extern fourfourfour

section .text
main:
    mov rdi, 40      
    add rdi, 2
    call [fourfourfour]
    add rdi, rax
    
.loop:
    jmp .loop       
