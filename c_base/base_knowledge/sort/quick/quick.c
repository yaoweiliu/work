#include <stdio.h>

static void quick(int *arr, int left, int right)
{

}

int main(int argc, char const *argv[])
{
	int arr[] = {1, 66, 23, 21, 20, 9, 41, 3, 10, 37, 99};
	int len = sizeof(arr)/sizeof(arr[0]);
	int i;

	for(i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n\n");

	quick(arr, 0, len-1);

	for(i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n\n");
	return 0;
}