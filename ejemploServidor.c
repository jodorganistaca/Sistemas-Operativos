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
#define PORT 9034   // puerto en el que escuchamos

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
		//int pow = (10,(i+1));
		//hash += ((a[i]%97)+1)*(tam%(pow));
		//hash += a[i]%97;
		hash = (41 * hash + a[i])%prime;
    }
    return hash%prime;
}

void preMenu(){
    printf("Oprima [Enter] para volver al menu\n");
    fflush(stdin);
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
		printf("Cantidad de registros:\t""%i\n",ingresados);
		printf("Cantidad de estructuras:\t""%i\n",totalRecords);

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
		printf("paso fseek END\n");
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
		printf("creo DataTemp de escritura lectura\n");
        struct dogType *temp;
        int tam = (int)(((wr-sizeof(int))/sizeof(struct dogType)));
        printf("tam leido %li\n",wr);
        printf("cantidad estructuras%i \n",tam);
        if(wr <= 0){///va a ingresar un registro por primera vez
            ///recorremos el archivo hasta la posicion y lo llenamos de estructuras Null
            printf("Entro a tam<=0\n");
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
            printf("OK\n");
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
					printf("Entro a pos<tam\n");
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


void refresh(int count,int key){
	int ingre, totalRecords;
	printf("ENTRO A REFRESH\n");
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
		printf("Cantidad de registros:\t""%i\n",ingresados);
    	printf("Cantidad de estructuras:\t""%i\n",totalRecords);
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
		printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
		int ingresados;
		fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("Cantidad de registros:\t""%i\n",ingresados);
		printf("Cantidad de estructuras:\t""%i\n",totalRecords);

    	memset(dog->Name,0,32);

		do {
		  fseek(files,(sizeof(int)+(sizeof(struct dogType)*(code-1))),SEEK_SET);
		  fread(dog,sizeof(struct dogType),1,files);
		  printf("nombreee %s\n",dog->Name);
		  int temp = equals(dog->Name,name);
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
    printf("ID ASISGNADO por el hash %s es %i\n",newDog->Name,h);
    newDog->existe=1;
    //En WriteTable si se llenan estos campos ya que se calculan
    int gD=getDir(h,newDog->Name);
    if (gD<0) {
		printf("ENTRO A H<0\n");
		h = gD;
		int fi=getFreeId();
		h=-h;
		refresh(fi,h);
		h=fi;
    }
    printf("ID ASIGNADO POR GETDIR %i\n",h);
    int m=isFull(h,newDog->Name);
    while (m!=0) {
      printf("m = %i\n",m);
      h=h+1005;
      printf("h + 1005 %i\n",h);
      m=isFull(h,newDog->Name);
    }
    newDog->colision=-1;
    newDog->id = h;
    newDog->next=h+1005;
    writeTable(h,newDog);
    printf("El id del registro%i\n",h);
    printf("registro hecho\n");
}

void seeRecord(int buscar,int cliente){
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
		return;
	}else{
		fseek(files, 0L, SEEK_END);
	    //fread(dog,sizeof(struct dogType),1,files);
		long int wr = ftell(files);
    	printf("tamleido%li\n",wr);
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
			fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar+1))),SEEK_SET);
		}else{
			fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar-1))),SEEK_SET);
		}
		fread(dog,sizeof(struct dogType),1,files);
		if (dog->existe==0) {
			printf("Registro no existente\n");
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
			/*char d[32];
			itoa(dog->id,d);
			printf("d= %s\n",d);
			char buffer[100];
			//strcat(b,path);
			strcat(d,c);
			FILE *files=fopen(d,"rb");
			if(files==NULL){
			}else{
				fscanf(files,"%s",buffer);
				send(cliente, buffer, sizeof(buffer), 0);
				fclose(files);
			}			
			fclose(files);*/
			printf("%s\n",b);
			send(cliente, b, sizeof(b), 0);
			//system(b);

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
		printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
		int ingresados;
		fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("Cantidad de registros:\t""%i\n",ingresados);
		printf("Cantidad de estructuras:\t""%i\n",totalRecords);

    	memset(dog->Name,0,32);

    do {
      fseek(files,(sizeof(int)+(sizeof(struct dogType)*(code-1))),SEEK_SET);
      fread(dog,sizeof(struct dogType),1,files);
      int temp = equals(dog->Name,name);
      if (temp==0) {
        code=dog->colision;
      }else{
        res=1;
      }
    } while(res!=1);
			printf("id = %i\n",dog->id);
			printf("existe = %i\n",dog->existe);

		fclose(files);
		free(dog);
  }
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
		printf("estructuras ingresadas%i\n",ingre);
		printf("estructuras encontradas%i\n",tam);
		printf("tam leido %li\n",wr);
		h=hash_function(n);
		h=getKey(h,n);
		printf("%i\n",h);
		do {
			if(h>1006){
				fseek(fp,(sizeof(int)+(sizeof(struct dogType)*(h+1))),SEEK_SET);
			}else{
				fseek(fp,(sizeof(int)+(sizeof(struct dogType)*(h-1))),SEEK_SET);
			}
  			fread(pet,sizeof(struct dogType),1,fp);
      		h+=1005;
			if (pet->existe==1) {
				//printRecord(pet);
				send(cliente, pet, sizeof(struct dogType), 0);
			}
		} while(pet->next==pet->id+1005 && h<=tam);
		pet->existe = 0;
		printRecord(pet);
		send(cliente, pet, sizeof(struct dogType), 0);
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
    myaddr.sin_addr.s_addr = INADDR_ANY;
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

