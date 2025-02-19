#include <stdbool.h>
#include <stdio.h>

char a_user_name[256];

static bool verify_user_name()
{
    puts("verifying username....\n");
    return memcmp(a_user_name, "dat_wil", 7u) != 0;
}

static bool verify_user_pass(const void *a1)
{
    return memcmp(a1, "admin", 5u) != 0;
}

int main()
{
    char s[64];
    bool is_equal;

    memset(s, 0, sizeof(s));
    is_equal = false;
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(a_user_name, 256, stdin);
    is_equal = verify_user_name();
    if (is_equal)
    {
        puts("nope, incorrect username...\n");
    }
    else
    {
        puts("Enter Password: ");
        fgets(s, 100, stdin);
        is_equal = verify_user_pass(s);
        puts("nope, incorrect password...\n");
    }
    return 1;
}