#include <stdio.h>

int main(void)
{
    int i, j, tmp;
    int lt[] = {5,3,1,8,0,98,2};

    for(i = 0; i < 7; i++) {
        for(j = 0; j < 7 - i - 1; j++) {
            if(lt[j + 1] > lt[j]) {
                tmp = lt[j + 1];
                lt[j + 1] = lt[j];
                lt[j] = tmp;
            }
        }
    }

    for(i = 0; i < 7; i++)
        printf("%d ", lt[i]);
    printf("\n");
    
    return 0;
}
