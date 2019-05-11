#include<stdio.h>
void printrightbit (int val)
{
    if (!val)
        return;
    printrightbit(val >> 1);
    printf("%d", val%2);
}
int main()
{
    int count = 0;
    int x = 10;
    int val = x;
    while(x) {
        count++;
        x &= (x-1);
    }
    printf("\nnumber of bits in %d is %d \n", x, count);
    printrightbit(val);
    printf("\n");
}
