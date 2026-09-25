#include "pthreadfuncs.h"

// common resources - is a file for logging
int g_fd = -1;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

// задание 29: счётчик и его мьютекс (оставлены, не мешают)
int g_counter = 0;
pthread_mutex_t g_counter_lock = PTHREAD_MUTEX_INITIALIZER;

// git ID of current thread
pid_t getThreadID(void) {
    return (pid_t) syscall(SYS_gettid);
}

// write a string from thread with mutex
int write_line(const char *msg) {
    int ret = 0;

    pthread_mutex_lock(&g_lock);
    ssize_t n = write(g_fd, msg, strlen(msg));
    if (n < 0) {
        fprintf(stderr, "write() failed: %s, [file descr = %d]\n", strerror(errno), g_fd);
        ret = -1;
    }
    pthread_mutex_unlock(&g_lock);

    return ret;
}

// function for thread
void *func_thread(void *arg){
    struct ThreadArgs *t = (struct ThreadArgs *)arg;

    char buf[128];
    // задание 24: пишем сообщение из main в лог
    write_line(t->message);

    // write something in opened file
    for (int i = 0; i < COUNT_ITERATIONS; ++i) {
        snprintf(buf, sizeof(buf),
                 "[tag = %s] pid = %d ppid = %d tid = %d pthread_self = %lu iter = %d\n",
                 t->tag, getpid(), getppid(), getThreadID(),
                 (unsigned long)pthread_self(), i);
        write_line(buf);
        // imitation of something
        struct timespec ts;
        ts.tv_sec  = 1;              /* 1 секунда */
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }

    // задание 25: возвращаем строку с числом итераций через malloc
    char *result = malloc(64);
    if (result == NULL) {
        return NULL;
    }
    snprintf(result, 64, "thread %s: %d iterations done\n",
             t->tag, COUNT_ITERATIONS);
    return result;
}

void about()  {
    printf("Pthread example\n");
}
//Задание 11 лабы
