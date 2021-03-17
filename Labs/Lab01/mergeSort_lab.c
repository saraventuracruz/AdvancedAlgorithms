#include <stdio.h>
#include <assert.h>

#define N 6

int A[N] = {7, 2, 5, 4, 8, 1};
int aux[N];

void merge(int i, int k, int j)
{
  int jj; /* iterate over the second list */
  int l;
  for(l = i; l <=j; l++){
    aux[l] = A[l];
  }

  l = i;
  jj = k+1;
  while( i <= k || jj <= j ){
    if(i > k) { /* First list over */
      A[l] = aux[jj];
      jj++;
    } else {
      if(jj > j) { /* Second list over */
	A[l] = aux[i];
	i++;
      } else { /* Both lists not empty */
	if(aux[i] <= aux[jj]){
	A[l] = aux[i];
	i++;
	} else {
	  A[l] = aux[jj];
	  jj++;
	}
      }
    }
    /* l++; */
  }
}

void mergeSort(int i, int j)
{
  int k;

  if(i != j){
    printf("%d %d\n", i, j);

    k = (i + j) / 2; /* Middle */

    mergeSort(i, k);
    mergeSort(k+1, j);
    merge(i, k, j); /* Linear time */
  }

  printf("%d %d\n", i, j);
}

int main()
{
  int i;

  /* printf("%d\n", A[1<<20]); */ 

  for(i = 0; i < N; i++){
    printf("%d, ", A[i]);
  }
  printf("\n");

  mergeSort(0, N-1);

   for(i = 0; i+1 < N; i++){ 
     assert(A[i] <= A[i+1]); 
   } 

  for(i = 0; i < N; i++){
    printf("%d, ", A[i]);
  }
  printf("\n");

  return 0;
}
