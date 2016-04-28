#include <config.h>
#include <whine.h>
#include <stdio.h>
int main ()
{
    printf("\n Hello world \n");
#ifdef DEBUG
    printf("\n Debug enabled \n");
#endif
    whine();
    return 0;
}
