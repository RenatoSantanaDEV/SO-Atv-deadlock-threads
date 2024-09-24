# Projeto de Concorrência em C

Este projeto implementa soluções clássicas de sincronização e concorrência, como o **problema do Produtor/Consumidor** e o **problema dos Leitores/Escritores**, utilizando threads POSIX, semáforos, variáveis de condição e bloqueios de leitura/escrita (rwlocks). As implementações foram feitas em linguagem C e seguem o padrão POSIX.



## Funcionalidades

### 1. Produtor/Consumidor com Semáforos POSIX
- Implementação que resolve o problema do produtor/consumidor usando semáforos para coordenar a inserção e remoção de itens de um buffer circular.
- Garante que o produtor não insira itens em um buffer cheio e que o consumidor não remova itens de um buffer vazio.
  
### 2. Produtor/Consumidor com Variáveis de Condição POSIX
- Alternativa ao problema do produtor/consumidor, utilizando variáveis de condição e mutexes para sincronizar as operações de produção e consumo.
  
### 3. Leitores/Escritores com Prioridade para Escritores usando Semáforos POSIX
- Solução que implementa o problema dos leitores/escritores com a prioridade para os escritores, garantindo que escritores tenham acesso preferencial ao recurso compartilhado, bloqueando leitores quando necessário.
  
### 4. Leitores/Escritores com Prioridade para Escritores usando RWLocks POSIX
- Implementação similar à anterior, mas utilizando rwlocks (`pthread_rwlock_t`), uma abstração de bloqueios de leitura/escrita mais eficiente para controlar o acesso de leitores e escritores ao recurso compartilhado.

## Estrutura do Projeto

O projeto é organizado da seguinte forma:

