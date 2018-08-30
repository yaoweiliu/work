/*
 * p未分配空间，为野指针，不能赋值，否则段错误！
 * 应该先分配内存空间，malloc()
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    /* A
    int *p = 0;
    *p = 10;
    */
    int *p = NULL;
    p = malloc(sizeof(p));
    *p = 10;
    printf("*p = %d\n", *p);
    printf("p = %p\n", p);
    
    free(p);

    return 0;
}
