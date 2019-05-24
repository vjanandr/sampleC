#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#define MSG_SIZE 16
char *msg1 = "Hello World 1";
char *msg2 = "Hello world 2";
char *msg3 = "Hello World 3";


int main ()
{
    char inbuf [MSG_SIZE];
    int p[2],i;

    pipe(p);
    if (fork() == 0 ){
        close(p[0]);
        printf("\n started writing in child process");
        write(p[1], msg1, MSG_SIZE);
        sleep(10);
        write(p[1], msg2, MSG_SIZE);
        write(p[1], msg3, MSG_SIZE);
        printf("\n Writing done in child process");
        close(p[1]);
    } else {
        close(p[1]);
        printf("\n started reading in parent process");
        
        for (i= 0;i<3;i++) {
            read(p[0],inbuf, MSG_SIZE);
            printf("%s \n",inbuf);
        }
        wait(NULL);
        close(p[0]);
        printf("\n reading done in parent process");
    }
}

