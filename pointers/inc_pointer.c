#include <stdio.h>
int a[10] = {1,20,30,40,50,60,70,80,90,100};
int main ()
{
    int *p  = a;
    int tmp;
    int *tmp2;
    tmp = *p++;
    printf("\n p = %d tmp = %d",*p, tmp);
    tmp = *++p;
    printf("\n p = %d tmp = %d",*p,tmp);
    tmp = (*p)++;
    printf("\n p = %d tmp = %d",*p,tmp);

    tmp2 = p++;
    printf("\n p = %d tmp2 = %d",*p,*tmp2);

}
