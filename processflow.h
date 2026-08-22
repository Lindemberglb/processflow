#ifndef PROCESSFLOW_H
#define PROCESSFLOW_H

typedef struct tarefa{
    char nome[100];
    char programa[100];
    char argumentos[10][100];
    int quantidade_argumentos;
}tarefa;

#endif