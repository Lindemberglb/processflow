#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "processflow.h"

int main(){
    char comando[100];

    tarefa t;

    while (1)
    {
        printf("processflow> ");

        fgets(comando, 100, stdin);

        comando[strcspn(comando, "\n")] = '\0';

        if (strcmp(comando, "exit") == 0){
            break;
        }

        if (strncmp(comando, "task ", 5) == 0){
            char *parte;

            parte = strtok(comando, " ");

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("tarefa não foi encontrada\n");
                continue;
            }

            strcpy(t.nome, parte);

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("programa nao foi encontrado\n");
                continue;
            }

            strcpy(t.programa, parte);

            t.quantidade_argumentos = 0;

            while ((parte = strtok(NULL, " ")) != NULL){
                if (t.quantidade_argumentos >= 10){
                    printf("limite de argumentos atingido\n");
                    break;
                }

                strcpy(t.argumentos[t.quantidade_argumentos],parte);
                t.quantidade_argumentos++;
            }

            printf("tarefa cadastrada\n");
            printf("nome: %s\n", t.nome);
            printf("programa: %s\n", t.programa);

            printf("argumentos: ");
            for (int i = 0; i < t.quantidade_argumentos; i++){
                printf(" %s", t.argumentos[i]);
            }

            printf("\n");
        }
    }
    return 0;
}

