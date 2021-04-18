#include <stdio.h>
#include <string.h>
#include <unistd.h>	   
#include <sys/types.h> 
#include <stdlib.h>

int main(int argc, char *argv[]){

	int fd[4][2];
	pid_t pid[4];
	int amount_pieces=1001, i, k, amount = 0, final=0, inventario=0, status;
	
	for(i = 0; i < 4; i++){
		if(pipe(fd[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	for(i=0;i<4;i++){
		pid[i] = fork();
		 if(pid[i]==0){
			if(i==0){	//M1
				close(fd[i+1][0]);
				for(k=1;k<amount_pieces;k++){
					if(k%5==0){
						printf("0 Foram cortadas e transferidas %d peças. \n",k);
						write(fd[i+1][1],&k, sizeof(int));
					}
				}
				close(fd[i+1][1]);
				exit(0);
			}else{
				if(i==1){	//M2
					close(fd[i][1]);
					close(fd[i+1][0]);
					int p=0;
					while(p<1000){
						read(fd[i][0],&amount,sizeof(int));
						if(amount%5==0){
							write(fd[i+1][1],&amount, sizeof(int));
							printf("1 Foram dobradas e transferidas %d peças. \n",amount);
							p=p+5;
						}
					}
					close(fd[i][0]);
					close(fd[i+1][1]);
					exit(0);
				}else{
					if(i==2){	//M3
						close(fd[i][1]);
						close(fd[i+1][0]);
						int p=0;
						while(p<1000){
							read(fd[i][0],&amount,sizeof(int));
							if(amount%10==0){
								write(fd[i+1][1],&amount, sizeof(int));
								printf("2 Foram fundidas e transferidas %d peças. \n",amount);
								p=p+10;
							}
						}
						close(fd[i][0]);
						close(fd[i+1][1]);
						exit(0);
					}else{
						if(i==3){	//M4
							close(fd[i][1]);
							close(fd[0][0]);
							int p=0;
							while(p<1000){
								read(fd[i][0],&amount,sizeof(int));
								if(amount%100==0){
									write(fd[0][1],&amount, sizeof(int));
									printf("3 Foram armazenadas e adicionadas a stock %d peças. \n",amount);
									p=p+100;
								}
							}
							close(fd[i][0]);
							close(fd[0][1]);
							exit(0);
						}
					}						
				}
			}
			exit(0);
		}
	}
	
	
	
	close(fd[0][1]);
	while((read(fd[0][0],&final,sizeof(int)))>0){
		inventario=final;
		printf("Inventário total a ser armazenado por M4: %d peças. \n", inventario);
	}
	close(fd[0][0]);
	printf("Inventário final %d. \n",inventario);
	
	return 0;
}
