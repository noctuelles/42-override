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