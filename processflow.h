#ifndef PROCESSFLOW_H
#define PROCESSFLOW_H
#define MAXIMO_DE_TAREFAS 10
#include <sys/types.h>

typedef struct tarefa{
    char nome[100];
    char programa[100];
    char argumentos[10][100];
    int quantidade_argumentos;
    char arquivo_entrada[100];
    char arquivo_saida[100];
    char arquivo_append[100];
}tarefa;

typedef struct job{
    int id;            
    pid_t pid;          
    int terminado;     
}job;

tarefa *encontrar_tarefa(tarefa t[], int quantidade_tarefas, char nome[]);

#endif