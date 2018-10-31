#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#define MAX 10000000
#define maxN 1005
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

int hash_function(char a[32]){
    int hash = 0;
    for (int i = 0; i < 32; i++) {
        if(a[i]>=65&&a[i]<=90){
            a[i]+=32;
        }
		hash = (41* hash + a[i])%prime;
    }
    return hash%prime;
}

void printRecord(struct dogType *dog){
    printf("------------------------\n");
    printf("Id: %i\n",dog->id);
	printf("Next: %i\n",dog->next);
	printf("Existe: %i\n",dog->existe);
    printf("Nombre: %s\n",dog->Name);
    printf("Tipo: %s\n", dog->Type);
    printf("Edad: %i años\n", dog->Age);
    printf("Raza: %s\n", dog->breed);
    printf("Estatura: %i cm\n", dog->height);
    printf("Peso: %3f Kg\n", dog->weight);
    printf("Sexo: %c\n", dog->gender);
	printf("Colision: %i\n",dog->colision);
    printf("------------------------\n");
}

struct dogType *hash_table[MAX];

char *names[maxN];//arreglo de tamaño 1005 de punteros para los nombres
char *namesRes[maxN];//arreglo de nombres restantes
int ids[maxN];
int freeIds[maxN];

char *types[] = {"perro","gato","tortuga","conejo",
"elefante","Caballo","gallina","hamster","vaca",
"ganso","oveja","loro","pavo","codorniz","gallo",
"pajaro","mono","jirafa","ardilla","iguana","aguila"};

char *breed[] = {"Akita","Azawakh","Basenji","Beagle","Bulldog",
 "Caniche","Chihuahua","Dogo","Foxhound","Galgo","Greyhound",
"Harrier","Husky", "Kishu","Komondor","Labrador","Landseer",
"Lebrel","Leonberger","Mudi","Pumi",};

void preMenu(){
    printf("Oprima [Enter] para Continuar\n");
    fflush(stdin);
	getchar();
}

void writeTable(int pos, struct dogType *pet){
	pet->next=pos+1005;
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
			//printRecord(pet);
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
				// printRecord(pet);
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
					printRecord(pet);
					free(temp);
					free(pet);
                }
            }
        }
    }
}

int isColitioned(int w,char name[32]){
  char buf[32];
  int res;
  FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
		res=0;
	}else{
    fseek(files, 0L, SEEK_END);
    long int wr = ftell(files);
		printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
		int ingresados;
		fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("Cantidad de registros:\t""%i\n",ingresados);
		printf("Cantidad de estructuras:\t""%i\n",totalRecords);

    if (w>totalRecords) {
			res=0;
		}else{

      do {
        fseek(files,(sizeof(int)+(sizeof(struct dogType)*(w-1))),SEEK_SET);
        fread(dog,sizeof(struct dogType),1,files);
        if (dog->Name==name) {
          res = 1;
        }else{
          w=dog->colision;
        }
        printf("id = %i\n",dog->id);
        printf("existe = %i\n",dog->existe);
      } while(dog->colision!=-1 && res!=0);

      if (res=0) {
        w=-w;
      }else{

      }

    }
    return w;
  }
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
			fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar-1))),SEEK_SET);
			memset(dog->Name,0,32);
			fread(dog,sizeof(struct dogType),1,files);
			printf("id = %i\n",dog->id);
			printf("existe = %i\n",dog->existe);
			if (dog->existe ==0) {
				res = 0;
			}else{
				printf("dog: %s\n",dog->Name);
				printf("dog: %s\n",name);
				int temp = equals(dog->Name,name);
				printf("%i\n",temp);
				if (temp) {
					res=2;
				}else{
          			if (dog->colision!=-1) {
            			return 1;
          			}else{
            			return -1;
          			}
        		}
			}
			printf("Nombre %s\n",dog->Name);

		}
		fclose(files);
		free(dog);
	}
	return res;
}

