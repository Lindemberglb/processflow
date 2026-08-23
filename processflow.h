#ifndef PROCESSFLOW_H
#define PROCESSFLOW_H
#define MAXIMO_DE_TAREFAS 10

typedef struct tarefa{
    char nome[100];
    char programa[100];
    char argumentos[10][100];
    int quantidade_argumentos;
}tarefa;

tarefa *encontrar_tarefa(tarefa t[], int quantidade_tarefas, char nome[]);

#endif