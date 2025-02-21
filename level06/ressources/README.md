# level06

This program is an authentication system that prompts the user for a username and a serial number. After reversing the program into C, we can see that it includes an anti-debugging mechanism using ptrace. The auth function performs a checksum on the username and compares the result with the user-provided serial number. Authentication is granted if both values match, allowing access to a shell. Bypassing the ptrace protection is straightforward: we can modify the `eax` register immediately after the `ptrace` call and then set a breakpoint at the end of the checksum algorithm to extract the expected serial number.

```
level06@OverRide:~$ gdb level06 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level06/level06...(no debugging symbols found)...done.
(gdb) set disassembly-flavor intel
(gdb) disas auth
Dump of assembler code for function auth:
   0x08048748 <+0>:	push   ebp
   0x08048749 <+1>:	mov    ebp,esp
   0x0804874b <+3>:	sub    esp,0x28
   0x0804874e <+6>:	mov    DWORD PTR [esp+0x4],0x8048a63
   0x08048756 <+14>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048759 <+17>:	mov    DWORD PTR [esp],eax
   0x0804875c <+20>:	call   0x8048520 <strcspn@plt>
   0x08048761 <+25>:	add    eax,DWORD PTR [ebp+0x8]
   0x08048764 <+28>:	mov    BYTE PTR [eax],0x0
   0x08048767 <+31>:	mov    DWORD PTR [esp+0x4],0x20
   0x0804876f <+39>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048772 <+42>:	mov    DWORD PTR [esp],eax
   0x08048775 <+45>:	call   0x80485d0 <strnlen@plt>
   0x0804877a <+50>:	mov    DWORD PTR [ebp-0xc],eax
   0x0804877d <+53>:	push   eax
   0x0804877e <+54>:	xor    eax,eax
   0x08048780 <+56>:	je     0x8048785 <auth+61>
   0x08048782 <+58>:	add    esp,0x4
   0x08048785 <+61>:	pop    eax
   0x08048786 <+62>:	cmp    DWORD PTR [ebp-0xc],0x5
   0x0804878a <+66>:	jg     0x8048796 <auth+78>
   0x0804878c <+68>:	mov    eax,0x1
   0x08048791 <+73>:	jmp    0x8048877 <auth+303>
   0x08048796 <+78>:	mov    DWORD PTR [esp+0xc],0x0
   0x0804879e <+86>:	mov    DWORD PTR [esp+0x8],0x1
   0x080487a6 <+94>:	mov    DWORD PTR [esp+0x4],0x0
   0x080487ae <+102>:	mov    DWORD PTR [esp],0x0
   ...
   0x08048866 <+286>:	cmp    eax,DWORD PTR [ebp-0x10]
   0x08048869 <+289>:	je     0x8048872 <auth+298>
   0x0804886b <+291>:	mov    eax,0x1
   0x08048870 <+296>:	jmp    0x8048877 <auth+303>
   0x08048872 <+298>:	mov    eax,0x0
   0x08048877 <+303>:	leave  
   0x08048878 <+304>:	ret    
End of assembler dump.
(gdb) b *0x080487ba
Breakpoint 1 at 0x80487ba
(gdb) b *0x08048866
Breakpoint 2 at 0x8048866
(gdb) r
Starting program: /home/users/level06/level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: plouvel
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 1234567

Breakpoint 1, 0x080487ba in auth ()
(gdb) i r eax
eax            0xffffffff	-1
(gdb) set $eax=0
(gdb) c
Continuing.

Breakpoint 2, 0x08048866 in auth ()
(gdb) i r eax
eax            0x12d687	1234567
(gdb) x/1wd $ebp-0x10
0xffffd6a8:	6234017
```

So, if we use the login `plouvel` we have to enter the value `6234017` to have a shell access ! Let's try :

```
level06@OverRide:~$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: plouvel
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6234017
Authenticated!
$ whoami
level07
$ cd ..
$ cat level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

Success !