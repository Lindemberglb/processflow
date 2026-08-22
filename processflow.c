#include <stdio.h>
#include <string.h>

#include "processflow.h"

int main(){
    char comando[100];

    while (1)
    {
        printf("processflow> ");

        fgets(comando, 100, stdin);

        comando[strcspn(comando, "\n")] = '\0';

        if (strcmp(comando, "exit") == 0){
            break;
        }
    }
    return 0;
}