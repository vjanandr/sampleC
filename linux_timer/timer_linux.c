#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREAD_TIMER_EVENTS 20
#define AN_PTHREAD_TIMER_EVENT 0x10

typedef unsigned int uint32_t;


typedef uint32_t an_timer_e;
typedef bool boolean;

typedef struct an_linux_timer_t_ {
    int signum;
    uint32_t type;
    timer_t timerid;
    void *context;
} an_linux_timer_t;

typedef struct an_linux_pthread_t_ {
    char *thread_str;
    pthread_t thread_id;
    pthread_cond_t thread_cond;
    pthread_mutex_t thread_mutex;
    // Bitmask of events
    int event_type;

    pthread_mutex_t timer_events_mutex;
    an_linux_timer_t *timer_events[MAX_THREAD_TIMER_EVENTS];
    int curr_timer_event_index;
//    list *queue_events;
} an_linux_pthread_t;

an_linux_pthread_t *an_master_thread = NULL;

an_linux_timer_t an_timer_test;

// Move this to a common linux file. 
#define SIGTIMER     (SIGRTMAX)

// Move this to a common linux file. 
#define AN_LINUX_ERROR -1
#define AN_LOG_NONCE -1

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
    an_new_thread->event_type = 0;
    pthread_cond_init(&an_new_thread->thread_cond, NULL);
    pthread_mutex_init(&an_new_thread->thread_mutex, NULL); 
    pthread_mutex_init(&an_new_thread->timer_events_mutex, NULL); 
    an_new_thread->curr_timer_event_index = 0;
    an_new_thread->thread_str = str;

    *an_thread = an_new_thread;
    return true;
}

static bool an_pthread_enqueue_timer_event (an_linux_pthread_t *an_thread,
                                            an_linux_timer_t *timer)
{
    int status;
    status = pthread_mutex_lock(&an_thread->timer_events_mutex);
    if (status != 0) {
        return false;
    }
   
    if (an_thread->curr_timer_event_index >= MAX_THREAD_TIMER_EVENTS) {
        pthread_mutex_unlock(&an_thread->timer_events_mutex);
        return false;
    }

    an_thread->timer_events[an_thread->curr_timer_event_index] = timer;
    an_thread->curr_timer_event_index++;
    an_thread->event_type |= AN_PTHREAD_TIMER_EVENT;

    pthread_mutex_unlock(&an_thread->timer_events_mutex);
    return true;
}

bool an_pthread_notify_event (an_linux_pthread_t *an_thread) 
{
    int status = 0;
    printf("\nan_pthread_notify_event");
    status = pthread_mutex_lock(&an_thread->thread_mutex);
    if (status != 0) {
        return false;
    }

    status = pthread_cond_signal(&an_thread->thread_cond);
    if (status != 0) {
        pthread_mutex_unlock(&an_thread->thread_mutex);
        return false;
    }

    status = pthread_mutex_unlock(&an_thread->thread_mutex);
    if (status != 0) {
        return false;
    }
    return true;
}


bool
an_pthread_enqueue_timer_event_and_notify (an_linux_pthread_t *an_thread, 
                                an_linux_timer_t *timer)
{

    if (!an_pthread_enqueue_timer_event(an_thread, timer)) {
        return false;
    }

    if (!an_pthread_notify_event(an_thread)) {
        return false;
    }

    return true;

}

void an_handle_timer_events (int signo, siginfo_t * info, void *context)
{
    an_linux_timer_t*timer = NULL;

    timer = (an_linux_timer_t*)info->si_value.sival_ptr;

    printf("\nhandle timer events");

    an_pthread_enqueue_timer_event_and_notify(an_master_thread, timer);
}

void an_timer_services_init (void)
{
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = an_handle_timer_events;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGTIMER, &sa, NULL) == AN_LINUX_ERROR) {
        perror("sigaction failed");
    }
}

void
an_timer_init (an_linux_timer_t *timer, an_timer_e timer_type,
               void *context, boolean interrupt)
{
    struct sigevent sigev;

    if (timer == NULL) {
        return;
    }

    timer->type = timer_type;
    // Create the POSIX timer to generate signo
    sigev.sigev_notify = SIGEV_SIGNAL;
//    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_signo = SIGTIMER;
    sigev.sigev_value.sival_ptr = timer;
    timer->context = context;

    if (timer_create(CLOCK_REALTIME, &sigev, &timer->timerid) 
            == AN_LINUX_ERROR) {
        perror("sigaction failed");
    }
}

void
an_timer_uninit (an_linux_timer_t *timer) 
{
    timer_delete(timer->timerid);
}

/**
  * delay in milli seconds.
  */
void
an_timer_start (an_linux_timer_t *timer, uint32_t delay)
{
    struct itimerspec tspec;

    if (timer == NULL) { 
        return;
    }

    tspec.it_value.tv_sec = delay / 1000;
    tspec.it_value.tv_nsec = (delay * 1000000) % 1000000000;
    tspec.it_interval.tv_sec = tspec.it_value.tv_sec;
    tspec.it_interval.tv_nsec = tspec.it_value.tv_nsec;

    if (timer_settime(timer->timerid, 0, &tspec, NULL) == AN_LINUX_ERROR) {
        perror("\nFailed to start timer");
    }

}

