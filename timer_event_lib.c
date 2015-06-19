#include <event2/event.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>

#include <syslog.h>
#include <sys/sysctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/route.h>
#include <stdio.h>
#include <sys/types.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include "rt_netlink_utils.h"


//an_log_type_e an_get_log_timer_type(an_timer_e timer_type);

//const uint8_t * an_timer_get_timer_type_str(an_timer_e timer_type);
//void an_timer_services_uninit(void);


//void an_timer_start64(an_timer *timer, int64_t delay);
//void an_timer_stop(an_timer *timer);
//void an_timer_reset(an_timer *timer, uint32_t delay);
//void an_timer_update(an_timer *timer, uint32_t delay);
//boolean an_timer_is_expired(an_timer *timer);
//boolean an_timer_is_running(an_timer *timer);

//uint32_t an_mgd_timer_type(an_timer *expired_timer);
//void *an_mgd_timer_context(an_timer *expired_timer);
//void an_process_timer_events(an_timer *expired_timer);
//boolean an_handle_timer_events(void);
//boolean an_helper_handle_timer_events(void);

typedef bool boolean;

typedef enum if_state_t_ {
    IF_DOWN,
    IF_UP
} if_state_t;

typedef struct intf_db_t_ {
    int if_index;
    char *if_name;
    if_state_t state;
} intf_db_t;

#define IF_LIST_MAX 10

intf_db_t if_list[IF_LIST_MAX];

rt_netlink_sock_t *_netlink_sock = NULL;

typedef struct an_linux_pthread_t_ {
    char *thread_str;
    pthread_t thread_id;
} an_linux_pthread_t;

an_linux_pthread_t *an_evnt_handler_thread = NULL;

typedef enum an_timer_e_ {
    AN_TIMER_TEST
} an_timer_e;

typedef struct an_timer_t_ {
    uint32_t delay;
    int type;
    struct event *evnt_handle;
    void *context;
} an_timer_t;

typedef an_timer_t an_timer;
struct event_base *_evt_base = NULL;
an_timer_t timer_test;
an_timer_t timer_test_2;

void an_timer_services_init (void) 
{
    if (!_evt_base) {
        _evt_base = event_base_new();
    }
}

static void timer_cbk (int fd, short type, void *args)
{
    an_timer_t *timer = args;

    if (!args) {
        return;
    }

    printf("\ntimer cbk handler thread ID %d",(int)an_evnt_handler_thread->thread_id);
    printf("\n Event cbk received %d, ctxt %s", timer->type, (char *)timer->context);
    fflush(stdout);
}

void an_timer_init (an_timer *timer, an_timer_e timer_enum,
                    void *context, boolean interrupt)
{
    if (!timer || !_evt_base) {
        return ;
    }

//    timer->evnt_handle = evtimer_new(_evt_base, timer_cbk, timer);
    timer->evnt_handle = event_new(_evt_base, -1, EV_PERSIST, 
                                  timer_cbk, timer);
    timer->context = context;
}

void an_timer_start (an_timer *timer, uint32_t delay)
{
    struct timeval timeout;

    if (!timer) {
        return;
    }

    timer->delay = delay;

    timeout.tv_sec = delay/1000;
    timeout.tv_usec = (delay%1000)*1000;

    evtimer_add(timer->evnt_handle,  &timeout);
}


bool an_pthread_create (an_linux_pthread_t **an_thread, char *str,
                        void *(*start_routine) (void *), void *arg)
{
    an_linux_pthread_t *an_new_thread;
    pthread_t pthread_id;
    int retval;

    retval = pthread_create(&pthread_id, NULL, start_routine, arg);
    if (retval != 0) {
        return false;
    }

    an_new_thread = malloc(sizeof(an_linux_pthread_t));

    if (!an_new_thread) {
        return false;
    }

    an_new_thread->thread_id = pthread_id;
    *an_thread = an_new_thread;
}

void* 
an_event_handler_thread_func (void *arg)
{
    printf("\n Before event_base_dispatch");

    event_base_loop(_evt_base, EVLOOP_NO_EXIT_ON_EMPTY);
//    event_base_dispatch(_evt_base);
    printf("\n After event_base_dispatch");
}

void
an_receive_netlink_info (rt_netlink_sock_t *rt_nt_sock)
{
    rt_nl_msg_buf_t *msg_buf = NULL;
    bool msg_done = false;

    if (!rtnetlink_msg_buf_new(&msg_buf)) {
        printf("\n return from msg buf new");
        return;
    }

    while (!msg_done) {
        int len;
        struct nlmsghdr *data_ptr = NULL;

        if (!rtnetlink_recv_msg(rt_nt_sock, msg_buf)) { 
            break;
        }
        data_ptr = rtnetlink_msg_buf_get_data(msg_buf);
        len = rtnetlink_msg_buf_get_length(msg_buf);

        if (len == 0) {
            break;
        }

        for (; NLMSG_OK(data_ptr, len); data_ptr = NLMSG_NEXT(data_ptr, len)) {
            switch (data_ptr->nlmsg_type) {
                case NLMSG_DONE:
                    msg_done = true;
                    break;
                case RTM_NEWLINK:
                    rtnetlink_parse_link_info(data_ptr);
                    break;
                default:
                    printf("\n msg type received %d not requested",
                            data_ptr->nlmsg_type);
                    break;
            }
        }
    }
    rtnetlink_msg_buf_free(&msg_buf);
}


void
an_handle_intf_events (evutil_socket_t listener, short event, void *arg)
{
    rt_netlink_sock_t *rt_nt_sock = arg;

    printf("\n handle interface events");

    an_receive_netlink_info(rt_nt_sock);
}


void
an_register_interface_events (rt_netlink_sock_t *rt_nt_sock)
{
    evutil_socket_t listener;
    struct event *listener_event;

    listener = rt_netlink_sock_fd_get(rt_nt_sock);

    evutil_make_socket_nonblocking(listener);

    if (!_evt_base) {
        _evt_base = event_base_new();
    }

    listener_event = event_new(_evt_base, listener, EV_READ|EV_PERSIST,
                               an_handle_intf_events, rt_nt_sock);

    event_add(listener_event, NULL);
}


void
an_get_interface_list (void)
{
    printf("\n Get interface list");

    if (!_netlink_sock) {
        int groups = RTMGRP_LINK;

        if (!rtnetlink_sock_new(&_netlink_sock)) {
            return;
        }

        if (!rtnetlink_sock_create_bind(_netlink_sock, groups)) {
            return;
        }
    }
    
    if (!rtnetlink_send_request(_netlink_sock, RTM_GETLINK, 
                AF_PACKET,NLM_F_REQUEST | NLM_F_DUMP)) {
        return; 
    } 

    an_receive_netlink_info(_netlink_sock);
    an_register_interface_events(_netlink_sock);
}

void main ()
{
    int retval;

    an_timer_services_init();
    an_timer_init(&timer_test, AN_TIMER_TEST, (void *)"Hello world test", false);
    an_timer_start(&timer_test, 3000);
    retval = an_pthread_create(&an_evnt_handler_thread, "Event handler thread",
                                an_event_handler_thread_func, NULL);

    //an_timer_init(&timer_test_2, AN_TIMER_TEST, (void *)"Hello world test 2", false);
    //an_timer_start(&timer_test_2, 3000);

    an_get_interface_list();

    pthread_join(an_evnt_handler_thread->thread_id, NULL);
}
