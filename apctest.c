#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <apc.h>

/* main code */
void main()
{
  char *str = malloc(500);
  
  /* read expr */
  printf("expr: ");
  scanf("%[^\n]", str);
  
  /* process string */
  printf("result : ");
  if (!process_str(str))
	printf("invalid format\n");
}

