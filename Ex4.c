//   Leitores/Escritores com prioridade para escritores usando threads e rwlocks POSIX

#include <stdio.h>
#include <pthread.h>

pthread_rwlock_t rwlock;

void* writer(void* arg) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);  
        escrever_dados();                
        pthread_rwlock_unlock(&rwlock);
    }
}

void* reader(void* arg) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);  
        ler_dados();                    
        pthread_rwlock_unlock(&rwlock); 
    }
}

int main() {
    pthread_t readers[5], writers[5];

    pthread_rwlock_init(&rwlock, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
