#include <stdio.h> /*to write stuff*/
#include <stdlib.h> /*to use malloc and free*/

int main()
{
  /*char T[10];*/ /*an array of characters, but we don't know how much space will it take- for now we allocate 10 bytes*/

  char *T;
  int i;
  int c;

  int A = 2;

  T = malloc(A*sizeof(char)); /*give me a piece of memory*/
  i = 0;
  c = getchar();
  while(EOF != c){
    if(i == A){
      A = A*2;
      T = realloc(T, A*sizeof(char));
    }
    T[i] = (char) c;
    i++;

    c = getchar();
  }

  T[i] = 0; /*the numerical value and not the character '0' either = 0 or = '\0' two possible ways of writing the value*/

  printf("%s\n", T);

  free(T); /*free memory*/

  return 0;
}
