#include <stdio.h>
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

void countingSort (int a[], int n, int r)
{
    int t[83];
    int c[10];
    int i = 0;
    while (i < 83) {
        t[i++] = 0;
    }
    i=0;
    while (i < n) {
        t[a[i]] = t[a[i]] + 1;
        i++;
    }
    for (i = 0; i < n; i++) {
        c[t[a[i]] - 1] = a[i];
        t[a[i]] = t[a[i]] +1;
    }
    for (i = 0; i<n;i++) {
        a[i] = c[i];
    }
}

void main ()
{
    print(a_, "Insertion Sort \nBefore Sort", 10);
    countingSort(a_,10,83);
    print(a_, "After sort", 10);
}

