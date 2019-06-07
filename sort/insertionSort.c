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
void insertionSort (int a[], int count)
{
    int i = 1, j=0, temp;
    for (i = 1; i < count; i++) {
        temp = a[i];
        for (j = i-1; j >= 0; j--) {
            if (a[j] > temp) {
                a[j+1] = a[j];
            } else {
                break;
            }
        }
        a[j+1] = temp;
    }
}
void main ()
{
    print(a_, "Insertion Sort \nBefore Sort", 10);
    insertionSort(a_,10);
    print(a_, "After sort", 10);
}

