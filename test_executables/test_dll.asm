default rel
global main

extern __imp_fourfourfour

section .text
main:
    sub rsp, 48
    mov rdi, 40      
    add rdi, 2
    call [qword __imp_fourfourfour]
    add rdi, rax
    add rsp, 48

.loop:
    jmp .loop       
