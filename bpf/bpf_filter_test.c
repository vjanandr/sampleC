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
    // This is for AF_PACKET.
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
        .len = 10,
        .filter = code,
    };
#if 0
    /* 
     * For IPPROTO_UDP, assuming the skb->data is from network start
     * Just check if the IP version is 6.
     *
     * step0:ldb [0]
     * step1:jeq #0x60, step2, step3
     * step2:ret #65535
     * step3:ret #0
     */

    struct sock_filter code[] = {
        { 0x30, 0, 0, 0x00000000 },
        { 0x15, 0, 1, 0x00000060 },
        { 0x6, 0, 0, 0x0000ffff },
        { 0x6, 0, 0, 0x00000000 },
    };
    struct sock_fprog bpf = {
        .len = 4,
        .filter = code,
    };

#endif 
    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth2");
//    sock = socket(AF_INET6, SOCK_RAW, IPPROTO_UDP);
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    printf("\nSocket created is %d", sock);
    if (sock < 0) {
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, 
                (void *)&ifr, sizeof(ifr)) < 0) {
        printf("\nbind to interface failed");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0) {
        perror("\n Failed to attach filter");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_LOCK_FILTER, &bpf, sizeof(bpf)) < 0) {
        perror("\n Failed to lock filter");
        return -1;
    }
    

    while (1) {
        i = 0;
        memset(buffer, 0 , 255);
        no_of_bytes_received = recv(sock, buffer, 255, 0);
        printf("\nData received(%d) - \"", no_of_bytes_received);
        while(no_of_bytes_received--) {
            printf(" %x",buffer[i++]);
        }
        printf("\"\n");
    }
    return 0;
}
