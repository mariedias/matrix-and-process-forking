#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#define NUM_L 10
#define NUM_C 10

void iniciaMatriz(int matriz[NUM_L][NUM_C]){
int i,j;
        srand(time(NULL));
        for (i=0;i<NUM_L;i++){
                for (j=0;j<NUM_C;j++){
                        matriz[i][j] = rand()%10;
                }
        }
}

void imprimeMatriz(int matriz[NUM_L][NUM_C]){
int i,j;
        for (i=0;i<NUM_L;i++){
                for (j=0;j<NUM_C;j++){
                        printf("%-3d", matriz[i][j]);
                }
                printf("\n");
        }
}

void calculaTransposta(int matriz[NUM_L][NUM_C], int transposta[NUM_C][NUM_L]){
int i,j;
	for(i=0;i<NUM_L;i++){
		for(j=0;j<NUM_C;j++){
			transposta[j][i] = matriz[i][j];
		}
	}
}

int main() {
	int matriz[NUM_L][NUM_C];
	int transposta[NUM_C][NUM_L];
	
	iniciaMatriz(matriz);
	printf("Matriz original: \n");
       	imprimeMatriz(matriz);

	pid_t p =  fork(); /* Cria um novo processo */
	int status = 0;

	if(p < 0){
		/* Tratamento de erro */
		perror("Erro no fork!\n");
		exit(-1);
	}
	else if(p == 0){
		/* Processo filho */
		printf("Iniciando a execução do processo filho...\n");
		printf("Sou o filho de PPID = %d, tenho PID = %d\n", getppid(), getpid());
		printf("PID do filho (no filho): %d\n", p);

		/* Executa o comando curl para baixar o conteúdo da página*/
 	   	system("curl -s -o bbc.txt https://www.bbc.com/portuguese");

		/* Executa o comando egrep para encontrar a palavra no arquivo baixado e conta a quantidade */
		FILE *pipe = popen("egrep -o Oscar bbc.txt | wc -l", "r");
    		if (!pipe) {
        		perror("Erro ao executar o comando!\n");
        		exit(-1);
   	 	}

		/* Lê o valor retornado pelo comando wc -l */
    		int quantidade;
    		fscanf(pipe, "%d", &quantidade);
    	
    		pclose(pipe);

    		printf("Quantidade de vezes que a palavra 'Oscar' aparece na página 'https://www.bbc.com/portuguese': %d\n", quantidade);
		exit(0); /* Terminação de sucesso */
	}
	else{
		/* Processo pai */
		printf("Pai aguardando o filho terminar...\n");
		printf("Sou o pai - PID = %d\n", getpid());
		printf("PID do filho (no pai): %d\n", p);
		wait(&status);

		calculaTransposta(matriz, transposta);
		printf("Matriz transposta: \n");
       		imprimeMatriz(transposta);

		printf("Pai terminou! PID: %d, PPID (PID do pai): %d.\nStatus de terminação do filho = %s\n", getpid(), getppid(), (WEXITSTATUS(status) == 0 ? "Sucesso" : "Falha"));
		printf("Filho completou a execução!\n");
	
		exit(0); /* Terminação de sucesso */
	}

	return 0;
}