void init(){
	int i,j = 0;
    FILE *fileR = fopen("names.txt","r");//Abre el archivo en modo lectura
	/*FILE *fileW = fopen("namesIds2.txt","w");//Abrimos un archivo de escritura para los nombres y ids
	FILE *fileW3 = fopen("namesIdsRep.txt","w");//Abrimos un archivo de escritura solo con los id de fileW repetidos
	FILE *fileW4 = fopen("countSort.txt","w");*/
	char line[256]; //buffer de tamaño 256 para leer la linea de el archivo
    int arrId[1010];//arreglo para saber que pos estan libres
	int rep[maxN];
    for(i = 0; i<maxN;i++){//Reservo espacio para cada puntero e inicializo los arrId
        names[i] = malloc( 32 * sizeof(char));
        arrId[i] = 0;
		ids[i] = -1;
		rep[i] = -1;
    }
	for(;i<1010;i++){//Inicializo los arrId restantes en 0
		arrId[i] = 0;
	}
    i =0;//reinicio el contador
	int id;//el hash por cada nombre
    while(fgets(line, sizeof(line)-1,fileR)){//Mientras lea el archivo de nombres
        strtok(line, "\n"); //Los separa hasta que haya un salto de linea
        strcpy(names[i],line);//copia lo que esta en la linea y lo copia en el arreglo de punteros
		id = hash_function(names[i]);
        //printf("%i\n",hash_function(names[i]));//muestra en consola los id de cada nombre
        arrId[id]++;//aumenta el contador en la pos del hash
		//printf("%i %i\n",id,arrId[id]);
		ids[i]=id;
		if(arrId[id]>1){//quiere decir que ya hay un nombre con ese id
//			fprintf(fileW3,"%s %i \n",names[i],id);
//			fprintf(fileW3,"%s\n",names[i]);
			namesRes[j] = names[i];
			ids[i]=-1;
			rep[j]=i;
			j++;
		}
		i+=1;//aumenta contador
    }
//	fclose(fileW3);//Cierra el archivo namesPosIds
    fclose(fileR);//Cierra archivo de lectura
	FILE *freeId = fopen("freeIds.txt","w");
/*	FILE *fW3 = fopen("namesPosIds.dat","r");
	fseek(fW3,0L,SEEK_SET);
	int r;*/
	j = 0;
    for(i = 1; i<1005;i++){
        id = hash_function(names[i]);
		if(arrId[i]==0){
			freeIds[j]=i;
			fprintf(freeId, "%i\n",i);
			ids[rep[j]]=i;
			j++;
		}
//        fprintf(fileW, " %d %s \n", ids[i], names[i]);//escribimos en el archivo W1 los id y su nombre
		//fprintf(fileW, " %d %s \n", id, names[i]);//escribimos en el archivo W1 los id y su nombre
	//	fprintf(fileW4," %i\n",arrId[i]);

    }
	fclose(freeId);
	/*for(;i<1010;i++){
		//fprintf(fileW4," %i\n",arrId[i]);
	}
    fclose(fileW);
	fclose(fW3);
	fclose(fileW4);*/
}

