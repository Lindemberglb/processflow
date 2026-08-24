#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "processflow.h"

tarefa *encontrar_tarefa(tarefa tarefas[], int quantidade_tarefas, char nome[]){
    for (int i = 0; i < quantidade_tarefas; i++){
        if (strcmp(tarefas[i].nome, nome) == 0){
            return &tarefas[i];
        }
    }
    return NULL;
}

void preparar_redirecionamento(tarefa *tarefa_atual, char diretorio_trabalho[]){
    if (diretorio_trabalho[0] != '\0'){
        if (chdir(diretorio_trabalho) == -1){
            printf("erro ao mudar diretorio de trabalho\n");
            exit(1);
        }
    }

    if (tarefa_atual->arquivo_entrada[0] != '\0'){

        int arquivo = open(tarefa_atual->arquivo_entrada, O_RDONLY);

        if (arquivo == -1){
            printf("erro ao abrir arquivo de entrada\n");
            exit(1);
        }

        dup2(arquivo, STDIN_FILENO);
        close(arquivo);
    }

    if (tarefa_atual->arquivo_saida[0] != '\0'){
        int arquivo = open(tarefa_atual->arquivo_saida, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
        if (arquivo == -1){
            printf("erro ao abrir arquivo de saida\n");
            exit(1);
        }

        dup2(arquivo, STDOUT_FILENO);
        close(arquivo);
    }

    if (tarefa_atual->arquivo_append[0] != '\0'){

        int arquivo = open(tarefa_atual->arquivo_append, O_WRONLY | O_CREAT | O_APPEND, 0644);

        if (arquivo == -1){
            printf("erro ao abrir arquivo append\n");
            exit(1);
        }

        dup2(arquivo, STDOUT_FILENO);
        close(arquivo);
    }
}

int main(){
    char comando[100];

    tarefa t[MAXIMO_DE_TAREFAS];
    int quantidade_tarefas = 0;
    char diretorio_trabalho[100] = "";

    while (1){
        printf("processflow> ");

        if (fgets(comando, 100, stdin) == NULL){
            printf("\n");
            break;
        }

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
            t[quantidade_tarefas].arquivo_entrada[0] = '\0';
            t[quantidade_tarefas].arquivo_saida[0] = '\0';
            t[quantidade_tarefas].arquivo_append[0] = '\0';

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

        if (strncmp(comando, "workdir ", 8) == 0){
            char *parte;

            parte = strtok(comando, " ");

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("diretorio nao encontrado\n");
                continue;
            }

            strcpy(diretorio_trabalho, parte); 
        }

        if (strncmp(comando, "input ", 6) == 0){
            char *parte;

            parte = strtok(comando, " ");
            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("tarefa nao encontrada\n");
                continue;
            }

            tarefa *encontrada = encontrar_tarefa(t, quantidade_tarefas, parte);

            if (encontrada == NULL){
                printf("tarefa nao encontrada\n");
                continue;
            }

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("arquivo nao encontrado\n");
                continue;
            }

            strcpy(encontrada->arquivo_entrada, parte);
        }

        if (strncmp(comando, "output ", 7) == 0){
            char *parte;

            parte = strtok(comando, " ");
            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("tarefa nao encontrada\n");
                continue;
            }

            tarefa *encontrada = encontrar_tarefa(t, quantidade_tarefas, parte);

            if (encontrada == NULL){
                printf("tarefa nao encontrada\n");
                continue;
            }

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("arquivo nao encontrado\n");
                continue;
            }

            strcpy(encontrada->arquivo_saida, parte);
        }

        if (strncmp(comando, "append ", 7) == 0){
            char *parte;

            parte = strtok(comando, " ");
            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("tarefa nao encontrada\n");
                continue;
            }

            tarefa *encontrada = encontrar_tarefa(t, quantidade_tarefas, parte);

            if (encontrada == NULL){
                printf("tarefa nao encontrada\n");
                continue;
            }

            parte = strtok(NULL, " ");

            if (parte == NULL){
                printf("arquivo nao encontrado\n");
                continue;
            }

            strcpy(encontrada->arquivo_append, parte);
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
                        preparar_redirecionamento(encontrada, diretorio_trabalho);

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

        if (strncmp(comando, "run parallel ", 13) == 0){
            pid_t processos[MAXIMO_DE_TAREFAS];
            int quantidade_processos = 0;

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
                        preparar_redirecionamento(encontrada, diretorio_trabalho);

                        execv(encontrada->programa, argumentos_exec);

                        printf("erro ao executar o programa\n");
                        exit(1);
                    }

                    else{
                        processos[quantidade_processos] = pid;
                        quantidade_processos++;
                    }
                }
                parte = strtok(NULL, " ");
            }

            for (int i = 0; i < quantidade_processos; i++){
                waitpid(processos[i], NULL, 0);
            }
        }

        if (strncmp(comando, "run pipe ", 9) == 0){
            char *parte;
            tarefa *tarefas_pipe[MAXIMO_DE_TAREFAS];
            int quantidade_pipe = 0;

            parte = strtok(comando, " ");

            parte = strtok(NULL, " ");

            while ((parte = strtok(NULL, " ")) != NULL){
                if (quantidade_pipe >= MAXIMO_DE_TAREFAS){
                    printf("limite de tarefas atingido\n");
                    break;
                }

                tarefas_pipe[quantidade_pipe] = encontrar_tarefa(t, quantidade_tarefas, parte);

                if (tarefas_pipe[quantidade_pipe] == NULL){
                    printf("tarefa nao encontrada: %s\n", parte);
                    quantidade_pipe = 0;
                    break;
                }

                quantidade_pipe++;
            }

            if (quantidade_pipe < 2){
                printf("pipe precisa de pelo menos duas tarefas\n");
                continue;
            }

            int pipes[MAXIMO_DE_TAREFAS - 1][2];

            int erro = 0;

            for (int i = 0; i < quantidade_pipe - 1; i++){
                if (pipe(pipes[i]) == -1){
                    printf("erro ao criar pipe %d\n", i);
                    erro = 1;
                    break;
                }
            }

            if (erro){
                for (int i = 0; i < quantidade_pipe - 1; i++){
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }
                continue;
            }

            for (int i = 0; i < quantidade_pipe; i++){
                pid_t pid = fork();

                if (pid == -1){
                    printf("erro ao criar processo\n");
                    continue;
                }

                if (pid == 0){
                    if (i > 0){
                        if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1){
                            printf("erro ao redirecionar entrada\n");
                            exit(1);
                        }
                    }

                    if (i < quantidade_pipe - 1){
                        if (dup2(pipes[i][1], STDOUT_FILENO) == -1){
                            printf("erro ao redirecionar saida\n");
                            exit(1);
                        }
                    }

                    for (int j = 0; j < quantidade_pipe - 1; j++){
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }

                    char *argumentos_exec[12];

                    argumentos_exec[0] = tarefas_pipe[i]->programa;

                    for (int j = 0; j < tarefas_pipe[i]->quantidade_argumentos; j++){
                        argumentos_exec[j + 1] = tarefas_pipe[i]->argumentos[j];
                    }

                    argumentos_exec[tarefas_pipe[i]->quantidade_argumentos + 1] = NULL;

                    execv(tarefas_pipe[i]->programa, argumentos_exec);

                    printf("erro ao executar a tarefa\n");
                    exit(1);
                }
            }

            for (int i = 0; i < quantidade_pipe - 1; i++){
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            for (int i = 0; i < quantidade_pipe; i++){
                wait(NULL);
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
                char *argumentos_exec[12];

                argumentos_exec[0] = encontrada->programa;

                for (int i = 0; i < encontrada->quantidade_argumentos; i++){
                    argumentos_exec[i + 1] = encontrada->argumentos[i];
                }

                argumentos_exec[encontrada->quantidade_argumentos + 1] = NULL; 
                preparar_redirecionamento(encontrada, diretorio_trabalho);

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

