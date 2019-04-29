#include <pthread.h>
#include <setjmp.h>

#define run 1
#define blocked 0
#define exited -1
#define Max_Threads 128

typedef struct{
    pthread_t id;
    jmp_but jb;
    char *stack;
}TCB;

struct AllTCB{
    struct TCB ALLTCBs[Max_Threads];
    int Initialized;
};

struct AllTCB ThreadPool;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg){
    static int Initialized = 0;
    //initialize thread subsystem when pthread_create is called for the first time`
    if(Initialized == 0){
        Initialized = 1;
             
    }
    


}