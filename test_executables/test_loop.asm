default rel
global main

section .text
main:
    mov rdi, 40      
    add rdi, 2
    
.loop:
    jmp .loop       
