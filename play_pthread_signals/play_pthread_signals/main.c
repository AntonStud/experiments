#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>

#include <stdio.h>
#include <signal.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
const int period = 100;
const int totalTime = 50000;
const int utime = 5;

void thread_sig_handler(int signum)
{
    /*pthread_id_np_t   tid;
    pthread_t         self;
    self = pthread_self();
    pthread_getunique_np(&self, &tid);
*/
    printf("Thread exited\n");
    pthread_exit(NULL);
}

void* thread_block_signal()
{
    sigset_t set;
    char name[] = "block";
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    int i = totalTime;
    while(1){
    pthread_sigmask(SIG_BLOCK, &set,NULL);
    printf("%s: Start of while \n", name);
    while(i--)
    {
        if(!(i% period))
            printf("%s: %d\n", name, i);
        usleep(utime);
    }
    printf("%s: end of while \n", name);
    pthread_sigmask(SIG_UNBLOCK, &set,NULL);
    }

    return NULL;
}

void* thread_non_block_signal()
{
    char name[] = "free";
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    int i = totalTime;

    printf("%s: Start of while \n", name);
    while(i--)
    {
        if(!(i% period))
            printf("%s: %d\n", name, i);
        usleep(utime);
    }
    printf("%s: end of while \n", name);


    return NULL;
}

int main(void)
{
    char name[] = "main";
    pthread_t t1, t2;
    struct sigaction action;
    printf("Start of main \n");
    memset(&action, 0, sizeof(action));

    action.sa_sigaction = &thread_sig_handler;
    action.sa_flags = 0;
    sigemptyset(&(action.sa_mask));
    sigaction(SIGUSR1, &action, NULL);

    pthread_create(&t1, NULL, &thread_block_signal, NULL);
    pthread_create(&t2, NULL, &thread_non_block_signal, NULL);

    sleep(2);
    printf("%s: send signal t1 \n", name);
    pthread_kill(t1, SIGUSR1);
    printf("%s: send signal t2\n", name);
    pthread_kill(t2, SIGUSR1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("End of main \n");


    return 0;
}
