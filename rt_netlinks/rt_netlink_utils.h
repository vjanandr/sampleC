/*
 * RT netlink Utility library.
 */
#include <stdbool.h>
typedef struct rt_netlink_sock_t_ {
    int fd;
    int pid;
    int seq_no;
} rt_netlink_sock_t;

extern bool rtnetlink_sock_new(rt_netlink_sock_t **rt_nt_sock);
extern bool rtnetlink_sock_create_bind(rt_netlink_sock_t *rt_nt_sock, int grp);
extern bool rtnetlink_send_request(rt_netlink_sock_t *rt_nt_sock,
        int nlmsg_type, int req_family, int flags);

typedef struct rt_nl_msg_buf_t_ {
        struct nlmsghdr *msg;
        int data_length;
        int msg_buf_length;
} rt_nl_msg_buf_t;

extern bool rtnetlink_msg_buf_new(rt_nl_msg_buf_t **msg_buf_);
extern bool rtnetlink_msg_buf_free(rt_nl_msg_buf_t **msg_buf_);
extern bool rtnetlink_recv_msg(rt_netlink_sock_t *rt_nl_sock,
                            rt_nl_msg_buf_t *msg_buf);
extern struct nlmsghdr* rtnetlink_msg_buf_get_data(rt_nl_msg_buf_t *msg_buf);
extern int rtnetlink_msg_buf_get_length(rt_nl_msg_buf_t *msg_bu);
extern void rtnetlink_parse_link_info(struct nlmsghdr *data_ptr);
