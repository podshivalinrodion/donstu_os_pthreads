#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// ============================================================
// Задание 31: кольцевой буфер на N элементов
// Две условные переменные: not_full и not_empty.
// ============================================================

#define N 5              // размер кольцевого буфера
#define NUM_ITEMS 20     // сколько элементов произведёт producer

int buffer[N];           // сам кольцевой буфер
int head = 0;            // индекс для записи (producer)
int tail = 0;            // индекс для чтения (consumer)
int count = 0;           // сколько элементов сейчас в буфере

pthread_mutex_t rb_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  not_full     = PTHREAD_COND_INITIALIZER;   // сигнал: буфер не полон
pthread_cond_t  not_empty    = PTHREAD_COND_INITIALIZER;   // сигнал: буфер не пуст


// ---------- производитель ----------
void *producer(void *arg) {
    (void)arg;
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&rb_mutex);

        // ждём, пока буфер не полон
        while (count == N) {
            printf("[producer] buffer full, waiting...\n");
            pthread_cond_wait(&not_full, &rb_mutex);
        }

        // кладём элемент
        buffer[head] = i;
        head = (head + 1) % N;
        count++;
        printf("[producer] put %d (count = %d)\n", i, count);

        // будим consumer'а — появились данные
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&rb_mutex);

        usleep(50 * 1000);  // 50 мс
    }
    return NULL;
}


// ---------- потребитель ----------
void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&rb_mutex);

        // ждём, пока буфер не пуст
        while (count == 0) {
            printf("[consumer] buffer empty, waiting...\n");
            pthread_cond_wait(&not_empty, &rb_mutex);
        }

        // забираем элемент
        int val = buffer[tail];
        tail = (tail + 1) % N;
        count--;
        printf("[consumer] got %d (count = %d)\n", val, count);

        // будим producer'а — появилось место
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&rb_mutex);

        usleep(100 * 1000);  // 100 мс (медленнее producer'а)
    }
    return NULL;
}


int main(void) {
    printf("=== Задание 31: ring buffer (N = %d) ===\n", N);

    pthread_t prod_thread, cons_thread;

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    printf("=== done ===\n");
    return 0;
}
