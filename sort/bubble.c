#include<stdio.h>
int a_[10] = {54,81,37,44,29,47,59,83,51,45};
void print (int a[], char *str, int size)
{
    int i = 0;
    printf("\n%s\n", str);
    while (i<size) {
        printf("%d ",a[i]);
        i++;
    }
    printf("\n");
}
int main ()
{
    int i,j, tmp;
    print(a_,"Before sort", 10);
    for (i=0; i < 10-1; i++){
        for (j=0; j<10-1-i;j++){
            if (a_[j] > a_[j+1]) {
                tmp = a_[j];
                a_[j] = a_[j+1];
                a_[j+1] = tmp;
            }
        }
    }
    print(a_,"After sort", 10);
}
