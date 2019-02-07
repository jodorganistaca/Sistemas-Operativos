#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define SIZE_CHUNK 1024
#define TAM 1005
#define prime 1009

char buffer[1024];
char fileName[100];

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

void preMenu(){
    printf("Oprima [Enter] para volver al menu\n");
    fflush(stdin);
	getchar();
	getchar();
}

void printRecord(struct dogType *dog){
	printf("------------------------\n");
	printf("Colision: %i\n",dog->colision);
	printf("Id: %i\n",dog->id);
	printf("Nombre: %s\n",dog->Name);
	printf("Tipo: %s\n", dog->Type);
	printf("Edad: %i años\n", dog->Age);
	printf("Raza: %s\n", dog->breed);
	printf("Estatura: %i cm\n", dog->height);
	printf("Peso: %3f Kg\n", dog->weight);
	printf("Sexo: %c\n", dog->gender);
	printf("next: %i\n",dog->next);
	printf("------------------------\n");
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
			case 6:
				return 6;
				break;
        };
    }while(option!=5);
	return -1;
}

void insertRecord(struct dogType *newDog){
	printf("Por favor ingrese los datos pedidos a continuación\n"
		"ingrese nombre:\n"
		"Cuando haya terminado presione enter\n");
	memset(newDog->Name,0,32);
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

void sendMessage(void* message, size_t len, int socketClient) {
	int bytesSent;
    if((bytesSent = send(socketClient, message, len, 0)) == -1) {
        perror("Error sending message.");
        return;
    }
}

void receiveMessage(void* message, size_t len, int socketClient) {
	int bytesRead;
    if((bytesRead = recv(socketClient, message, len, 0)) <= 0) {
        perror("Error reading bytes");
    }
}

char* receiveFile(int socketClient) {
    #ifdef VERBOSE_MODE
    printf("Reading the file: ");
    #endif // VERBOSE_MODE
    receiveMessage(&fileName, sizeof(fileName),socketClient);

    #ifdef VERBOSE_MODE
    printf("%s\n", fileName);
    #endif // VERBOSE_MODE
    int bytesRead;
	FILE *file;
    if((file = fopen(fileName, "w+")) == NULL) {
        perror("Error creating file.");
    }

    do {
        bzero(buffer, SIZE_CHUNK);
        bytesRead = recv(socketClient, buffer, SIZE_CHUNK, 0);
        if(buffer[0] == -1){
            printf("Empty file\n");
            break;
        }
        fwrite(buffer, bytesRead, 1, file);
    } while(bytesRead == SIZE_CHUNK);

    #ifdef VERBOSE_MODE
    printf("End file read\n");
    #endif // VERBOSE_MODE

    fclose(file);

    return fileName;
}

void sendFileUpdated(char *name, size_t len, int client) {
    sendMessage(name, len, client);    

    bool isEmptyFile = true;

    char *content = NULL;
    FILE *file;
    if((file = fopen(name, "r")) == NULL) {
        perror("File does not exists.");
        sendMessage(&isEmptyFile, sizeof(bool), client);
    } else {
        if(fseek(file, 0, SEEK_END) != 0) {
            perror("File manipulation failed.");
            return;
        }

        //sendMessage(&isEmptyFile, sizeof(bool));

        int lenght = ftell(file);
        rewind(file);

        if(fread(buffer, (lenght < SIZE_CHUNK) ? lenght : SIZE_CHUNK, 1, file) <= 0){
            perror("Error reading the file. (out loop)");
            sendMessage(&isEmptyFile, sizeof(bool), client);
            return;
        }

        isEmptyFile = false;
        sendMessage(&isEmptyFile, sizeof(bool), client);

        sendMessage(buffer, (lenght < SIZE_CHUNK) ? lenght : SIZE_CHUNK, client);
        lenght -= SIZE_CHUNK;

        while(lenght >= 0) {
            if(fread(buffer, (lenght < SIZE_CHUNK) ? lenght : SIZE_CHUNK, 1, file) <= 0){
                perror("Error reading the file. (in loop)");
                return;
            }

            sendMessage(buffer, (lenght < SIZE_CHUNK) ? lenght : SIZE_CHUNK, client);
            lenght -= SIZE_CHUNK;
        }

        #ifdef VERBOSE_MODE
        printf("File sent successfully\n");
        #endif // VERBOSE_MODE
    }
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
				printRecord(mens->dog);
				
				send(cliente, mens, sizeof(struct mensaje), 0);
				send(cliente, newDog, sizeof(struct dogType), 0);
				
				free(mens);
				free(newDog);
				preMenu();
				break;
			case 2:
				;
				struct dogType *newDog2 = malloc(sizeof(struct dogType));
				
				struct mensaje *mens2 = malloc(sizeof(struct mensaje));
				int buscar=0;
				printf("Por favor digite el registro a ver seguido de la tecla ENTER:\n");
				scanf("%i",&buscar);
				printf("%i\n",buscar);
				mens2->dog = newDog2;
				mens2->option = 2;
				memcpy(mens2->cadena,newDog2->Name,32);
				mens2->registro = buscar;
				int ingresados, totalRecords;
				send(cliente, mens2, sizeof(struct mensaje), 0);
				recv(cliente, &ingresados, sizeof(int), 0);
				printf("Cantidad de registros:\t""%i\n",ingresados);
				recv(cliente, &totalRecords, sizeof(int), 0);
				printf("Cantidad de estructuras:\t""%i\n",totalRecords);
				char b[5];
				struct dogType *petSee = malloc(sizeof(struct dogType));
				int nb;
				if((nb = recv(cliente, petSee, sizeof(struct dogType), 0)) > 0){
					printRecord(petSee);
					printf("%i\n",nb);
					if(petSee->existe == 0){
						free(petSee);
						break;					
					}
				}
				free(petSee);
				recv(cliente, b, sizeof(b), 0);
				char q;
				
				fflush(stdin);
				printf("oprima y / n para abrir el historial.\n");
				do{
					scanf("%c",&q);
					if(q!='y'&&q!='n'){
						printf("opcion invalida, solo se permite y / n.\n");
						continue;
					}
					break;
				}while(1);
		        if(q=='y'){
					
					system(b);
				}
				sendMessage(&q,sizeof(q),cliente);
				free(mens2);
				free(newDog2);
				preMenu();
				break;
			case 3:
				;
				struct dogType *newDog3 = malloc(sizeof(struct dogType));
				struct mensaje *mens3 = malloc(sizeof(struct mensaje));
				int eliminar=0;
				printf("Por favor digite el registro a eliminar seguido de la tecla ENTER:\n");
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
				printf("Por favor digite el nombre de su mascota\n"
				"Cuando haya terminado presione enter\n");
				memset(newDog4->Name,0,32);
				scanf("%s", newDog4->Name);
				mens4->dog = newDog2;
				mens4->option = 4;
				memcpy(mens4->cadena,newDog4->Name,32);
				mens4->registro = 0;
				send(cliente, mens4, sizeof(struct mensaje), 0);
				free(mens4);
				free(newDog4);
				struct dogType *petR = malloc(sizeof(struct dogType));
				int nbytes;
				while((nbytes = recv(cliente, petR, sizeof(struct dogType), 0)) > 0){					
					printf("%i\n",nbytes);
					if(petR->existe == 0){
						free(petR);
						break;					
					}else{
						printf("%i\n",petR->existe);
						printRecord(petR);
					}
				}
				free(petR);
				break;
			case 5:
				break;
			case 6:
                ;
                char nombre[32];
                printf("Ingrese el nombre de la mascota a calcular la hash Function\n");
                memset(nombre,0,32);
                scanf("%s",nombre);
                printf("%s\n",nombre);
                int h = hash_function(nombre);
                printf("%i\n", h);
                break;

		}		

	}

	return 0;
}
