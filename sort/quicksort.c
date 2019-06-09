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

#define swap(a , b)  { int  _temp = a; \
        a = b; \
        b = _temp; }

int partition (int a[], int lpos, int rpos)
{
    int pivot = a[lpos];
    int i = lpos;
    int j = rpos+1;

    while (i < j) {
        do {
            i++;
        } while ((a[i] <= pivot) && (i <= rpos));
//        } while ((a[i] <= pivot));
        do {
            j--;
        } while (a[j] > pivot); // not required because atleast the pivot should bail this out.
        if (i < j) {
            swap(a[i], a[j]);
        }
    }
    swap(a[lpos], a[j]);
    return j;
}

void quicksort (int a[], int lpos, int rpos)
{
    int pivot;
    int i,j;
    if (lpos < rpos) {
        j = partition(a, lpos, rpos);
        quicksort(a, lpos, j);
        quicksort(a, j+1, rpos);
    }
}

void main ()
{
    print(a_, "Insertion Sort \nBefore Sort", 10);
    quicksort(a_,0, 9);
    print(a_, "After sort", 10);
}

