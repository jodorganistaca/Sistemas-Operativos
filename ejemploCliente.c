#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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
	int a=0;
	while (a!=-1) {
		
		printf("Ingrese 1 para enviar un mensaje \n");
		printf("Ingrese 2 para recibir un mensaje \n");
		scanf("%i",&a);

		if(a==1){
			char mensaje[1000];
			//scanf("%s", mensaje);
			struct dogType *newDog = malloc(sizeof(struct dogType));
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
			struct mensaje *mens = malloc(sizeof(struct mensaje));
			mens->dog = newDog;
			mens->option = 1;
			memcpy(mens->cadena,newDog->Name,32);
			mens->registro = 0;
			send(cliente, mens, sizeof(struct mensaje), 0);
			send(cliente, newDog, sizeof(struct dogType), 0);
			free(mens);
			free(newDog);
		}else{
		if(a==2) {

		int bytesRecibidos = recv(cliente, buffer, 1000, 0);
		if (bytesRecibidos <= 0) {
			perror("El chabón se desconectó o bla.");
			return 1;
		}
		buffer[bytesRecibidos] = '\0';
		printf("Me llegaron %d bytes con %s\n desde el servidor", bytesRecibidos, buffer);
		}

		}

	}

	return 0;
}
