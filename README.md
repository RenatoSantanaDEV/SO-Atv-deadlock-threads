# Projeto de Concorrência em C

Este projeto implementa soluções clássicas de sincronização e concorrência, como o **problema do Produtor/Consumidor** e o **problema dos Leitores/Escritores**, utilizando threads POSIX, semáforos, variáveis de condição e bloqueios de leitura/escrita (rwlocks). As implementações foram feitas em linguagem C e seguem o padrão POSIX.

## Integrantes do Grupo

- **Renato Santana**
- **Thiago Belo**
- **Minguel Andrade**
- **João Pedro**

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

## Requisitos

- Sistema operacional baseado em Linux (ou ambiente que suporte POSIX Threads).
- Compilador **GCC**.
- Biblioteca **pthread** para suporte a threads POSIX.
  
Para instalar as dependências no Ubuntu, execute o seguinte comando:

```bash
sudo apt update
sudo apt install gcc make
gcc -o Ex1 Ex1.c -lpthread
gcc -o Ex2 Ex2.c -lpthread
gcc -o Ex3 Ex3.c -lpthread
gcc -o Ex4 Ex4.c -lpthread
./Ex1      # Executa a solução do Produtor/Consumidor com Semáforos
./Ex2      # Executa a solução do Produtor/Consumidor com Variáveis de Condição
./Ex3      # Executa a solução dos Leitores/Escritores com Semáforos
./Ex4      # Executa a solução dos Leitores/Escritores com RWLocks

