#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

sem_t rw_mutex; // Semáforo para acesso exclusivo à área compartilhada
sem_t mutex;    // Semáforo para proteger a contagem de leitores
sem_t write_mutex; // Semáforo para dar prioridade aos escritores
int read_count = 0; // Contador de leitores

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        sem_wait(&write_mutex); // Respeita a prioridade dos escritores
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // Primeiro leitor bloqueia os escritores
        }
        sem_post(&mutex);
        sem_post(&write_mutex);

        // Seção crítica (leitura)
        printf("Leitor %d está lendo\n", reader_id);
        sleep(1); // Simula tempo de leitura

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // Último leitor libera os escritores
        }
        sem_post(&mutex);

        sleep(1); // Simula tempo fora da seção crítica
    }
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        sem_wait(&write_mutex); // Assegura que nenhum novo leitor possa entrar
        sem_wait(&rw_mutex); // Escritor bloqueia leitores e outros escritores

        // Seção crítica (escrita)
        printf("Escritor %d está escrevendo\n", writer_id);
        sleep(2); // Simula tempo de escrita

        sem_post(&rw_mutex); // Escritor libera a área compartilhada
        sem_post(&write_mutex); // Permite entrada de leitores ou escritores

        sleep(2); // Simula tempo fora da seção crítica
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    sem_init(&write_mutex, 0, 1);

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    sem_destroy(&write_mutex);

    return 0;
}
