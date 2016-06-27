#include <isan/sdwrap_dsf.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "tp_0.h"
#include <lttng/tracef.h>
#include <isan/sdwrap_intf.h>
#include <curses.h> 

#define SRVUUID_CALLHOME 63
#define CALLHOME_TEST_TRACE_BINARY_LOG SDHIST_MKEVENT(SRVUUID_CALLHOME,0)


struct timeval start_time;
struct timeval end_time;

typedef struct temp_t {
        char a[6];
        char b[6];
        char c[6];
        char d[6];
        char e[6];
} temp;

void
calculate_time_diff(char *ctxt)
{
    printf("Time to log using %10s with 5 arguments is: %ld microseconds\n", ctxt, ((end_time.tv_sec * 1000000 + end_time.tv_usec)
                - (start_time.tv_sec * 1000000 + start_time.tv_usec)));
}

void sdwrap_baseline (void)
{
    temp test_bin;

    // gettimeofday and store it in a global
    gettimeofday(&start_time, NULL);

    for (int i=0; i<1000000; i++) {
        strcpy(test_bin.a, "test1");
        strcpy(test_bin.b, "test2");
        strcpy(test_bin.c, "test3");
        strcpy(test_bin.d, "test4");
        strcpy(test_bin.e, "test5");
 //       strcpy(test_bin.c, "test5");
        sdwrap_hist_event(CALLHOME_TEST_TRACE_BINARY_LOG, sizeof(test_bin), &test_bin);
    }

    // Get end time and calculate time diff from start_time
    gettimeofday(&end_time, NULL);
    calculate_time_diff("sdwrap");
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
//        strcpy(str3, "test5");
//        tracepoint(tp_0, my_second_tracepoint, str1, str2, str3);
        tracepoint(tp_0, my_second_tracepoint, str1, str2, str3, str4, str5);
    }
    gettimeofday(&end_time, NULL);
    calculate_time_diff("lttng");
}

int main ()
{
    printf("\nstring out side for loop");
    sdwrap_hist_init(1000000);
    sdwrap_baseline();
    sleep(30);
    lttng_baseline();
    return 0;
}
