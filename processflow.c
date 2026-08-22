#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "processflow.h"

int main(){
    char comando[100];

    tarefa t[MAXIMO_DE_TAREFAS];
    int quantidade_tarefas = 0;

    while (1)
    {
        printf("processflow> ");

        fgets(comando, 100, stdin);

        comando[strcspn(comando, "\n")] = '\0';

        if (strcmp(comando, "exit") == 0){
            break;
        }

        if (strncmp(comando, "task ", 5) == 0){
            if (quantidade_tarefas >= MAXIMO_DE_TAREFAS){
                printf("limite de tarefas atingido\n");
                continue;
            }

            char *parte;

            parte = strtok(comando, " ");

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("tarefa não foi encontrada\n");
                continue;
            }

            strcpy(t[quantidade_tarefas].nome, parte);

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("programa nao foi encontrado\n");
                continue;
            }

            strcpy(t[quantidade_tarefas].programa, parte);

            t[quantidade_tarefas].quantidade_argumentos = 0;

            while ((parte = strtok(NULL, " ")) != NULL){
                if (t[quantidade_tarefas].quantidade_argumentos >= 10){
                    printf("limite de argumentos atingido\n");
                    break;
                }

                strcpy(t[quantidade_tarefas].argumentos[t[quantidade_tarefas].quantidade_argumentos],parte);
                t[quantidade_tarefas].quantidade_argumentos++;
            }

            quantidade_tarefas++;

            int quant_arg = 0;

            printf("tarefa cadastrada\n");
            printf("nome: %s\n", t[quantidade_tarefas - 1].nome);
            printf("programa: %s\n", t[quantidade_tarefas - 1].programa);
            printf("argumentos: ");
            for (int i = 0; i < t[quantidade_tarefas - 1].quantidade_argumentos; i++){
                printf(" %s", t[quantidade_tarefas - 1].argumentos[i]);
            }
            printf("\n");
        }
    }
    return 0;
}

