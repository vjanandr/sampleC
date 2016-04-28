#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>
/* For constants ORIG_EAX etc */
int main()
{   pid_t child;
    long orig_eax;
    child = fork();
    if(child == 0) {
        printf("\n Child process ptrace \n");
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        printf("\n Child process going to ls \n");
        execl("/bin/ls", "ls", NULL);
        printf("\n Child process after ls \n");
    }
    else {
        printf("\n parent process going to `wait \n");
        wait(NULL);
        printf("\n Parent process after waiting \n");
        orig_eax = ptrace(PTRACE_PEEKUSER,
                          child, 4 * ORIG_EAX,
                          NULL);
        printf(" \nThe child made a "
               "system call %ld\n", orig_eax);
        ptrace(PTRACE_CONT, child, NULL, NULL);
    }
    return 0;
}
