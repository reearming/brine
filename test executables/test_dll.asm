default rel
global main

extern fourfourfour
extern printf

section .data
message: db "hello", 0

section .text
main:
    mov rdi, message
    xor eax, eax
    call printf

    mov rdi, 40      
    add rdi, 2
    call [fourfourfour]
    add rdi, rax
    
.loop:
    jmp .loop