int randNum(int lower, int upper){
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

float randNumFloat(int lower, int upper){
    float num = (float) (rand() % (upper - lower + 1)) + lower;
    return num;
}

char *randName(){
    int r = randNum(0,999);
    return names[r];
}

char *randType(){
    int r = randNum(0,20);
    return types[r];
}

int randAge(){
    return randNum(0,16);
}

char *randBreed(){
    int r = randNum(0,20);
    return breed[r];
}

int randHeight(){
    return randNum(10,25);
}

float randWeight(){
    return randNumFloat(1,5);
}

char randGender(){
    char g[2] = {'H','M'};
    int r = randNum(0,1);
    return g[r];
}

int equals(char petName[], char petName2[]){
	if(strlen(petName)!=strlen(petName2)){
		return 0;
	}else{
		int i=0;
		for(i;i<strlen(petName);i++){
			if(tolower(petName[i])!=tolower(petName2[i])){
				return 0;
			}
		}
	}
	return 1;
}

int hash_function2(char a[32]){
	char line[32];
	int r,i=0;
	FILE *lec=fopen("namesIdsRep.txt","r");
	while(fgets(line, sizeof(line)-1,lec)){
		strtok(line, "\n");
		r = equals(line,a);
		printf("%i\n",r);
		if(r){
			break;
		}
		i++;
	}
	fclose(lec);
	int sol;
	FILE *idLec=fopen("freeIds.txt","r");
	for(int j=0;j<=i;j++){
		fscanf(idLec,"%i\n",&sol);
	}
	fclose(idLec);
//	printf("sol: %i\n",sol);
	return sol;
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
			//preMenu();
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

void randomStruct(){
	struct dogType *pet;
	//se meten los datos en el archivo
	int i=0,idF;
	int counts=0;
	for(;i<1000;i++){
		pet=malloc(sizeof(struct dogType));
		memset(pet->Name,0,32);
		strcpy(pet->Name,names[i]);
        strcpy(pet->Type,randType());
        pet->Age = randAge();
        strcpy(pet->breed,randBreed());
        pet->height = randHeight();
        pet->weight = randWeight();
        pet->gender = randGender();
        pet->colision = -1;
		idF = hash_function(pet->Name);
   		int m = isFull(idF,pet->Name);
		printf("%i\n",m);
		if(m==0){//si es 0 no ha escrito nada
			printf("OK\n");
			pet->id = idF;
			pet->existe = 1;
			printRecord(pet);
			printf("%i\n",idF);
			writeTable(idF,pet);
			counts++;
		}
	}
	int id,idH2;
	for(i=0;i<(1000-counts);i++){
    	int e=0;
		id = hash_function(namesRes[i]);
		//printf("%s %i %i\n",namesRes[i],id,freeIds[i]);
		do {
		  id=getEndCol(id);
		  e++;
		} while(id>0);
    	id=-id;
		int fi = getFreeId();
		//refresh(freeIds[i],id);
		refresh(fi,id);
		pet=malloc(sizeof(struct dogType));
		memset(pet->Name,0,32);
		strcpy(pet->Name,namesRes[i]);
        strcpy(pet->Type,randType());
        pet->Age = randAge();
        strcpy(pet->breed,randBreed());
        pet->height = randHeight();
        pet->weight = randWeight();
        pet->gender = randGender();
		//pet->id = freeIds[i];
		pet->id = fi;
		pet->existe = 1;
    	printf("OKKKKKKKK %i\n",e);
		//writeTable(freeIds[i],pet);
		writeTable(fi,pet);
	// 	preMenu();
    // free(pet);
	}
  	printf("ingresados%i\n",counts);
	printf("Archivo de prueba generado exitosamente.\n");
}

int getEndCol(int beg){
  int ingre, totalRecords;
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
	}else{
		fseek(files, 0L, SEEK_END);
		long int wr = ftell(files);
		fseek(files, 0L, SEEK_SET);
	    int ingresados;
    	fread(&ingresados,sizeof(int),1,files);
		totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));


  fseek(files,(sizeof(int)+(sizeof(struct dogType)*(beg-1))),SEEK_SET);
  fread(dog,sizeof(struct dogType),1,files);

  if (dog->colision==-1) {
    int r=-beg;
    return r;
  }else{
    return dog->colision;
  }
}
}

void randomStructF(){
	FILE *files, *fileA;	
	struct dogType *dog, *pet;
	
	int i=1,idF,j = 0,offset =1005;
	int counts=0;
	for(;i<9999001;i++){
	//for(;i<1001;i++){
		files=fopen("dataDogs.dat","rb");
		dog=malloc(sizeof(struct dogType));
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

			if (j>totalRecords) {
				printf("Posicion erronea\n");
			}
			printf("j: 	%i\n",j);
			if(j>1){
				fseek(files,(sizeof(int)+(sizeof(struct dogType)*(j-1))),SEEK_SET);
			}else{
				fseek(files,(sizeof(int)+(sizeof(struct dogType)*(j))),SEEK_SET);
			}
			fread(dog,sizeof(struct dogType),1,files);
			if (dog->existe==0) {
				printf("Registro no existente\n");
			}else{
				printf("Nombre %s\n",dog->Name);
				printf("id %i\n",dog->id);
				printf("next %i\n",dog->next);
				printf("colision %i\n",dog->colision);
			
			}
			fseek(files,0L,SEEK_END);
			fclose(files);
		}
		pet=malloc(sizeof(struct dogType));	
		memset(pet->Name,0,32);
		strcpy(pet->Name,dog->Name);
		pet->id = (dog->id) + offset;
		pet->next = (pet->id)+1005;	
		pet->colision = (dog->colision)+offset;	
		pet->existe = dog->existe;

		free(dog);

		printf("Información mostrada exitosamente.\n");		
        strcpy(pet->Type,randType());
        pet->Age = randAge();
        strcpy(pet->breed,randBreed());
        pet->height = randHeight();
        pet->weight = randWeight();
        pet->gender = randGender();		
		fileA = fopen("dataDogs.dat","ab");
		/*if(i%1005==0){
			printRecord(pet);
			preMenu();		
		}	*/	
		fwrite(pet,sizeof(struct dogType),1,fileA);
		fclose(fileA);
		free(pet);		
		
		j++;
		if(i%1005==0){
			j=0;		
			offset += 1005;
		}

	}
	printf("Archivo de prueba generado exitosamente.\n");
}

