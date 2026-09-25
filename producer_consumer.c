#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// ============================================================
// Задание 30: producer-consumer с одним буфером
// Один поток пишет данные, другой читает.
// Синхронизация: мьютекс + одна условная переменная.
// ============================================================

#define BUF_SIZE 256
#define NUM_ITEMS 10

// общий буфер (одна ячейка)
char shared_buf[BUF_SIZE];

// флаг: 1 — буфер полон, 0 — буфер пуст
int has_data = 0;

// мьютекс и одна условная переменная
pthread_mutex_t pc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  pc_cond  = PTHREAD_COND_INITIALIZER;


// ---------- производитель ----------
void *producer(void *arg) {
    (void)arg;
    for (int i = 0; i < NUM_ITEMS; i++) {
        // генерируем строку
        char msg[BUF_SIZE];
        snprintf(msg, sizeof(msg), "item %d from producer\n", i);

        pthread_mutex_lock(&pc_mutex);

        // пока буфер полон — ждём, пока потребитель его заберёт
        while (has_data) {
            pthread_cond_wait(&pc_cond, &pc_mutex);
        }

        // записываем данные в буфер
        strncpy(shared_buf, msg, BUF_SIZE - 1);
        shared_buf[BUF_SIZE - 1] = '\0';
        has_data = 1;
        printf("[producer] put: %s", shared_buf);

        // будим потребителя
        pthread_cond_signal(&pc_cond);
        pthread_mutex_unlock(&pc_mutex);

        // имитация работы
        usleep(100 * 1000);
    }
    return NULL;
}


// ---------- потребитель ----------
void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&pc_mutex);

        // пока буфер пуст — ждём, пока производитель положит данные
        while (!has_data) {
            pthread_cond_wait(&pc_cond, &pc_mutex);
        }

        // забираем данные из буфера
        char local[BUF_SIZE];
        strncpy(local, shared_buf, BUF_SIZE - 1);
        local[BUF_SIZE - 1] = '\0';
        has_data = 0;
        printf("[consumer] got: %s", local);

        // будим производителя
        pthread_cond_signal(&pc_cond);
        pthread_mutex_unlock(&pc_mutex);

        // имитация работы
        usleep(100 * 1000);
    }
    return NULL;
}


int main(void) {
    printf("=== Задание 30: producer-consumer ===\n");

    pthread_t prod_thread, cons_thread;

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    printf("=== done ===\n");
    return 0;
}

