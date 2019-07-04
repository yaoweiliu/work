#include <stdio.h>

static void bubble(int *arr, int len)
{
	int i, j;
	int flag, tmp;

	for(i = 0; i < len; i++) {
		flag = 1;
		for(j = 0; j < len-i; j++) {
			if(*(arr+j) > *(arr+j-1)) {
				tmp = *(arr+j);
				*(arr+j) = *(arr+j-1);
				*(arr+j-1) = tmp;

				flag = 0;
			}
		}
		if(flag)
			break;
	}
}

int main(int argc, char const *argv[])
{
	int arr[] = {1, 66, 24, 20, 21, 9, 3, 41};
	int len = sizeof(arr)/sizeof(arr[0]);
	int i;

	for(i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
		printf("%d ", arr[i]);
	printf("\n\n");

	bubble(arr, len);

	for(i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
		printf("%d ", arr[i]);
	printf("\n\n");

	return 0;
}


