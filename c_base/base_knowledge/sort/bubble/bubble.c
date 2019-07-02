#include <stdio.h>

static void bubble(int *arr, int len)
{
	int i, j, tmp;

	for(i = 0; i < len-1; i++) {
		for(j = 0; j < len-1-i; j++) {
			if(*(arr+j+1) > *(arr+j)) {
				tmp = *(arr+j+1);
				*(arr+j+1) = *(arr+j);
				*(arr+j) = tmp;
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	int arr[] = {1, 67, 4, 98, 45, 30, 21};
	int i, len;

	len = sizeof(arr)/sizeof(arr[0]);
	for(i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n\n");

	bubble(arr, len);
	for(i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n\n");

	return 0;
}


