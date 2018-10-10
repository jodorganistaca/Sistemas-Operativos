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
    printf("------------------------\n");
}

struct dogType *hash_table[MAX];

char *names[maxN];//arreglo de tamaño 1005 de punteros para los nombres
char *namesRes[maxN];//arreglo de nombres restantes
int ids[maxN];

char *types[] = {"perro","gato","tortuga","conejo",
"elefante","Caballo","gallina","hamster","vaca",
"ganso","oveja","loro","pavo","codorniz","gallo",
"pajaro","mono","jirafa","ardilla","iguana","aguila"};

char *breed[] = {"Akita","Azawakh","Basenji","Beagle","Bulldog",
 "Caniche","Chihuahua","Dogo","Foxhound","Galgo","Greyhound",
"Harrier","Husky", "Kishu","Komondor","Labrador","Landseer",
"Lebrel","Leonberger","Mudi","Pumi",};

void preMenu(){
    printf("Oprima [Enter] para Ingresar\n");
    fflush(stdin);
	getchar();
}

void writeTable(int pos, struct dogType *pet){
	pet->next=pos+1000;
	printRecord(pet);
    long int wr=0;
    int tam;
    int ingre;
	preMenu();
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
        //fseek(files,wr,SEEK_SET);
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
        }else{
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
                    if (temp->next > pos) {
                      temp->next = pos;
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

int isFull(int buscar){
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
			fread(dog,sizeof(struct dogType),1,files);
			printf("id = %i\n",dog->id);
			printf("existe = %i\n",dog->existe);
			if (dog->existe ==0) {
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

void init(){
	int i,j = 0;
    FILE *fileR = fopen("names.txt","r");//Abre el archivo en modo lectura
	FILE *fileW = fopen("namesIds2.txt","w");//Abrimos un archivo de escritura para los nombres y ids
	FILE *fileW3 = fopen("namesIdsRep.txt","w");//Abrimos un archivo de escritura solo con los id de fileW repetidos
	FILE *fileW4 = fopen("countSort.txt","w");
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
			fprintf(fileW3,"%s\n",names[i]);
			namesRes[j] = names[i];
			ids[i]=-1;
			rep[j]=i;
			j++;
		}
		i+=1;//aumenta contador
    }
	fclose(fileW3);//Cierra el archivo namesPosIds
    fclose(fileR);//Cierra archivo de lectura    
	FILE *fW3 = fopen("namesPosIds.dat","r");
	fseek(fW3,0L,SEEK_SET);
	int r;
	j = 0;
    for(i = 0; i<1005;i++){
        id = hash_function(names[i]);					
        fprintf(fileW, " %d %s \n", ids[i], names[i]);//escribimos en el archivo W1 los id y su nombre  
		fprintf(fileW4," %i\n",arrId[i]);      
    }	
	for(;i<1010;i++){
		fprintf(fileW4," %i\n",arrId[i]);      
	}
    fclose(fileW);
	fclose(fW3);
	fclose(fileW4);
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

void randomStruct(){
	struct dogType* pet=malloc(sizeof(struct dogType));
	//se meten los datos en el archivo
	int i=10,idF;
	for(;i<1000;i++){
		strcpy(pet->Name,names[i]);
        strcpy(pet->Type,randType());
        pet->Age = randAge();
        strcpy(pet->breed,randBreed());
        pet->height = randHeight();
        pet->weight = randWeight();
        pet->gender = randGender();
		idF = ids[i];
		if(idF==-1){

		}else{
			pet->id = idF;
			pet->existe = 1;
//			printRecord(pet);
			printf("%i\n",idF);
			writeTable(idF,pet);
		}		
	}
	printf("Archivo de prueba generado exitosamente.\n");
}

int main(){
	init();
    randomStruct();
    return 0;
}
