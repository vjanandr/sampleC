/*
 * Route Netlink Sock utility APIs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <signal.h>
#include <errno.h>
#include "rt_netlink_utils.h"

typedef struct rt_nl_req_t_ {
    struct nlmsghdr nl_hdr;
    struct rtgenmsg gen_msg;
} rt_nl_req_t;

bool rtnetlink_sock_new (rt_netlink_sock_t **rt_nt_sock)
{
    *rt_nt_sock = malloc(sizeof(rt_netlink_sock_t));

    if (!*rt_nt_sock) {
        return false;
    }
    memset(*rt_nt_sock, 0, sizeof(rt_netlink_sock_t));

    return true;
}

bool
rtnetlink_sock_free (rt_netlink_sock_t **rt_nt_sock)
{
    free(*rt_nt_sock);
    *rt_nt_sock = NULL;
    return true;
}

bool rtnetlink_sock_create_bind (rt_netlink_sock_t *rt_nt_sock, int grp)
{
    int fd;
    int pid;
    struct sockaddr_nl local_nl_addr;

    if (!rt_nt_sock) {
        printf("\nError Null pointer fd");
        return false;
    }

    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

    memset(&local_nl_addr, 0, sizeof(local_nl_addr));
    pid = getpid();

    local_nl_addr.nl_family = AF_NETLINK;
    local_nl_addr.nl_pid= pid;
    local_nl_addr.nl_groups = grp;

    if (bind(fd, (struct sockaddr *) &local_nl_addr,
                sizeof(local_nl_addr)) < 0) {
        close(fd);
        return false;
    }

    rt_nt_sock->fd = fd;
    rt_nt_sock->pid = pid;
    rt_nt_sock->seq_no = 0;

    return true;
}

bool rt_netlink_sock_close (rt_netlink_sock_t *rt_nt_sock)
{
    rt_nt_sock->seq_no = 0;
    close(rt_nt_sock->fd);
    return true;
}

int rt_netlink_sock_fd_get (rt_netlink_sock_t *rt_nt_sock)
{
    return (rt_nt_sock->fd);
}

bool
rtnetlink_send_request (rt_netlink_sock_t *rt_nt_sock, 
                        int nlmsg_type,
                        int req_family, 
                        int flags)
{
    struct msghdr msg = {0};
    struct iovec io = {0};
    struct sockaddr_nl kernel_nl_addr = {0};
    rt_nl_req_t nl_req = {0};

    kernel_nl_addr.nl_family = AF_NETLINK;

    nl_req.nl_hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
    nl_req.nl_hdr.nlmsg_type = nlmsg_type;
    nl_req.nl_hdr.nlmsg_flags = flags;
    nl_req.gen_msg.rtgen_family = req_family;

    io.iov_base = &nl_req;
    io.iov_len = nl_req.nl_hdr.nlmsg_len;

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    msg.msg_name = &kernel_nl_addr;
    msg.msg_namelen = sizeof(kernel_nl_addr);

    if (sendmsg(rt_nt_sock->fd, (struct msghdr *) &msg, 0) < 0) {
        printf("\nFailed to send the message");
        return false;
    }
    return true;
}

#define MSG_BUF_SIZE 8192

bool rtnetlink_msg_buf_new (rt_nl_msg_buf_t **msg_buf_)
{
    rt_nl_msg_buf_t *msg_buf;

    if (!msg_buf_) {
        return false;
    }

    msg_buf = malloc(sizeof(rt_nl_msg_buf_t));
    if (!msg_buf) {
        return false;
    }

    msg_buf->msg = malloc(MSG_BUF_SIZE);
    msg_buf->msg_buf_length = MSG_BUF_SIZE;
    msg_buf->data_length = 0;

    *msg_buf_ = msg_buf;
    return true;
}

bool rtnetlink_msg_buf_free (rt_nl_msg_buf_t **msg_buf_) 
{
    rt_nl_msg_buf_t *msg_buf;
    if (!msg_buf_) {
        return false;

    }
    msg_buf = *msg_buf_;
    free(msg_buf->msg);
    free(msg_buf);
    *msg_buf_ = NULL;
    return true;
}

int rtnetlink_msg_buf_get_length (rt_nl_msg_buf_t *msg_buf)
{
    return (msg_buf->msg_buf_length);
}

struct nlmsghdr* rtnetlink_msg_buf_get_data (rt_nl_msg_buf_t *msg_buf)
{
    return (msg_buf->msg);
}

bool 
rtnetlink_recv_msg (rt_netlink_sock_t *rt_nl_sock,
                    rt_nl_msg_buf_t *msg_buf)
{
    struct nlmsghdr *nl_msg_resp_hdr;
    struct msghdr msg;
    struct iovec io_reply;
    struct sockaddr_nl kernel_nl_addr = {0};

    kernel_nl_addr.nl_family = AF_NETLINK;

    if (!msg_buf || !msg_buf->msg) {
        return false;
    }

    memset(&io_reply, 0, sizeof(io_reply));
    memset(&msg, 0, sizeof(msg));

    io_reply.iov_base = msg_buf->msg;
    io_reply.iov_len = msg_buf->msg_buf_length;

    msg.msg_iov = &io_reply;
    msg.msg_iovlen = 1;
    msg.msg_name = &kernel_nl_addr;
    msg.msg_namelen = sizeof(kernel_nl_addr);
    msg_buf->data_length = recvmsg(rt_nl_sock->fd, &msg, 0);

    nl_msg_resp_hdr = (struct nlmsghdr *)msg_buf->msg;

    if (!NLMSG_OK(nl_msg_resp_hdr, msg_buf->data_length)) {
        return false;
    }

    return true;
}

void rtnetlink_parse_link_info (struct nlmsghdr *h)
{
    struct ifinfomsg *iface;
    struct rtattr *attribute;
    struct rtnl_link_stats *stats;
    char name[IFNAMSIZ];
    int len;

    iface = NLMSG_DATA(h);
    len = h->nlmsg_len - NLMSG_LENGTH(sizeof(*iface));

    printf("\n in rtnetlink_parse_link_info");

    /* loop over all attributes for the NEWLINK message */
    for (attribute = IFLA_RTA(iface); 
         RTA_OK(attribute, len); 
         attribute = RTA_NEXT(attribute, len)) {

        switch(attribute->rta_type) {
            case IFLA_IFNAME:
                strcpy(name, (char *) RTA_DATA(attribute));
                printf("\n Interface name %s ifIndex %d", name,iface->ifi_index);
                printf("\t state: %s", iface->ifi_flags & IFF_UP ? "UP":"DOWN");
                printf("\t Loopback: %s", iface->ifi_flags & IFF_LOOPBACK ?
                        "YES" : "NO");
                break;
        }
    }
    fflush(stdout);
}
