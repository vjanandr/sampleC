#include <stdio.h>

void printrightbit (int a)
{

    if (!a) {
        return;
    }
    printrightbit(a>>1);
    printf("%d",a%2);
}
int main ()
{
    printrightbit(7);
}
