#include<stdio.h>

unsigned int setbits (int x , int p, int n, int y)
{
   unsigned int npos_bits = ((~(~0 << n)) << (p-n)); // n 1 bits at p position
   unsigned int ny_bits = (y & (~(~0 << n))) << (p-n);
   x &= ~npos_bits;
   x |= ny_bits;
   return x;
}
void printrightbit (int a)
{
    if (!a)
        return;
    printrightbit(a>>1);
    printf("%d",a%2);
}
int main()
{
    printrightbit(0);
    printf("\n");
    printrightbit(0xfffe);
    printf("\n");
    printrightbit(setbits(0, 5, 2, 0xfffe));
    printf("\n");
}
