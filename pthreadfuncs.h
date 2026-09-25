#ifndef PTHREADFUNCS_H
#define PTHREADFUNCS_H

#include <sched.h>
#include <time.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>


// count of threads
#define COUNT_THREADS 4
// count of iterations
#define COUNT_ITERATIONS 100

// struct send to thread
struct ThreadArgs {
    // my ID of thread
    int  id;
    // something string (with name of thread, f.e.)
    char tag[10];
    // message from main to thread
    char message[64];
};


// common resources - is a file for logging
extern int g_fd;

// mutex
extern pthread_mutex_t g_lock;
// задание 29: счётчик и его мьютекс
extern int g_counter;
extern pthread_mutex_t g_counter_lock;
// git ID of current thread
pid_t getThreadID(void);

// write a string from thread with mutex
int write_line(const char *msg);

// pointer for thread's function
void *func_thread(void *arg);

void about(void);

#endif
