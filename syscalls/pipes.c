#include <stdio.h>
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
    write(p[1], msg1, MSG_SIZE);
    write(p[1], msg2, MSG_SIZE);
    write(p[1], msg3, MSG_SIZE);

    for (i= 0;i<3;i++) {
        read(p[0],inbuf, MSG_SIZE);
        printf("%s \n",inbuf);
    }
}