int getColision(int count,int key){
  char buf[32];
  int res=1;
  int ingre;
  FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
	if(files==NULL){
		printf("Error abriendo archivo dataDogs.dat.\n");
		res=0;
	}else{
		fseek(files, 0L, SEEK_END);
		long int wr = ftell(files);
    	printf("tamleido%li\n",wr);
		fseek(files, 0L, SEEK_SET);
	    int ingresados;
    	fread(&ingresados,sizeof(int),1,files);
		int totalRecords=(int)(((wr-sizeof(int))/sizeof(struct dogType)));
		printf("Cantidad de registros:\t""%i\n",ingresados);
    	printf("Cantidad de estructuras:\t""%i\n",totalRecords);
		// fread(dog,sizeof(struct dogType),1,files);
		// printRecord(dog);
	  //   if (dog->existe==0) {
		// 	preMenu();
	  //       res=0;
	  //   }
    // 	while (res==1 && count<=totalRecords) {
		// 	count++;
    // 		fread(dog,sizeof(struct dogType),1,files);
		// 	printRecord(dog);
		// 	printf("counts= %i\n",count);
		// 	preMenu();
		//     if (dog->existe==0) {
		//         res=0;
		//     }
		// }
		/*do {
			count+=1;
			fread(dog,sizeof(struct dogType),1,files);
			printRecord(dog);
		    printf("counts= %i\n",count);
			preMenu();

	    } while(dog->existe==1);*/
	    printf("counts= %i\n",count);
		preMenu();
	    if (count>totalRecords) {
			printf("No se puede ingresar\n");
		    return -1;
    	}else{
			fseek(files, 0L, SEEK_SET);
			fread(&ingre,sizeof(int),1,files);
			FILE *files2=fopen("dataTemp.dat","wb+");
			if(files==NULL){
				printf("Error abriendo archivo dataTemp.dat.\n");
				return -1;
			}else{
				fwrite(&ingre,sizeof(int),1,files2);
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
    printf("FREEE ASISGNADO0 %i\n",count);
    return count;
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

void randomStruct2(){
	struct dogType *pet;
	//se meten los datos en el archivo
	int i=0,idF;
 	int colitions=0;
	for(;i<10e6;i++){
		pet=malloc(sizeof(struct dogType));
		memset(pet->Name,0,32);
		strcpy(pet->Name,randName());
        strcpy(pet->Type,randType());
        pet->Age = randAge();
        strcpy(pet->breed,randBreed());
        pet->height = randHeight();
        pet->weight = randWeight();
        pet->gender = randGender();
		idF = hash_function(pet->Name);
    idF = getKey(idF,pet->Name);
    int m = isFull(idF,pet->Name);
    while(m!=0){
      idF=idF+1005;
      m = isFull(idF,pet->Name);
    }
    pet->id= idF;
    writeTable(idF,pet);

	}
  	printf("colitions ingresadas%i\n",colitions);
	printf("Archivo de prueba generado exitosamente.\n");
}

int main(){
	init();
	randomStruct();
	randomStructF();
/*  for (int i = 0; i < 17; i++) {
    randomStruct2();
  }*/
    return 0;
}
