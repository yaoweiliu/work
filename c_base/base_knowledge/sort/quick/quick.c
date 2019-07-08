#include <stdio.h>

static void quick(int *arr, int left, int right)
{
	int lp, rp;
	int key;
	int tmp;

	if(left > right)
		return ;

	key = *(arr+left); //基准值
	lp = left; //左指针
	rp = right; //右指针

	while(lp < rp) {
		while(*(arr+rp) >= key && lp < rp) //先从右边找
			rp--;
		while(*(arr+lp) <= key && lp < rp) //再从左边找
			lp++;
		//交换左右指针的值
		tmp = *(arr+lp);
		*(arr+lp) = *(arr+rp);
		*(arr+rp) = tmp;
	}

	//更新基准值
	tmp = *(arr+left);
	*(arr+left) = *(arr+lp);
	*(arr+lp) = tmp;

	quick(arr, left, lp-1); //递归处理左区间,lp在++
	quick(arr, rp+1, right); //递归处理右区间,rp在--

	return ;
}

int main(int argc, char const *argv[])
{
	int arr[] = {1, 66, 23, 21, 20, 69, 9, 41, 3, 10, 37, 99};
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