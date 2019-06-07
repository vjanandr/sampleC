#include <stdio.h>
int a_[10] = {54,81,37,44,29,47,59,83,51,45};
int temp_[10];

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
void merge (int a[], int temp[], int lpos, 
        int mpos, int rpos)
{
    int i=0, j=0, k=lpos;
    i = lpos;
    j = mpos;

    while ((i < mpos) &&  (j <= rpos)) {
        if (a[i] < a[j]) {
            temp[k++]= a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }
    while (i < mpos) {
        temp[k++] = a[i++];
    }
    while (j <= rpos) {
        temp[k++] = a[j++];
    }
    //k--;
    k = rpos;
    while (k >= lpos) {
        a[k] = temp[k];
        k--;
    }
    //print(a, "merge",10);
}
void mergeSort (int a[], int temp[], int lpos, int rpos)
{
    int mid = (lpos+rpos)/2;
    if (rpos > lpos) {
        mergeSort(a, temp, lpos, mid);
        mergeSort(a, temp, mid+1, rpos);
        merge(a,temp, lpos, mid+1, rpos);
    }
}

void main ()
{
    print(a_, "Insertion Sort \nBefore Sort", 10);
    mergeSort(a_, temp_, 0,9);
    print(a_, "After sort", 10);
}

