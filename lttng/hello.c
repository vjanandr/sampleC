#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "hello-tp.h"
#include <lttng/tracef.h>

int main(int argc, char *argv[])
{
    int x;
    pid_t pid;


 //   puts("Hello, World!\nPress Enter to continue...");

    /*
     * The following getchar() call is only placed here for the purpose
     * of this demonstration, for pausing the application in order for
     * you to have time to list its events. It's not needed otherwise.
     */
//    getchar();

    /*
     * A tracepoint() call. Arguments, as defined in hello-tp.h:
     *
     *     1st: provider name (always)
     *     2nd: tracepoint name (always)
     *     3rd: my_integer_arg (first user-defined argument)
     *     4th: my_string_arg (second user-defined argument)
     *
     * Notice the provider and tracepoint names are NOT strings;
     * they are in fact parts of variables created by macros in
     * hello-tp.h.
     */
    pid = getpid();
    printf("\n Process Id %d",(int)pid);
    tracepoint(hello_world, my_first_tracepoint, (int)pid, "Process ID:");
    for (x = 0; x < 50; ++x) {
        tracepoint(hello_world, my_first_tracepoint, (int)pid, "pid:");
        tracepoint(hello_world, my_second_tracepoint, x, "x^2");
        sleep(1);
    }

  //  puts("Quitting now!");

    tracepoint(hello_world, my_third_tracepoint, x,
               "Checkout the third tracepoint");
    tracef("my message, my integer: %d", 20);

   // puts("Press enter again");
   // getchar();
    printf("\n Process End Id %d", (int)pid);

    return 0;
}
