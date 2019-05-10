#include<stdio.h>
void
printrightbits (int a)
{
    if (!a)
        return;
    printrightbits(a>>1);
    printf("%d", a%2);
}

unsigned getbits (unsigned x , int p , int n)
{
    if (p < n)
        return 0;
    return ((x >> p-n+1) & ~(~0 << n ));
}
int main ()
{
    printrightbits(44);
    printf("\n");
    printrightbits(getbits(44, 5, 3));
}
