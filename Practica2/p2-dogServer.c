
/*
** selectserver.c -- servidor de chat multiusuario
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>
#define PORT 9034   // puerto en el que escuchamos
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

void sendMessage(void* message, size_t len, int socketClient) {
	int bytesSent;
    if((bytesSent = send(socketClient, message, len, 0)) == -1) {
        perror("Error sending message.");
        return;
    }
    //#ifdef VERBOSE_MODE
    printf("Message sent.\n");
    //#endif // VERBOSE_MODE
}

void receiveMessage(void* message, size_t len, int socketClient) {
	int bytesRead;
    if((bytesRead = recv(socketClient, message, len, 0)) <= 0) {
        perror("Error reading bytes");
    }
}

void buildName(char* nm, int res, char* ans) {
    char it[12];
    sprintf(it, "%d", res);

    strcpy(ans, "registrosMedicos/");
    strcat(ans, it);
    strcat(ans, nm);
    strcat(ans, ".txt");
}

void sendFile(char* name, size_t len, int cliente) {
	printf("NAME: %s\n",name);
    sendMessage(name, len, cliente);
	FILE *tempFile;
    if((tempFile = fopen(name, "r")) == NULL) {
        char err = -1;
        sendMessage(&err, sizeof(char),cliente);
    } else {
        if(fseek(tempFile, 0, SEEK_END) != 0) {
            perror("File manipulation failed.");
            return;
        }

        long lenght = ftell(tempFile);
        rewind(tempFile);

        do {
            if(fread(buffer, (lenght < SIZE_CHUNK) ? lenght : SIZE_CHUNK, 1, tempFile) <= 0){
                perror("Error reading the file.");
                return;
            }

            sendMessage(buffer, (lenght < SIZE_CHUNK) ? lenght : SIZE_CHUNK,cliente);
            lenght -= SIZE_CHUNK;
        } while(lenght >= 0);

        #ifdef VERBOSE_MODE
        printf("File sent successfully\n");
        #endif // VERBOSE_MODE
        fclose(tempFile);
    }
}

void receiveFile(int socketClient) {
	int bytesRead;
    receiveMessage(&fileName, 100, socketClient);
    #ifdef VERBOSE_MODE
    printf("Receiving file %s\n", fileName);
    #endif // VERBOSE_MODE

    bool isEmptyFile;
    receiveMessage(&isEmptyFile, sizeof(bool), socketClient);

    if(isEmptyFile == true) {
        #ifdef VERBOSE_MODE
        printf("Empty file.\n");
        #endif // VERBOSE_MODE
        remove(fileName);
        return;
    }
	FILE *tempFile;
    if((tempFile = fopen(fileName, "w+")) == NULL) {
        perror("Error creating file.");
        return;
    }

    do {
        bzero(buffer, SIZE_CHUNK);
        bytesRead = recv(socketClient, buffer, SIZE_CHUNK, 0);
        fwrite(buffer, bytesRead, 1, tempFile);
    } while(bytesRead == SIZE_CHUNK);

    #ifdef VERBOSE_MODE
    printf("End of file receiving.\n");
    #endif // VERBOSE_MODE

    fclose(tempFile);
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
	printf("Existe: %i\n",dog->existe);
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

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

int isFull(int buscar,char name[32]){
	char buf[32];
	int res;
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
		res=0;
	}else{
		fseek(files, 0L, SEEK_END);
		//fread(dog,sizeof(struct dogType),1,files);
		long int wr = ftell(files);
		printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
		int ingresados;
		fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		/*printf("Cantidad de registros:\t""%i\n",ingresados);
		printf("Cantidad de estructuras:\t""%i\n",totalRecords);*/

		if (buscar>totalRecords) {
			res=0;
		}else{
			printf("buscando %i\n",buscar);
			if(buscar>1006){
				fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar+1))),SEEK_SET);
			}else{
				fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar-1))),SEEK_SET);
			}
			memset(dog->Name,0,32);
			fread(dog,sizeof(struct dogType),1,files);
		    printf("nombre %s\n",dog->Name);
			printf("id = %i\n",dog->id);
			printf("existe = %i\n",dog->existe);
			if (dog->existe==0) {
				res = 0;
			}else{
            			return -1;
			}
			printf("Nombre %s\n",dog->Name);

		}
		fclose(files);
		free(dog);
	}
	return res;
}

int equals(char petName[], char petName2[]){
	if(strlen(petName)!=strlen(petName2)){
		return 0;
	}else{
		for(int i=0;i<strlen(petName);i++){
			if(tolower(petName[i])!=tolower(petName2[i])){
				return 0;
			}
		}
	}
	return 1;
}

void writeTable(int pos, struct dogType *pet){
  if (pos>1006) {
    pos=pos+2;
  }
	pet->colision=-1;
    long int wr=0;
    int tam;
    int ingre;
    FILE *files2=fopen("dataDogs.dat","rb");
    if(files2==NULL){
        printf("Error abriendo archivo dataDogs.dat.\n");
    }else{
		fseek(files2, 0L,SEEK_END);
		//printf("paso fseek END\n");
		wr=ftell(files2);
		fseek(files2,0L,SEEK_SET);
		fread(&ingre,sizeof(int),1,files2);
		int tam = (int)(((wr-sizeof(int))/sizeof(struct dogType)));
		/*printf("estructuras ingresadas%i\n",ingre);
		printf("estructuras encontradas%i\n",tam);
		printf("tam leido %li\n",wr);*/
  	}
    FILE *files=fopen("dataTemp.dat","wb+");
	if(files==NULL){
        printf("Error abriendo archivo dataTemp.dat.\n");
        return;
    }else{
		printf("creo DataTemp de escritura lectura\n");
        struct dogType *temp;
        int tam = (int)(((wr-sizeof(int))/sizeof(struct dogType)));
        /*printf("tam leido %li\n",wr);
        printf("cantidad estructuras%i \n",tam);*/
        if(wr <= 0){///va a ingresar un registro por primera vez
            ///recorremos el archivo hasta la posicion y lo llenamos de estructuras Null
            //printf("Entro a tam<=0\n");
            fseek(files,0L,SEEK_SET);
            int f=1;
            fwrite(&f,sizeof(int),1,files);
            temp = malloc(sizeof(struct dogType));
            for(int rec = 1; rec < pos; rec++){
                temp->id = rec;
                temp->next = pos;
                temp->existe = 0;
                fwrite(temp,sizeof(struct dogType),1,files);
            }
            fwrite(pet,sizeof(struct dogType),1,files);
            fclose(files);
            rename("dataTemp.dat","dataDogs.dat");
            //printf("OK\n");
            free(temp);
			free(pet);
        }else{
			printf("%i\n",pos);
            if(pos>tam){///ingresa un registro dependiendo la poscion
                        ///en este caso mayor al tam del file actual
                        ///Cerramos el archivo para escribirlo en un archivo
                ingre++;
                fseek(files,0L,SEEK_SET);
                fwrite(&ingre,sizeof(int),1,files);
                temp = malloc(sizeof(struct dogType));
                for (int i = 1; i <= tam; i++) {
					fread(temp,sizeof(struct dogType),1,files2);
					fwrite(temp,sizeof(struct dogType),1,files);
                }

                printf("Entro a pos>tam\n");
                printf("Leyo %i estructuras ingresadas\n", ingre);
                for(int rec = tam+1; rec < pos; rec++){//recorremos el archivo files escribiendo ahora en filesNew
                  //Escribimos los nulls nuevos
                    temp->id = rec;
                    temp->next = pos;
                    temp->existe = 0;
                    fwrite(temp,sizeof(struct dogType),1,files);
                }
                // //Escribimos en la posicion la mascota ingresada/borramos el archivo viejo y renombramos el nuevo

                fwrite(pet,sizeof(struct dogType),1,files);
                fclose(files2);
                fclose(files);
                remove("dataDogs.dat");
                rename("dataTemp.dat","dataDogs.dat");
                free(temp);
                free(pet);
            }else{
				if (pos<tam) {
					//printf("Entro a pos<tam\n");
					ingre++;
					fseek(files,0L,SEEK_SET);
					fwrite(&ingre,sizeof(int),1,files);
					temp = malloc(sizeof(struct dogType));
					for (int i = 1; i < pos; i++) {
						fread(temp,sizeof(struct dogType),1,files2);
						if (temp->next < pos) {
							fwrite(temp,sizeof(struct dogType),1,files);
						}else{
							if (temp->next > pos && temp->existe==0) {
							  temp->next = pos;
							  fwrite(temp,sizeof(struct dogType),1,files);
							}else{
                				fwrite(temp,sizeof(struct dogType),1,files);
              				}
						}
					}
					fread(temp,sizeof(struct dogType),1,files2);
					fwrite(pet,sizeof(struct dogType),1,files);
					for (int rec = pos; rec < tam; rec++) {
						fread(temp,sizeof(struct dogType),1,files2);
						fwrite(temp,sizeof(struct dogType),1,files);
					}
					fclose(files2);
					fclose(files);
					remove("dataDogs.dat");
					rename("dataTemp.dat","dataDogs.dat");
					free(temp);
					free(pet);
                }
            }
        }
    }
}

int getFreeId(){
	FILE *readId = fopen("freeIds.txt","r");
	FILE *writeId = fopen("freeTemp.txt","w");
	int fi,temp;
	fscanf(readId,"%i\n",&fi);
	while(fscanf(readId,"%i\n",&temp)!=EOF){
		fprintf(writeId,"%i\n",temp);
	}
	fclose(readId);
	fclose(writeId);
	remove("freeIds.txt");
	rename("freeTemp.txt","freeIds.txt");
	return fi;
}

//fi h
void refresh(int count,int key){
	int ingre, totalRecords;
	printf("ingresando..\n");
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog;
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
	}else{
		fseek(files, 0L, SEEK_END);
		long int wr = ftell(files);
    	printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
	    int ingresados;
    	fread(&ingresados,sizeof(int),1,files);
		totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		/*printf("Cantidad de registros:\t""%i\n",ingresados);
    	printf("Cantidad de estructuras:\t""%i\n",totalRecords);*/
	}
	if (count>totalRecords) {
		printf("No se puede ingresar\n");
	    return ;
	}else{
		fseek(files, 0L, SEEK_SET);
		fread(&ingre,sizeof(int),1,files);
		FILE *files2=fopen("dataTemp.dat","wb+");
		if(files==NULL){
			printf("Error abriendo archivo dataTemp.dat.\n");
			return ;
		}else{
			fseek(files2, 0L, SEEK_SET);
			fwrite(&ingre,sizeof(int),1,files2);
		    dog=malloc(sizeof(struct dogType));
			for (int i = 1; i < key; i++) {
				//printf("ENTRO\n");
				fread(dog,sizeof(struct dogType),1,files);
				fwrite(dog,sizeof(struct dogType),1,files2);
		    }
			fread(dog,sizeof(struct dogType),1,files);
			dog->colision=count;
			printf("Nombre cambiado%s\n",dog->Name);
			fwrite(dog,sizeof(struct dogType),1,files2);
			for (int rec = key; rec < totalRecords; rec++) {
			    if(rec%1000000==0){
					printf(".\n");
				}
			    fread(dog,sizeof(struct dogType),1,files);
			    fwrite(dog,sizeof(struct dogType),1,files2);
			}
			fclose(files2);
			fclose(files);
			remove("dataDogs.dat");
			rename("dataTemp.dat","dataDogs.dat");
			free(dog);
  		}
	}
}


int getDir(int code,char name[32]){
	char buf[32];
	int res=0;
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
		res=0;
	}else{
		fseek(files, 0L, SEEK_END);
		//fread(dog,sizeof(struct dogType),1,files);
		long int wr = ftell(files);
		//printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
		int ingresados;
		fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		/*printf("Cantidad de registros:\t""%i\n",ingresados);
		printf("Cantidad de estructuras:\t""%i\n",totalRecords);
		*/
    	memset(dog->Name,0,32);

		do {
			fseek(files,(sizeof(int)+(sizeof(struct dogType)*(code-1))),SEEK_SET);
			fread(dog,sizeof(struct dogType),1,files);
			//printf("nombreee %s\n",dog->Name);
			int temp = equals(dog->Name,name);
			printf("temp: %i\n",temp);
			if (temp==0 && dog->colision!=-1) {
				code=dog->colision;
			}else{
				res=1;
			}
		} while(res!=1);

		if (equals(dog->Name,name)==0) {
		  code=-code;
		}
		printf("id = %i\n",dog->id);
		printf("existe = %i\n",dog->existe);

		fclose(files);
		free(dog);
  }
  return code;

}

void insertRecord(struct dogType *newDog){
	printRecord(newDog);
    int h = hash_function(newDog->Name);
    printf("%i\n",h);
   // preMenu();
    printf("ID ASISGNADO por el hash %s es %i\n",newDog->Name,h);
    newDog->existe=1;
    //En WriteTable si se llenan estos campos ya que se calculan
    h = getDir(h,newDog->Name);
    printf("gD %i\n",h);
    preMenu();
    if (h<0) {
		//printf("ENTRO A H<0\n");
		//h = gD;
		int fi=getFreeId();
		printf("%i\n",fi);
		//preMenu();
		h=-h;
		refresh(fi,h);
		h=fi;
    }
    printf("ID ASIGNADO POR GETDIR %i\n",h);
    int m=isFull(h,newDog->Name);
    printf("m%i\n",m);
    preMenu();
    int b = 0;
    if(m != 0){
		h--;
		b = 1;
	}
    while (m!=0) {
      printf("m = %i\n",m);
      h=h+1005;
      printf("h + 1005 %i\n",h);
      m=isFull(h,newDog->Name);
    }
    newDog->colision=-1;
    if(b){
		newDog->id = h+1;
	}else{
		newDog->id = h;
	}
    if(h<1005){
		newDog->next = h + 1005;
	}else{
		newDog->next = h+1006;
	}
    
    printf("lo vas a ingresar are you sure?\n");
    printf("%i\n",h);
    preMenu();
    writeTable(h,newDog);
    printf("El id del registro%i\n",h);
    printf("registro hecho\n");
}

void seeRecord(int  buscar,int cliente){
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
		return;
	}else{
		fseek(files, 0L, SEEK_END);
	    //fread(dog,sizeof(struct dogType),1,files);
		long int wr = ftell(files);
    	//printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
    	int ingresados;
    	fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("Cantidad de registros:\t""%i\n",ingresados);
    	printf("Cantidad de estructuras:\t""%i\n",totalRecords);
		
		send(cliente, &ingresados, sizeof(int), 0);
		send(cliente, &totalRecords, sizeof(int), 0);

		if (buscar>totalRecords) {
			printf("Posicion erronea\n");
		}
		if(buscar>1006){
			fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar))),SEEK_SET);
		}else{
			fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar-1))),SEEK_SET);
		}
		fread(dog,sizeof(struct dogType),1,files);
		printRecord(dog);
		send(cliente, dog, sizeof(struct dogType), 0);
		if (dog->existe==0) {
			printf("Registro no existente\n");
			char m[22] = "Registro no existente\n";
			//send(cliente, m, sizeof(m), 0);
		}else{
			printf("Nombre %s\n",dog->Name);
			printf("id %i\n",dog->id);
			printf("next %i\n",dog->next);
			printf("colision %i\n",dog->colision);
			char path[12];
			char b[5] = "nano ";
			itoa(dog->id,path);
			char c[4] = ".txt";
			printf("%s\n",path);
			printf("c= %s\n", c);
			printf("cliente: %d\n", cliente);			
			
			
			printf("%s\n",b);
			send(cliente, b, sizeof(b), 0);
			//char q;
			fflush(stdin);
			

		}
		char q;
		receiveMessage(&q,sizeof(q),cliente);
		printf("%c\n",q);
		if(q=='y'){
			/*itoa(dog->id,path);
			buildName(dog->Name, rec + 1, name);*/
			char *name = (char*) calloc(1, 100);
			char it[12];
			char *ans[12];
			memset(ans,0,12);
			sprintf(it, "%d", dog->id);
			printf("it: %s\n", it);
			strcat(ans, &it);
			
			strcat(ans, dog->Name);
			strcat(ans, ".txt");
			printf("nombre= %s\n",ans);
			sendFile(&ans, 100,cliente);		
			receiveFile(cliente);	
			free(name);
		}
		fseek(files,0L,SEEK_END);

		fclose(files);
	}
	free(dog);
	printf("Información mostrada exitosamente.\n");

}

void deleteRecord(int record){
	long int wr=0;
	int tam;
	int ingre;
	FILE *files2=fopen("dataDogs.dat","rb");
	if(files2==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
	}else{
		fseek(files2, 0L,SEEK_END);
		wr=ftell(files2);
		fseek(files2,0L,SEEK_SET);
		fread(&ingre,sizeof(int),1,files2);
		int tam = (int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("estructuras ingresadas%i\n",ingre);
		printf("estructuras encontradas%i\n",tam);
		printf("tam leido %li\n",wr);
	}
	FILE *files=fopen("dataTemp.dat","wb+");
	if(files==NULL){
		printf("Error abriendo archivo dataTemp.dat.\n");
		return;
	}else{
		struct dogType *temp;
		temp=malloc(sizeof(struct dogType));
		//fseek(files,wr,SEEK_SET);
		int tam = (int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("tam leido %li\n",wr);
		printf("cantidad estructuras%i \n",tam);

	    if(record>tam || record<1){///ingresa un registro dependiendo la poscion
                  ///en este caso mayor al tam del file actual
                  ///Cerramos el archivo para escribirlo en un archivo
			printf("Registro a eliminar no existente\n");
			return;
        }else{
			fseek(files2,(sizeof(int)+(sizeof(struct dogType)*(record-1))),SEEK_SET);
			fread(temp,sizeof(struct dogType),1,files2);
			if (temp->existe=0) {
				printf("Registro a eliminar no existente\n");
				free(temp);
				return;
			}
          	else{
		        fseek(files,0L,SEEK_SET);
		        fseek(files2,sizeof(int),SEEK_SET);
		        ingre--;
		        fwrite(&ingre,sizeof(int),1,files);
		        temp = malloc(sizeof(struct dogType));
		        if (record==tam) {
					for (int i = 1; i < tam; i++) {
						fread(temp,sizeof(struct dogType),1,files2);
						fwrite(temp,sizeof(struct dogType),1,files);
					}
					fread(temp,sizeof(struct dogType),1,files2);
					temp->existe=0;
					fwrite(temp,sizeof(struct dogType),1,files);
	            }else{

					for (int i = 1; i < record; i++) {
						fread(temp,sizeof(struct dogType),1,files2);
						fwrite(temp,sizeof(struct dogType),1,files);
					}
					fread(temp,sizeof(struct dogType),1,files2);
					temp->existe=0;
					fwrite(temp,sizeof(struct dogType),1,files);
					for (int rec = record; rec < tam; rec++) {
						fread(temp,sizeof(struct dogType),1,files2);
						fwrite(temp,sizeof(struct dogType),1,files);
              		}
            	}
		        fclose(files2);
		        fclose(files);
		        remove("dataDogs.dat");
		        rename("dataTemp.dat","dataDogs.dat");
		        printf("Registro eliminado con exíto\n");
		        free(temp);
			}
        }
	}
}

int getKey(int code,char name[32]){
	char buf[32];
	int res=0;
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
		res=0;
	}else{
		fseek(files, 0L, SEEK_END);
		//fread(dog,sizeof(struct dogType),1,files);
		long int wr = ftell(files);
		//printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
		int ingresados;
		fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		/*printf("Cantidad de registros:\t""%i\n",ingresados);
		printf("Cantidad de estructuras:\t""%i\n",totalRecords);*/

    	memset(dog->Name,0,32);

		do {
			fseek(files,(sizeof(int)+(sizeof(struct dogType)*(code-1))),SEEK_SET);
			fread(dog,sizeof(struct dogType),1,files);
			int temp = equals(dog->Name,name);
			printf("%s %s\n", dog->Name, name);
			printf("temp!!! %i\n", temp);
			if (temp==0) {
				code=dog->colision;
			}else{
				res=1;
			}
			if(code == -1){
				res=1;
			}
			printf("code: %i\n", code);
			preMenu();
		} while(res!=1);
		printf("id = %i\n",dog->id);
		printf("existe = %i\n",dog->existe);

		fclose(files);
		free(dog);
	}
	printf("%i\n",code);
	return code;
}

void searchRecord(char n[32],int cliente){
    long int wr=0;
    int tam;
    int ingre;
    int h;
    
    int found = 0;
    FILE *fp=fopen("dataDogs.dat","rb");
    struct dogType* pet;
    pet=malloc(sizeof(struct dogType));
    if(fp==NULL){
		printf("Error abriendo archivo dataDogs.dat\n");
		return;
	}else{
		fseek(fp, 0L,SEEK_END);
		wr=ftell(fp);
		fseek(fp,0L,SEEK_SET);
		fread(&ingre,sizeof(int),1,fp);
		int tam = (int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("estructuras ingresadas %i\n",ingre);
		printf("estructuras encontradas %i\n",tam);
		//printf("tam leido %li\n",wr);
		h=hash_function(n);
		printf("%i\n",h);
		preMenu();
		h=getKey(h,n);
		printf("get key: %i\n",h);
		preMenu();
		//h=h-1;
		if(h == -1){
			pet->existe = 0;
			printRecord(pet);
			send(cliente, pet, sizeof(struct dogType), 0);			
		}else{
			do {
				if(h>1006){
					fseek(fp,(sizeof(int)+(sizeof(struct dogType)*(h))),SEEK_SET);
				}else{
					fseek(fp,(sizeof(int)+(sizeof(struct dogType)*(h-1))),SEEK_SET);
				}
				fread(pet,sizeof(struct dogType),1,fp);
				h+=1005;
				if (pet->existe==1) {
					printRecord(pet);
					send(cliente, pet, sizeof(struct dogType), 0);
				}
			} while(pet->next==pet->id+1005 && h<=tam);
			pet->existe = 0;
			printRecord(pet);
			send(cliente, pet, sizeof(struct dogType), 0);
		}		
		
		
	}
    free(pet);
	fclose(fp);
}

void server(){
	fd_set master;   // conjunto maestro de descriptores de fichero
    fd_set read_fds; // conjunto temporal de descriptores de fichero para select()
    struct sockaddr_in myaddr;     // dirección del servidor
    struct sockaddr_in remoteaddr; // dirección del cliente
    int fdmax;        // número máximo de descriptores de fichero
    int listener;     // descriptor de socket a la escucha
    int newfd;        // descriptor de socket de nueva conexión aceptada
    char buf[256];    // buffer para datos del cliente
    int nbytes;
    int yes=1;        // para setsockopt() SO_REUSEADDR, más abajo
    int addrlen;
    int i, j;
    FD_ZERO(&master);    // borra los conjuntos maestro y temporal
    FD_ZERO(&read_fds);
    // obtener socket a la escucha
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    // obviar el mensaje "address already in use" (la dirección ya se está usando)
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,
                                                        sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    // enlazar
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(PORT);
    memset(&(myaddr.sin_zero), '\0', 8);
    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    // escuchar
    if (listen(listener, 32) == -1) {
        perror("listen");
        exit(1);
    }
    // añadir listener al conjunto maestro
    FD_SET(listener, &master);
    // seguir la pista del descriptor de fichero mayor
    fdmax = listener; // por ahora es éste
    // bucle principal
    for(;;) {
        read_fds = master; // cópialo
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }
        // explorar conexiones existentes en busca de datos que leer
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // ¡¡tenemos datos!!
                if (i == listener) {
                    // gestionar nuevas conexiones
                    addrlen = sizeof(remoteaddr);
                    if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr,
                                                             &addrlen)) == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // añadir al conjunto maestro
                        if (newfd > fdmax) {    // actualizar el máximo
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n", inet_ntoa(remoteaddr.sin_addr), newfd);
                    }
                } else {
                    // gestionar datos de un cliente
					struct mensaje *mens = malloc(sizeof(struct mensaje));
					if((nbytes = recv(i, mens, sizeof(struct mensaje), 0)) <= 0){
						if(nbytes == 0){
							// conexión cerrada
                            printf("selectserver: socket %d hung up\n", i);
						}else{
							perror("recv");
						}
						close(i); // bye!
                        FD_CLR(i, &master); // eliminar del conjunto maestro
					}else{
						FILE *f;
						f = fopen("serverDogs.log", "a+"); 
						if (f == NULL) { /* Something is wrong   */}					

						char buff[20];
						struct tm *sTm;

						time_t now = time (0);
						sTm = gmtime (&now);

						strftime (buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);		
				
						fprintf (f,"|%s| |%s| |%i| |%i| |%s|\n", 
						buff, inet_ntoa(remoteaddr.sin_addr),mens->option,mens->registro,mens->cadena);
						fclose(f);
						printf ("|%s| |%s| |%i| |%i| |%s|\n", 
						buff, inet_ntoa(remoteaddr.sin_addr),mens->option,mens->registro,mens->cadena);
						switch(mens->option){
							case 1:
								;
								struct dogType *newDog = malloc(sizeof(struct dogType));
								if((nbytes = recv(i, newDog, sizeof(struct dogType), 0)) <= 0){
									if(nbytes == 0){
										// conexión cerrada
							            printf("selectserver: socket %d hung up\n", i);
									}else{
										perror("recv");
									}
									close(i); // bye!
							        FD_CLR(i, &master); // eliminar del conjunto maestro
								}else{
									printRecord(newDog);
									insertRecord(newDog);	
									free(newDog);
								}
								break;
							case 2:
								seeRecord(mens->registro,i);
								break;
							case 3:
								deleteRecord(mens->registro);
								break;
							case 4:
								searchRecord(mens->cadena,i);
								break;
						}
						for(j = 0; j <= fdmax; j++) {
                            // ¡enviar a todo el mundo!
                            if (FD_ISSET(j, &master)) {
                                // excepto al listener y a nosotros mismos
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
						free(mens);
						
					}
					

                    
                } // Esto es ¡TAN FEO!
            }
        }
    }

}


int main(void){    
	server();
    return 0;
}
