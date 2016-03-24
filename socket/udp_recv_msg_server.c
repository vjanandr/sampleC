#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXBUF 65536

int main()
{
   int sock;
   int status;
   struct sockaddr_storage sin_store;
   struct sockaddr_in6 sin6;
   int len;
   char buffer[MAXBUF];
   char addr_str[INET_ADDRSTRLEN];
   struct iovec iov[1];

   sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);


   memset(&sin_store, 0, len);

   /* just use the first address returned in the structure */

   sin6.sin6_port = htons(4936);
   sin6.sin6_family = AF_INET6;
   sin6.sin6_addr = in6addr_any;

   status = bind(sock, (struct sockaddr *)&sin6, sizeof(struct sockaddr_in6));
   if(-1 == status)
     perror("bind"), exit(1);

   len = sizeof(struct sockaddr_in6);
   status = getsockname(sock, (struct sockaddr *)&sin6, &len);

   printf("%d\n", ntohs(sin6.sin6_port));
   len = sizeof(struct sockaddr_storage);
   while (1) {
       struct sockaddr_in6 *sin6frm;
       char port[10];
       int byte_cnt;
       struct msghdr message; 

       iov[0].iov_base = buffer;
       iov[0].iov_len = MAXBUF;
       message.msg_name = &sin_store;
       message.msg_namelen = sizeof(struct sockaddr_storage);
       message.msg_iov=iov;
       message.msg_iovlen=1;
       message.msg_control=0;
       message.msg_controllen=0;

       status = recvmsg(sock, &message,0);
       printf("\nbuffer : %s", buffer);
       sin6frm = (struct sockaddr_in6 *)&sin_store;
       while (byte_cnt < 16) {
           uint8_t byte = sin6frm->sin6_addr.__in6_u.__u6_addr8[byte_cnt];
           printf("\n byte[%d] %d", byte_cnt, byte);
           byte_cnt++;
       }
       printf("\n");
   }

   shutdown(sock, 2);
   close(sock);
   return 0;
}
