#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <queue>

#define STATUS int
#define RUNNING 1
#define BLOCKED 0
#define EXITED -1
#define Max_Threads 128
#define INTERVAL 50

struct TCB{
    pthread_t id;
    jmp_buf jb;
    STATUS status;
    void *(*thread_start_routine)(void*);
	void* thread_arg;
};

static std::queue<TCB> Thread_Pool;
static int Initialized = 0;
static int numThreads = 0;
// mangle function
static long int i64_ptr_mangle(long int p){
    long int ret;
    asm(" mov %1, %%rax;\n"
        " xor %%fs:0x30, %%rax;"
        " rol $0x11, %%rax;"
        " mov %%rax, %0;"
        : "=r"(ret)
        : "r"(p)
        : "%rax"
        );
        return ret;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg){

    //initialize thread subsystem when pthread_create is called for the first time
    if(Initialized == 0){
        Initialized = 1;
        
        /* timer setting */
        struct itimerval it_val;
	    struct sigaction siga;

        if (sigaction(SIGALRM, &siga, NULL) == -1) {
    	    perror("Error calling sigaction()");
    	    exit(1);
 	    }
  
  	    it_val.it_value.tv_sec =     INTERVAL/1000;
  	    it_val.it_value.tv_usec =    (INTERVAL*1000) % 1000000;   
  	    it_val.it_interval = it_val.it_value;
  
  	    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
    	    perror("Error calling setitimer()");
    	    exit(1);
    	}
        /* timer setting ends */

        /* storing main routine into TCB */
        TCB MainThread;
        MainThread.id = numThreads;
        MainThread.status = RUNNING;
        MainThread.thread_arg = NULL;
        MainThread.thread_start_routine = NULL;
        setjmp(MainThread.jb);
        /*storing main routine into TCB done */

        /*push main TCB into thread pool*/
        Thread_Pool.push(MainThread);
        numThreads++;
    }

    /* create new threads */
    TCB NewThread;
    NewThread.id = numThreads;
    
}