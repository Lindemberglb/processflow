#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "processflow.h"

tarefa *encontrar_tarefa(tarefa tarefas[], int quantidade_tarefas, char nome[]){
    for (int i = 0; i < quantidade_tarefas; i++){
        if (strcmp(tarefas[i].nome, nome) == 0){
            return &tarefas[i];
        }
    }
    return NULL;
}

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
                printf("\n");
                continue;
            }

            char *parte;

            parte = strtok(comando, " ");

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("tarefa não foi encontrada. Tente de novo\n");
                printf("\n");
                continue;
            }

            strcpy(t[quantidade_tarefas].nome, parte);

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("programa nao foi encontrado. Tente de novo\n");
                printf("\n");
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
                quant_arg++;
            }
            printf("\n");
            printf("quantidade de argumentos: %d\n" , quant_arg);
        }

        if (strncmp(comando, "run sequential ", 15) == 0){
            char *parte = strtok(comando, " ");

            parte = strtok(NULL, " ");
            parte = strtok(NULL, " ");

            while (parte != NULL){
                tarefa *encontrada = encontrar_tarefa(t, quantidade_tarefas, parte);

                if (encontrada == NULL){
                    printf("tarefa nao encontrada\n");
                }

                else{
                    pid_t pid = fork();

                    if (pid == -1){
                        printf("erro ao criar processo\n");
                    }

                    else if (pid == 0){
                        char *argumentos_exec[12];

                        argumentos_exec[0] = encontrada->programa;

                        for (int i = 0; i < encontrada->quantidade_argumentos; i++){
                            argumentos_exec[i + 1] = encontrada->argumentos[i];
                        }

                        argumentos_exec[encontrada->quantidade_argumentos + 1] = NULL;

                        execv(encontrada->programa, argumentos_exec);

                        printf("erro ao executar o programa\n");
                        exit(1);
                    }

                    else{
                        wait(NULL);
                    }
                }
                parte = strtok(NULL, " ");
            }
        }

        if (strncmp(comando, "run ", 4) == 0){
            char *nome = comando + 4;
            tarefa *encontrada = encontrar_tarefa(t, quantidade_tarefas, nome);
            if (encontrada == NULL){
                printf("tarefa nao encontrada\n");
                printf("\n");
                continue;
            }

            printf("tarefa encontrada\n");
            printf("nome: %s\n", encontrada->nome);
            printf("programa: %s\n", encontrada->programa);

            pid_t pid = fork();

            if (pid == -1){
                printf("erro ao criar processo\n");
                continue;
            }

            if (pid == 0){
                char *argumentos_exec[11];

                argumentos_exec[0] = encontrada->programa;

                for (int i = 0; i < encontrada->quantidade_argumentos; i++){
                    argumentos_exec[i + 1] = encontrada->argumentos[i];
                }

                argumentos_exec[encontrada->quantidade_argumentos + 1] = NULL; 

                execv(encontrada->programa, argumentos_exec);
                printf("erro ao executar o programa\n");
                exit(1);
            }

            wait(NULL);
        }
        printf("\n");
    }
    return 0;
}

