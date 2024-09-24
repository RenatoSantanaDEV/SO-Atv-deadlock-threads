// Leitores/Escritores com prioridade para escritores usando threads e semáforos POSIX

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int readers_count = 0;
sem_t rw_mutex, mutex;

void* writer(void* arg) {
    while (1) {
        sem_wait(&rw_mutex); 
        escrever_dados();  
        sem_post(&rw_mutex);  
    }
}

void* reader(void* arg) {
    while (1) {
        sem_wait(&mutex);    
        readers_count++;
        if (readers_count == 1)
            sem_wait(&rw_mutex);
        sem_post(&mutex);        

        ler_dados();          

        sem_wait(&mutex);        
        readers_count--;
        if (readers_count == 0) 
            sem_post(&rw_mutex);
        sem_post(&mutex);    
    }
}

int main() {
    pthread_t readers[5], writers[5];

    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    return 0;
}
