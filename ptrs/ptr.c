#include <stdio.h>
int main ()
{
    int a[10] = {1,2 ,3,4,5,6,7,8,9,10};
    int *ptr1 = a;
    int (*ptr2)[10] = a;
    printf("\n sizeof ptr1 %d", sizeof(ptr1));
    printf("\n sizeof ptr2 %d", sizeof(ptr2));
    printf("\n sizeof a %d", sizeof(a));
}