void
an_timer_stop (an_linux_timer_t *timer) 
{ 
    struct itimerspec tspec;
    if (timer == NULL) {
        return;
    }

    memset(&tspec, 0, sizeof(struct itimerspec));

    if (timer_settime(timer->timerid, 0, &tspec, NULL) 
            == AN_LINUX_ERROR) {
        perror("\t\tFailed to stop timer");
    }
}

uint32_t
an_mgd_timer_type (an_linux_timer_t *expired_timer)
{
    if (expired_timer == NULL) {
        return AN_LOG_NONCE;
    }

    return (expired_timer->type);
}

void
an_timer_reset (an_linux_timer_t *timer, uint32_t delay)
{
    return;
}

void *
an_mgd_timer_context (an_linux_timer_t *expired_timer)
{
    return (expired_timer->context);
}

bool 
an_pthread_wait_for_event (an_linux_pthread_t *thread) 
{
    int status;

    status = pthread_mutex_lock(&an_master_thread->thread_mutex);

    if (status != 0) {
        return false;
    }
    
    if (!an_master_thread->event_type) {
        // No event present wait for an event to occur
        printf("\n thread waiting for event");
        status = pthread_cond_wait(&an_master_thread->thread_cond,
                                   &an_master_thread->thread_mutex);
    }

    printf("\n received event signal");
    
    if (status != 0) {
        pthread_mutex_unlock(&an_master_thread->thread_mutex);
        return false;
    } 
    
    pthread_mutex_unlock(&an_master_thread->thread_mutex);
    return true;
}


boolean an_pthread_any_pending_timer_event (an_linux_pthread_t *thread) 
{
    int status;

    status = pthread_mutex_lock(&thread->timer_events_mutex);

    if (status != 0) {
        return false;
    }

    if (!(thread->event_type & AN_PTHREAD_TIMER_EVENT)) {
        status = pthread_mutex_unlock(&thread->timer_events_mutex);
        return false;
    }
    if (thread->curr_timer_event_index == 0) {
        // something wrong.
        thread->event_type &= ~AN_PTHREAD_TIMER_EVENT;
        status = pthread_mutex_unlock(&thread->timer_events_mutex);
        return false;
    }

    status = pthread_mutex_unlock(&thread->timer_events_mutex);
    return true;
}


void an_pthread_get_pending_timer_event (an_linux_pthread_t *thread,
                                         an_linux_timer_t **expired_timer)
{
    int status = 0, index = 0;
    *expired_timer = NULL;

    status = pthread_mutex_lock(&thread->timer_events_mutex);

    if (status != 0) {
        return ;
    }

    if (!(thread->event_type & AN_PTHREAD_TIMER_EVENT)) {
        status = pthread_mutex_unlock(&thread->timer_events_mutex);
        return;
    }
    if (thread->curr_timer_event_index == 0) {
        // something wrong.
        thread->event_type &= ~AN_PTHREAD_TIMER_EVENT;
        status = pthread_mutex_unlock(&thread->timer_events_mutex);
        return;
    }

    while (index < thread->curr_timer_event_index) {
         if (thread->timer_events[index]) {
             *expired_timer = thread->timer_events[index];
             thread->timer_events[index] = NULL;
             break;
         }
         index++;
    }

    if (index >= thread->curr_timer_event_index) {
        // No more pending events.
        thread->event_type &= ~AN_PTHREAD_TIMER_EVENT;
    }

    status = pthread_mutex_unlock(&thread->timer_events_mutex);

}

bool 
an_pthread_get_waiting_event_type (an_linux_pthread_t *thread,
                                   int *evt_type)
{
    int status;
    int thread_events = 0, evnt = 1;
    *evt_type = 0;

    status = pthread_mutex_lock(&thread->thread_mutex);

    if (status != 0) {
        return false;
    }

    thread_events = thread->event_type;
    while (thread_events) {
        if (thread_events & evnt) {
            *evt_type = evnt;
//            thread->event_type &= ~evnt;
            break;
        }
        evnt = evnt << 1;
    }

    pthread_mutex_unlock(&thread->thread_mutex);
    return true;
}

void * an_master_thread_func (void *arg)
{
    int status;
    int evt_type;
    an_linux_timer_t *expired_timer;

    printf("\n In an_master_thread_func");
    //sleep(5);

    while (an_pthread_wait_for_event(an_master_thread)) {
        while (an_pthread_get_waiting_event_type(an_master_thread, &evt_type)) {
            switch (evt_type) {
                case AN_PTHREAD_TIMER_EVENT:
                    while (an_pthread_any_pending_timer_event(
                                an_master_thread)) {
                        an_pthread_get_pending_timer_event(an_master_thread,
                                                           &expired_timer); 
                        if (!expired_timer) {
                            break;
                        }
                        printf("\nHandle timer expired event of type %d", 
                                an_mgd_timer_type(expired_timer));
                    }
                    break;
            }
        }
    }
    printf("\n exiting an_master_thread_func");
}

void main () 
{
    int retval;
 
    retval = an_pthread_create(&an_master_thread, "Master thread",
                                an_master_thread_func, NULL);

    an_timer_services_init();

    an_timer_init(&an_timer_test, 10,
                   NULL, true);

    printf("\n Timer started");
    an_timer_start(&an_timer_test, 600);


    pthread_join(an_master_thread->thread_id, NULL);
    
}
