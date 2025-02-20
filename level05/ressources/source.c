#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char buffer[100];

  fgets(buffer, 100, stdin);
  for (size_t i = 0; i < strlen(buffer); ++i )
  {
    if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
      buffer[i] += 32; /* Upper to lowercase */
    }
  }
  printf(buffer);
  exit(0);
}