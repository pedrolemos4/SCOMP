#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define TAMANHO 5


typedef struct{
	char name[50];
	float price;
	int barcode;;
} product;

int main(int argc, char *argv[]){
	
	int numChilds=5;
	int numPipes=6;
	int k,i,j,flag,ind,barcode;
	int fd[numPipes*2];
	pid_t p;
	
	for(i = 0; i < numPipes; i++) {
		if(pipe(fd + i * 2) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}
	
	
	for(k=0;k<5;k++){
		p=fork();
		if(p==0){
			close(fd[0]);
			close(*(fd + (k+1) * 2 + 1));
			printf("Caixa %d - Escreva o codigo de barras (1, 2, 3, 4 ou 5):\n", k+1);
			scanf("%d", &barcode);
			write(fd[1],&barcode, sizeof(int));
			write(fd[1],&k, sizeof(int));
			product prod1;
			read((*(fd + (k+1) * 2)),&prod1, sizeof(product));
			printf("Caixa %d - Nome: %s\nPreco: %f\n", k+1, prod1.name, prod1.price);		
			close(fd[1]);
			close(*(fd + (k+1) *2));
			exit(0);
		}
	}
	
	product database[TAMANHO];
	strcpy(database[0].name, "Agua");
	database[0].price = 1.5;
	database[0].barcode = 1;
	strcpy(database[1].name, "Arroz");
	database[1].price = 1.99;
	database[1].barcode = 2;
	strcpy(database[2].name, "Massa");
	database[2].price = 2.20;
	database[2].barcode = 3;
	strcpy(database[3].name, "Atum");
	database[3].price = 1.19;
	database[3].barcode = 4;
	strcpy(database[4].name, "Batata");
	database[4].price = 3;
	database[4].barcode = 5;

	
	for(i = 0; i<5; i++){
		close(fd[1]);
		read(fd[0], &barcode,sizeof(int));
		read(fd[0], &ind , sizeof(int));
		close(*(fd + (ind+1) *2 ));
		flag = 0;
		for(j=0; j <TAMANHO; j++){
			if(barcode == database[j].barcode){
				write(*(fd + (ind+1) * 2 + 1),&database[j],sizeof(product));
				flag = 1;
			}
		}
		if(flag == 0){
			product prod2;
			strcpy(prod2.name,"Produto não existe na base de dados.");
			prod2.price=0;
			write(*(fd + (ind +1) * 2 +1),&prod2,sizeof(prod2));
		}
		close(*(fd + (ind+1)*2 +1));
	}

	return 0;
	
}
