## ProcessFlow

O ProcessFlow é um programa desenvolvido em C para cadastrar e executar
tarefas como processos do sistema operacional.

A ideia principal é permitir que o usuário registre uma tarefa com um
nome, um programa e seus argumentos. Depois disso, essa tarefa pode ser
executada de diferentes formas.

O programa trabalha com execução sequencial, execução paralela, pipes,
jobs em background, redirecionamento de entrada e saída, diretório de
trabalho e arquivos de workflow.

## Arquivos

# processflow.c

É o arquivo principal. Nele ficam a função main e as rotinas
responsáveis por ler comandos, cadastrar tarefas, criar processos,
executar programas, criar pipes, controlar jobs e fazer
redirecionamentos.

# processflow.h

Contém as estruturas utilizadas pelo programa.
A estrutura tarefa guarda:
nome da tarefa;
caminho do programa;
até 10 argumentos;
quantidade de argumentos;
arquivo de entrada;
arquivo de saída;
arquivo de append.
A estrutura job guarda:
id: identificador do job;
pid: PID do processo;
terminado: indica se o processo já terminou.

# Makefile
Facilita a compilação e a execução do programa.
Compilação
Para compilar: make
O Makefile executa: gcc processflow.c -o processflow
Para executar: ./processflow
Também é possível usar: make run
Para remover o executável: make clean
comandos

#task
task <nome> <programa> [argumentos]
O programa armazena essas informações em uma estrutura tarefa.

#run
run <tarefa>
Executa uma tarefa e espera que ela termine.

# run sequential
run sequential <tarefa1> <tarefa2> 
Executa as tarefas uma depois da outra.

tempo1: |----------|
tempo2:             |----------|


# run parallel

run parallel <tarefa1> <tarefa2> 

Cria os processos sem esperar cada um terminar antes de criar o próximo.

tempo1: |----------|
tempo2: |----------|

Depois de criar os processos, o programa espera cada um terminar usando
waitpid().

# run pipe
run pipe <tarefa1> <tarefa2>
Liga a saída de uma tarefa à entrada da próxima.

Com duas tarefas:
tarefa1 -> pipe -> tarefa2

Com três:
tarefa1 -> pipe -> tarefa2 -> pipe -> tarefa3

Para três tarefas são necessários dois pipes.

O programa usa pipe() para criar esses canais e dup2() para ligar os
canais a entrada e a saída padrão dos processos.

Para uma tarefa que não é a primeira:

dup2(pipes[i - 1][0], STDIN_FILENO);

A entrada vem do pipe anterior.

Para uma tarefa que não é a última:

dup2(pipes[i][1], STDOUT_FILENO);

A saída vai para o próximo pipe.

Depois de fazer essas ligações, os descritores originais dos pipes são
fechados.

# start
start <tarefa>
Inicia a tarefa como um job.
processflow> start tempo1
[1] 12345

O 1 é o ID do job e 12345 é o PID do processo.

O programa registra essas informações na estrutura job.

# jobs
jobs
Consulta os jobs registrados.
O programa verifica o estado dos processos usando waitpid() com
WNOHANG, que permite fazer uma verificação sem bloquear imediatamente.

# wait
wait <jobId>
wait 1
Procura o job de ID 1 e espera o processo correspondente terminar.

# workdir
workdir <diretorio>
Exemplo:
workdir /tmp

O programa verifica primeiro se o caminho existe e se realmente é um
diretório. Depois, quando uma tarefa é executada, chdir() é usado no
processo filho para mudar seu diretório de trabalho.

# input
input <tarefa> <arquivo>
input ler entrada.txt
Faz a entrada padrão da tarefa vir do arquivo.

O programa abre o arquivo com open() e usa dup2() para colocá-lo no
lugar da entrada padrão.

# output
output <tarefa> <arquivo>
output listar resultado.txt
A saída da tarefa é enviada para o arquivo.
O arquivo é aberto com O_TRUNC, portanto seu conteúdo anterior é
substituído.

# append
append <tarefa> <arquivo>
append listar resultado.txt
A saída é adicionada ao final do arquivo, usando O_APPEND.

# exit
Encerra o ProcessFlow.
Como os processos são executados
O projeto utiliza fork() e execv() juntos.
fork() cria o processo filho:

processo principal
       |
       +---- filho

No filho, execv() substitui o programa que está sendo executado pelo
programa da tarefa.

exemplo:
task tempo1 /usr/bin/sleep 10

o vetor usado pelo execv() fica assim:

argumentos_exec[0] = "/usr/bin/sleep"
argumentos_exec[1] = "10"
argumentos_exec[2] = NULL

O NULL marca o fim da lista de argumentos.

Entrada e saída padrão

O programa trabalha com três descritores importantes:

STDIN_FILENO  -> entrada padrão
STDOUT_FILENO -> saída padrão
STDERR_FILENO -> saída de erro

O dup2() permite trocar para onde a entrada ou a saída padrão aponta.


# Redirecionamento

A função preparar_redirecionamento() concentra o tratamento de:
diretório de trabalho
entrada por arquivo
saída para arquivo
append

é chamada no processo filho antes de execv().

# Verificação de término
A função verificar_status() analisa como um processo terminou.
Quando o processo termina normalmente, WIFEXITED(status) é verdadeiro
e WEXITSTATUS(status) fornece o código de término.
Se o código for diferente de zero, o programa informa o código.
Também existe tratamento para processos que terminam por sinal, usando
WIFSIGNALED(status).

# Workflow

Também é possível executar comandos a partir de um arquivo .pf.

Uso:

./processflow arquivo.pf

Exemplo de arquivo:

task tempo1 /usr/bin/sleep 2
task listar /bin/ls -l
run tempo1
run listar
exit

O ProcessFlow lê as linhas do arquivo e executa os comandos na ordem em
que aparecem.

O exit é usado para indicar o encerramento do workflow.

## Limites

O programa define:

#define MAXIMO_DE_TAREFAS 10.

Cada tarefa possui espaço para até 10 argumentos.

O vetor usado para montar os argumentos do execv() possui 12 posições.
Isso permite guardar até 10 argumentos, além do programa na posição 0 e
do NULL que marca o final.

## Resumo dos comandos
task <nome> <programa> [args]     Cadastra uma tarefa
workdir <diretorio>               Define o diretório de trabalho
input <tarefa> <arquivo>          Redireciona a entrada
output <tarefa> <arquivo>         Redireciona a saída, substituindo o
append <tarefa> <arquivo>         Redireciona a saída adicionando ao
run <tarefa>                      Executa uma tarefa e espera
run sequential                    Executa tarefas em sequência
run parallel                      Executa tarefas em paralelo
run pipe                          Conecta tarefas por pipes
start <tarefa>                    Inicia uma tarefa como job
jobs                              Consulta os jobs
wait <jobId>                      Espera um job específico
exit                              Encerra o programa

