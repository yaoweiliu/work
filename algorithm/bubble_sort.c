/*数组作为函数的形参时会被退化为指针，此时不能用sizeof()来取得数组大小，只能直接传入数组大小*/

#include <stdio.h>

void bubble_sort(int len, int array[])
{
    int i, j, tmp;
    int flag;

    for(i = 0; i < len; i++) {
        flag = 1;
        for(j = 0; j < len - i - 1; j++) {
            if(array[j] > array[j+1]) {
                tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
                flag = 0;
            }
        }
        if(flag)
            break;
    }

    for(i = 0; i < len; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main(void)
{
    int lt[] = {5,3,1,8,0,98,2};
    int len = sizeof(lt)/sizeof(lt[0]);

    bubble_sort(len, lt);
    
    return 0;
}
