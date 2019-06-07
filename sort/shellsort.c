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
#if 0
void shellSort (int a[], int count)
{
    int i = 1, j=0, temp;
    int gap = count/2;
    for (; gap > 0 ; gap /=2) {
        for (i = 0; i < gap; i++) {
            temp = a[i];
            for (j = i+gap; j < count; j+= gap) {
                if (a[j] < temp) {
                    a[j-gap] = a[j];
                } else {
                    break;
                }
            }
            a[j] = temp;
        }
    }
}
#endif

int shellSort(int arr[], int n) 
{ 
    // Start with a big gap, then reduce the gap 
    for (int gap = n/2; gap > 0; gap /= 2) 
    { 
        // Do a gapped insertion sort for this gap size. 
        // The first gap elements a[0..gap-1] are already in gapped order 
        // keep adding one more element until the entire array is 
        // gap sorted  
        for (int i = gap; i < n; i += 1) 
        { 
            // add a[i] to the elements that have been gap sorted 
            // save a[i] in temp and make a hole at position i 
            int temp = arr[i]; 
  
            // shift earlier gap-sorted elements up until the correct  
            // location for a[i] is found 
            int j;             
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) 
                arr[j] = arr[j - gap]; 
              
            //  put temp (the original a[i]) in its correct location 
            arr[j] = temp; 
        } 
    } 
    return 0; 
}

void main ()
{
    print(a_, "Insertion Sort \nBefore Sort", 10);
    shellSort(a_,10);
    print(a_, "After sort", 10);
}

