default rel
global main

section .text
main:
    mov rax, 40      
    add rax, 2       
    
.loop:
    jmp .loop       
