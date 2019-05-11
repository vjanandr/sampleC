#include <stdio.h>
int a_[10] = {54,81,37,44,29,47,59,83,51,45};

void selectionSort(int a[], int size)
{
    int i, minindex, tmp;
    for (i=0; i < size; i++) {
        minindex = i;
        for (int j = i+1; j < size; j++) {
            if (a[minindex] > a [j]) {
                minindex = j;
            }
        }
        tmp = a[i];
        a[i] = a[minindex];
        a[minindex] = tmp;
    }
}
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
void main ()
{
    print(a_, "Before Sort", 10);
    selectionSort(a_,10);
    print(a_, "After sort", 10);
}

