#include <stdio.h>

int main()
{
  char password[96];
  char pass_buffer[48];
  char username[96];
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
  if (strncmp(pass_buffer, password, 0x29uLL))
  {
    printf(username);
    puts(" does not have access!");
    exit(1);
  }
  printf("Greetings, %s!\n", username);
  system("/bin/sh");
  return 0;
}