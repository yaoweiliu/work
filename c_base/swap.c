#include <stdio.h>

#define swap(a, b) \
    do {typeof(a) __tmp; (__tmp) = (a); (a) = (b); (b) = (__tmp);}while(0)

int main(void)
{
    int a = 5, b = 7;
    
    printf("a = %d, b = %d\n", a, b);
    
    swap(a, b);

    printf("a = %d, b = %d\n", a, b);
    return 0;
}
