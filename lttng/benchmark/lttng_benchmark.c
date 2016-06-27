#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "tp_0.h"
#include <lttng/tracef.h>

struct timeval start_time;
struct timeval end_time;

void
calculate_time_diff(char *ctxt)
{
    printf("Time to log using %10s with 5 arguments is: %ld microseconds\n",
            ctxt,
            ((end_time.tv_sec * 1000000 + end_time.tv_usec)
            - (start_time.tv_sec * 1000000 + start_time.tv_usec)));
}

void
lttng_baseline (void)
{
    char str1[6], str2[6];
    char str3[6];
    char str4[6];
    char str5[6];

    strcpy(str1, "test1");
    strcpy(str2, "test2");
    strcpy(str3, "test3");
    strcpy(str4, "test4");
    strcpy(str5, "test5");
    gettimeofday(&start_time, NULL);
    for (int i=0; i<1000000; i++) {
        tracepoint(tp_0, my_second_tracepoint, str1, str2, str3, str4, str5);
    }
    gettimeofday(&end_time, NULL);
    calculate_time_diff("lttng");
}

int main ()
{
    lttng_baseline();
    return 0;
}
