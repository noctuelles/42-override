#include <stdio.h>
#include <time.h>

static int decrypt(char key)
{
  size_t i;
  size_t len;
  char cyphertext[29];

  strcpy(cyphertext, "Q}|u`sfg~sf{}|a3");
  len = strlen(cyphertext);
  for (i = 0; i < len; ++i)
    cyphertext[i] ^= key;
  if (!strcmp(cyphertext, "Congratulations!"))
    return system("/bin/sh");
  else
    return puts("\nInvalid Password");
}

static int test(int a1, int a2)
{
  int result;
  char rnd_key;

  switch (a2 - a1)
  {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 16:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
    result = decrypt(a2 - a1);
    break;
  default:
    result = decrypt(rand());
    break;
  }

  return result;
}

int main()
{
  time_t now;
  int password;

  now = time(0);
  srand(now);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &password);
  test(password, 322424845);
  return 0;
}