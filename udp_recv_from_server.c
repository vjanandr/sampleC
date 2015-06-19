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
   char addr_str[INET6_ADDRSTRLEN];

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
       len = sizeof(struct sockaddr_storage);
       printf("\n len before %d", len);
       status = recvfrom(sock, buffer, MAXBUF, 0, 
                         (struct sockaddr *)&sin_store, &len);
       printf("\n len after %d sizeof(sockaddr_in6) %d", len, 
               (int)sizeof(struct sockaddr_in6));
       sin6frm = (struct sockaddr_in6 *)&sin_store;
       printf("\nbuffer : %s", buffer);
       printf("\nfrom %s",inet_ntop(AF_INET6, &sin6frm->sin6_addr, addr_str,
                   INET6_ADDRSTRLEN));
       getnameinfo((struct sockaddr *)&sin_store, len, addr_str,
                 INET6_ADDRSTRLEN, port, sizeof(port),NI_NUMERICHOST);
         printf("\nfrom %s",addr_str);
       printf("\n");
   }

   shutdown(sock, 2);
   close(sock);
   return 0;
}
