#include <stdio.h>

#define N 6 /*preprocessing instruction*/

int ListOfNumbers[] = {7,2,5,4,8,2,1};
int aux[N];

void mergeSort(int i, int j);
void merge(int i, int k, int j);

int main()
{
	int i;
	for(i = 0; i<N; i++){
		printf("%d, ", ListOfNumbers[i]);
	}

	printf("\n");

	/* sort the list by using merge sort*/
	mergeSort(0,N-1);


	return 0;
}

void mergeSort(int i, int j){

	int k;

	if(i != j){
		printf("%d %d\n", i, j);
	

		k = (i+j)/2;

		mergeSort(i,k);
		mergeSort(k+1,j);
		merge(i,k,j);
	}
	
}

void merge(int i , int k, int j){
	int jj; /* iterate over the second list*/
	int l;
	for(l = i; l<= j; l++){
		aux[l] = ListOfNumbers[l];
	}

	l = i;
	jj = k+1;
	while(i <= k || jj <= j){
		if(i > k){ /*FIrst list over */
			ListOfNumbers[l] = aux[jj];
			jj++;
		}else{
			if(jj>j){ /* Second list over*/
				ListOfNumbers[l] = aux[i];
				i++;
			} else{ /*Both lists not empty*/
				if(aux[i] <= aux[jj]){
					ListOfNumbers[l] = aux[i];
					i++;
				}else{
					ListOfNumbers[l] = aux[jj];
					jj++;
				}

			}
		}
		l++;
	}
}