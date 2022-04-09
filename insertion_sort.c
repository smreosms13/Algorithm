#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int RANGE = 100;
int main() {
	int arr[9];
	int len_arr = sizeof(arr)/sizeof(int);
	srand(time(NULL));
	printf("unsorted\n");
	for(int i=0 ; i<len_arr ; i++){
		arr[i] = rand() % RANGE +1;
		printf("%d" ,arr[i]);
		printf("\n");
	}
	int i,j,x;
	for(i=1; i<len_arr; i++){
		x = arr[i];
		j=i-1;
		while(arr[j]>x && j>=0){
			arr[j+1] = arr[j];
			j--;
		}
		arr[++j] = x;
	}
	printf("sorted\n");
	for (int i = 0 ; i<len_arr; i++){
		printf("%d" ,arr[i]);
		printf("\n");
	}		
}
	