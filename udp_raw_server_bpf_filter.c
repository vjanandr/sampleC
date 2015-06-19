#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <linux/filter.h>
#include <memory.h>
#include <net/ethernet.h>
#include <net/if.h>

int main ()
{
    int i = 0;
    int sock;
    int tmp = 1;
    char buffer[256];
    struct ipv6_mreq mreq;
    struct addrinfo hint = {0};
    struct addrinfo *multicastAddr;
    struct sockaddr_in6 serv_addr = {0};
    char ipv6_addr_str[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *multi_addr = NULL;
    int no_of_bytes_received;
    struct sockaddr_storage sender = {0};
    socklen_t sendsize = sizeof(sender);
    struct ifreq ifr;
    //tcpdump ip6 and udp and src port 4936 and dst port 4936 -dd
    struct sock_filter code[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 7, 0x000086dd },
        { 0x30, 0, 0, 0x00000014 },
        { 0x15, 0, 5, 0x00000011 },
        { 0x28, 0, 0, 0x00000036 },
        { 0x15, 0, 3, 0x00001348 },
        { 0x28, 0, 0, 0x00000038 },
        { 0x15, 0, 1, 0x00001348 },
        { 0x6, 0, 0, 0x0000ffff },
        { 0x6, 0, 0, 0x00000000 },
    };
    struct sock_fprog bpf = {
        .len = sizeof(code)/sizeof(code[0]),
        .filter = code,
    };


    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth1");
    sock = socket(AF_INET6, SOCK_RAW, IPPROTO_UDP);
//    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    printf("\nSocket created is %d", sock);
    if (sock < 0) {
        return -1;
    }

    hint.ai_family = AF_INET6;
    hint.ai_socktype = SOCK_DGRAM;
    hint.ai_protocol = IPPROTO_UDP;
    hint.ai_flags |= AI_NUMERICHOST;

    getaddrinfo("ff02::0150", "4936", &hint, &multicastAddr);

    if (multicastAddr->ai_addr->sa_family == AF_INET6) {
        multi_addr = (struct sockaddr_in6 *)multicastAddr->ai_addr;
        printf("\nAddress family IPV6");
    }

    if (multi_addr) {
        printf("\nAddress %s", inet_ntop(AF_INET6, &multi_addr->sin6_addr,
                                         ipv6_addr_str, INET_ADDRSTRLEN)); 
    }
    mreq.ipv6mr_multiaddr = multi_addr->sin6_addr;
    mreq.ipv6mr_interface = 0;

    if (setsockopt(sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&mreq,
                    sizeof(mreq)) != 0) {
        printf("\nFailed to join group");
        return -1;
    }

    if (setsockopt(sock, IPPROTO_IPV6, IP_HDRINCL, &tmp, sizeof(tmp)) < 0 ) {
        printf("\n Failed to set IP_HDRINCL options");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0) {
        perror("\n Failed to attach filter");
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_LOCK_FILTER, &bpf, sizeof(bpf)) < 0) {
        perror("\n Failed to lock filter");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, 
                (void *)&ifr, sizeof(ifr)) < 0) {
        perror("\nFailed to bind to interface ");
        return -1;
    }

    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(4936);

    if (bind(sock, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) != 0) {
        printf("\n Failed to bind");
        return -1;
    }

    freeaddrinfo(multicastAddr);
    while (1) {
        i = 0;
        memset(buffer, 0 , 255);
        no_of_bytes_received = recvfrom(sock, buffer, 255, 0,
                (struct sockaddr*)&sender, &sendsize);
        printf("\nNumber of bytes received %d", no_of_bytes_received);
        printf("\nFrom addr size %d", sendsize);
        if (sendsize) {
            struct sockaddr *from;
            from = (struct sockaddr *)&sender;
            char port[10];

            memset(ipv6_addr_str, 0, INET_ADDRSTRLEN);
            if (from->sa_family == AF_INET6) {
                struct sockaddr_in6 *s = (struct sockaddr_in6 *)from;
                inet_ntop(AF_INET6, &s->sin6_addr, ipv6_addr_str,
                          INET6_ADDRSTRLEN);
                getnameinfo((struct sockaddr *)&sender, sendsize, ipv6_addr_str,
                        INET6_ADDRSTRLEN, port, 10, NI_NUMERICHOST);
                printf("\nSrc IP %s, Src Port %s(%d) In Index %d", 
                        ipv6_addr_str,
                        port, ntohs(s->sin6_port), s->sin6_scope_id);
            }
        }
        printf("\nData received - \"");
        while(no_of_bytes_received--) {
            printf("%c",buffer[i++]);
        }
        printf("\"\n");
    }
    return 0;
}
