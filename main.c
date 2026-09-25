#include "pthreadfuncs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>


int main(void) {
    // headline
    about();
    // array with threads
    pthread_t threads[COUNT_THREADS];
    struct ThreadArgs args[COUNT_THREADS];

    // sys call - open
    // file, modes, rights
    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    char main_buf[128];
    snprintf(main_buf, sizeof(main_buf),
             "main: pid = %d, opened file: 'output.log' (fd = %d)\n",
             getThreadID(), g_fd);
    write_line(main_buf);

    // create structs for threads
    for (int i = 0; i < COUNT_THREADS; i++) {
        args[i].id = i + 1;
        snprintf(args[i].tag, sizeof(args[i].tag), "T%d", i);
        snprintf(args[i].message, sizeof(args[i].message),
                 "Hello from main to thread %s\n", args[i].tag);
    }

    // create threads
    for (int i = 0; i < COUNT_THREADS; i++) {
        int rc = pthread_create(&threads[i], NULL, func_thread, &args[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            return EXIT_FAILURE;
        }
    }

    // wait stoping all thread
    for (int i = 0; i < COUNT_THREADS; i++) {
        void *ret = NULL;
        int rc = pthread_join(threads[i], &ret);
        if (rc != 0) {
            printf("pthread_join for thread %d: rc = %d (%s)\n",
                   i, rc, strerror(rc));
        } else if (ret != NULL) {
            printf("main got: %s", (char *)ret);
            free(ret);
        }
    }

    // sys call for close file
    // задание 33: сравнение pthread_t через pthread_equal
    printf("\n--- pthread_equal demo ---\n");

    // сравниваем два разных потока
    if (pthread_equal(threads[0], threads[1])) {
        printf("threads[0] == threads[1]\n");
    } else {
        printf("threads[0] != threads[1]  (разные потоки)\n");
    }

    // сравниваем поток с самим собой
    if (pthread_equal(threads[0], threads[0])) {
        printf("threads[0] == threads[0]  (тот же поток)\n");
    } else {
        printf("threads[0] != threads[0]\n");
    }

    // сравниваем pthread_self() (главный поток) с threads[0]
    if (pthread_equal(pthread_self(), threads[0])) {
        printf("main == threads[0]\n");
    } else {
        printf("main != threads[0]  (главный поток ≠ рабочий)\n");
    }
}
