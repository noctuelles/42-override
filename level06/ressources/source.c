#include <stdio.h>
#include <stdbool.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdlib.h>

static bool auth(char *login, unsigned int serial)
{
    int login_checksum;
    size_t login_len;

    login[strcspn(login, "\n")] = 0;
    login_len = strnlen(login, 32);
    if (login_len <= 5)
        return 1;
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        puts("\x1B[32m.---------------------------.");
        puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1B[32m'---------------------------'");
        return true;
    }
    login_checksum = (login[3] ^ 0x1337) + 6221293;

    for (size_t i = 0; i < login_len; ++i)
    {
        /* Exclude control characters. */
        if (login[i] < ' ')
            return 1;

        login_checksum += (login_checksum ^ (unsigned int)login[i]) % 0x539;
    }

    return serial != login_checksum;
}

int main()
{
    unsigned int serial;
    char login[32];

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");
    fgets(login, 32, stdin);
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: ");
    scanf("%u", &serial);
    if (auth(login, serial))
        return 1;
    puts("Authenticated!");
    system("/bin/sh");
    return 0;
}