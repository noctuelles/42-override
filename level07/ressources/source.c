#include <stdio.h>
#include <stdint.h>

static int clear_stdin()
{
    int result;

    do
        result = getchar();
    while ((char)result != '\n' && (char)result != 0xFF);

    return result;
}

static uint32_t get_unum()
{
    int buffer[3];

    buffer[0] = 0;
    fflush(stdout);
    scanf("%u", buffer);
    clear_stdin();
    return buffer[0];
}

static int store_number(uint32_t *array)
{
    uint32_t number;
    uint32_t index;

    printf(" Number: ");
    number = get_unum();
    printf(" Index: ");
    index = get_unum();
    if ((index % 3 == 0) || number >> 24 == 0xB7)
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }
    else
    {
        array[index] = number;
        return 0;
    }
}

static int read_number(uint32_t *array)
{
    int unum;

    printf(" Index: ");
    unum = get_unum();
    printf(" Number at data[%u] is %u\n", unum, array[unum]);
    return 0;
}

int main(int argc, const char **argv, const char **envp)
{
    uint32_t array[100];
    int number;
    char s[4];

    number = 0;
    *(uint32_t *)s = 0;
    memset(array, 0, sizeof(array));
    while (*argv)
    {
        memset((void *)*argv, 0, strlen(*argv));
        ++argv;
    }
    while (*envp)
    {
        memset((void *)*envp, 0, strlen(*envp));
        ++envp;
    }
    puts(
        "----------------------------------------------------\n"
        "  Welcome to wil's crappy number storage service!   \n"
        "----------------------------------------------------\n"
        " Commands:                                          \n"
        "    store - store a number into the data storage    \n"
        "    read  - read a number from the data storage     \n"
        "    quit  - exit the program                        \n"
        "----------------------------------------------------\n"
        "   wil has reserved some storage :>                 \n"
        "----------------------------------------------------\n");
    while (1)
    {
        printf("Input command: ");
        number = 1;
        fgets(s, 20, stdin);
        s[strlen(s) - 1] = '\0';
        if (!memcmp(s, "store", 5u))
        {
            number = store_number(array);
        }
        else if (!memcmp(s, "read", 4u))
        {
            number = read_number(array);
        }
        else if (!memcmp(s, "quit", 4u))
        {
            return 0;
        }

        if (number)
        {
            printf(" Failed to do %s command\n", s);
        }
        else
        {
            printf(" Completed %s command successfully\n", s);
        }

        *(uint32_t *)s = 0;
    }
}