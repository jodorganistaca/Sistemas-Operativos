#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TAM 1005
#define prime 1009

struct dogType{
    int id; //Posicion en la cual esta guardada el registro la cual se calcula con la funcion hash(Name)
    int next;//Posicion en la cual se guardaria otro registro si se tiene el mismo nombre (colision) calculado con la funcion reHash(Name)
    int existe;
    char Name[32];
    char Type[32];
    int Age;
    int height;
    char breed[16];
    float weight;
    char gender;
    int colision;
};

struct mensaje{
	struct dogType *dog;
	int option;
	char cadena[32];
	int registro;
};

int hash_function(char a[32]){
    int hash = 0;
    for (int i = 0; i < 32; i++) {
        if(a[i]>=65&&a[i]<=90){
            a[i]+=32;
        }
		hash = (41 * hash + a[i])%prime;
    }
    return hash%prime;
}

int menu(){
    int option;
    do{
        printf("Por favor seleciona un numero de las sgtes opciones seguido de la tecla enter\n"
           "1. Ingresar registro\n"
           "2. Ver registro\n"
           "3. Borrar registro\n"
           "4. Buscar registro\n"
           "5. Salir\n");
        scanf("%i",&option);
        switch(option){
            case 1:
				return 1;
                break;
            case 2:
				return 2;
                break;
            case 3:
				return 3;
                break;
            case 4:
				return 4;
                break;
        };
    }while(option!=5);
	return -1;
}

void insertRecord(struct dogType *newDog){
	printf("Por favor ingrese los datos pedidos a continuación\n"
		"ingrese nombre:\n"
		"Cuando haya terminado presione enter\n");
	memset(newDog->Name,32,32);
	char n[32];
	scanf("%s", newDog->Name);
	printf("ingrese especie:\n"
		"Cuando haya terminado presione enter\n");
	// memcpy(n,newDog->Name,32);
	scanf("%s", newDog->Type);
	printf("ingrese edad:\n"
		"Cuando haya terminado presione enter\n");
	scanf("%i", &newDog->Age);
	printf("ingrese raza:\n"
		"Cuando haya terminado presione enter\n");
	scanf("%s", newDog->breed);
	printf("Ingrese estatura:\n"
		"Cuando haya terminado presione enter\n");
	scanf("%i", &newDog->height);
	printf("Ingrese peso:\n"
		"Cuando haya terminado presione enter\n");
	scanf("%f", &newDog->weight);
	printf("ingrese genero:\n"
		"Cuando haya terminado presione enter\n");
	do{
		scanf("%c",&newDog->gender);
		if(newDog->gender!='m'&&newDog->gender!='f'){
			printf("Sexo inválido, solo se permite m o f.\n");
			continue;
		}
		break;
	}while(1);	
}

int main(void) {
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	// direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(9034);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}

	char* buffer = malloc(1000);
	int o=0;
	while (o!=-1) {		
		o = menu();
		switch(o){
			case 1:
				;
				struct dogType *newDog = malloc(sizeof(struct dogType));
				insertRecord(newDog);
				struct mensaje *mens = malloc(sizeof(struct mensaje));
				mens->dog = newDog;
				mens->option = 1;
				memcpy(mens->cadena,newDog->Name,32);
				mens->registro = 0;
				send(cliente, mens, sizeof(struct mensaje), 0);
				free(mens);
				free(newDog);
				break;
			case 2:
				;
				struct dogType *newDog2 = malloc(sizeof(struct dogType));
				struct mensaje *mens2 = malloc(sizeof(struct mensaje));
				int buscar=0;
				scanf("%i",&buscar);
				mens2->dog = newDog2;
				mens2->option = 2;
				memcpy(mens2->cadena,newDog2->Name,32);
				mens2->registro = buscar;
				send(cliente, mens2, sizeof(struct mensaje), 0);
				free(mens2);
				free(newDog2);
				break;
			case 3:
				;
				struct dogType *newDog3 = malloc(sizeof(struct dogType));
				struct mensaje *mens3 = malloc(sizeof(struct mensaje));
				int eliminar=0;
				scanf("%i",&eliminar);
				mens3->dog = newDog2;
				mens3->option = 3;
				memcpy(mens3->cadena,newDog3->Name,32);
				mens3->registro = eliminar;
				send(cliente, mens3, sizeof(struct mensaje), 0);
				free(mens3);
				free(newDog3);
				break;
			case 4:
				;
				struct dogType *newDog4 = malloc(sizeof(struct dogType));
				struct mensaje *mens4 = malloc(sizeof(struct mensaje));
				printf("Por favor ingrese los datos pedidos a continuación\n"
				"ingrese nombre:\n"
				"Cuando haya terminado presione enter\n");
				memset(newDog4->Name,32,32);
				scanf("%s", newDog4->Name);
				mens4->dog = newDog2;
				mens4->option = 4;
				memcpy(mens4->cadena,newDog4->Name,32);
				mens4->registro = 0;
				send(cliente, mens4, sizeof(struct mensaje), 0);
				free(mens4);
				free(newDog4);
				break;
			case 5:
				break;
		}		

	}

	return 0;
}
