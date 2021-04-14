#include <stdio.h>
#include <string.h>
#include <unistd.h>	   
#include <sys/types.h> 
#include <stdlib.h>	   

#define ARRAY_SIZE 6

typedef struct {
	char product_name [20];
	float price;
	int code;
} estr;

int main(int argc, char *argv[]){

	int fd[5][2];
	int info[1][2];
	pid_t pid;
	int i, k, barcode, barcode1, ind;
	
	if(pipe(info[0]) == -1){
			perror("Pipe failed");
			return 1;
	}
	
	for(i = 0; i < 5; i++){
		if(pipe(fd[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	for(i=0;i<5;i++){
		pid = fork();
		 if(pid==0){
			 close(info[0][0]);
			 close(fd[i][1]);
			 printf("Caixa %d - Escreva o codigo de barras (0, 1, 2, 3 ou 4):\n", i);
			 scanf("%d",&barcode);
			 write(info[0][1],&barcode,sizeof(barcode));
			 write(info[0][1],&i,sizeof(i));
			 estr ptr2;
			 read(fd[i][0],&ptr2, sizeof(ptr2));
			 printf("Nome do produto: %s. Preco: %.2f. \n",ptr2.product_name,ptr2.price);
			 close(info[0][1]);
			 close(fd[i][0]);
			 exit(0);
		 }
	}
	
	estr ptr[ARRAY_SIZE];
	
	stpcpy(ptr[0].product_name,"Arroz");
	ptr[0].price=3.0;
	ptr[0].code=4;
	
	stpcpy(ptr[1].product_name,"Massa");
	ptr[1].price=2.5;
	ptr[1].code=0;
	
	stpcpy(ptr[2].product_name,"Fiambre");
	ptr[2].price=1.5;
	ptr[2].code=1;
	
	stpcpy(ptr[3].product_name,"Azeite");
	ptr[3].price=0.5;
	ptr[3].code=2;
	
	stpcpy(ptr[4].product_name,"Coelho");
	ptr[4].price=10.5;
	ptr[4].code=3;
	
	close(info[0][1]);
	for(i=0;i<5;i++){
		read(info[0][0],&barcode1,sizeof(barcode));
		read(info[0][0],&ind,sizeof(i));
		close(fd[ind][0]);
		int vazio=0;
		for(k=0;k<ARRAY_SIZE;k++){
			if(barcode1 == ptr[k].code){
				write(fd[ind][1],&ptr[k],sizeof(ptr[k]));
				vazio=1;
			}
		}
		if(vazio==0){
			stpcpy(ptr[5].product_name,"Não existe");
			ptr[5].price=0;
			write(fd[ind][1],&ptr[5],sizeof(ptr[5]));
		}
	}
	close(fd[ind][1]);
	close(info[0][0]);
	
	return 0;
}
