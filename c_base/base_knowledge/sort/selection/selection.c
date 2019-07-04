#include <stdio.h>

static void selection(int *arr, int len)
{
	int i, j, min;
	int tmp;

	for(i = 0; i < len; i++) {
		min = i; //1. 假定最小元素的下标为min等于i
		for(j = i+1; j < len; j++) {
			//2. 在第二个for循环里，从第i+1个元素遍历剩余所有元素，找出最小元素的下标j，并更新min
			if(*(arr+j) < *(arr+min))
				min = j;
		}
		//3. 交换下标为i和min的两个元素的值
		tmp = *(arr+i);
		*(arr+i) = *(arr+min);
		*(arr+min) = tmp;
	}
}

int main(int argc, char const *argv[])
{
	int arr[] = {1, 66, 23, 21, 20, 9, 41, 3, 10, 37, 99};
	int len = sizeof(arr)/sizeof(arr[0]);
	int i;

	for(i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n");

	selection(arr, len);

	for(i = 0; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n");

	return 0;
}

