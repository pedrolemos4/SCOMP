
typedef struct{
	int number;
	char name[50];
	char address[50];
} user;

typedef struct{
	user users[100];
	int posicao;
} sharedStruct;
