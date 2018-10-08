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

struct dogType *hash_table[MAX];

char *names[maxN];//arreglo de tamaño 1005 de punteros para los nombres
int ids[maxN];

char *types[] = {"perro","gato","tortuga","conejo",
"elefante","Caballo","gallina","hamster","vaca",
"ganso","oveja","loro","pavo","codorniz","gallo",
"pajaro","mono","jirafa","ardilla","iguana","aguila"};

char *breed[] = {"Akita","Azawakh","Basenji","Beagle","Bulldog",
 "Caniche","Chihuahua","Dogo","Foxhound","Galgo","Greyhound",
"Harrier","Husky", "Kishu","Komondor","Labrador","Landseer",
"Lebrel","Leonberger","Mudi","Pumi",};

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

void writeTable(int pos, struct dogType *pet){
    FILE *files=fopen("dataDogs.dat","wb");
    if(files==NULL){
        printf("Error abriendo archivo dataDogs.dat.\n");
        return;
    }else{
        struct dogType *temp;
        fseek(files, 0L,SEEK_END);
        int tam = ftell(files)/sizeof(struct dogType);
        //printf("%i\n",tam);
        if(tam == 0){///va a ingresar un registro por primera vez
            ///recorremos el archivo hasta la posicion y lo llenamos de estructuras Null
            temp = malloc(sizeof(struct dogType));
            for(int rec = 1; rec < pos; rec++){
                temp->id = rec;
                temp->next = pos;
                fwrite(temp,sizeof(struct dogType),1,files);
            }
            fwrite(pet,sizeof(struct dogType),1,files);
            fclose(files);
            free(temp);
            free(pet);
        }else{
            if(pos>tam){///ingresa un registro dependiendo la poscion
                        ///en este caso mayor al tam del file actual
                ///Cerramos el archivo para escribirlo en un archivo nuevo
                fclose(files);
                files=fopen("dataDogs.dat","rb");
                fseek(files, 0L,SEEK_SET);
                FILE *filesNew = fopen("dataTemp.dat","wb");
                temp = malloc(sizeof(struct dogType));
                for(int rec = 1; rec < pos; rec++){//recorremos el archivo files escribiendo ahora en filesNew
                    if(rec<tam+1){//Dado que la posicion es menor reescribimos lo que ya teniamos previamente
                        fread(temp,sizeof(struct dogType),1,files);
                        fwrite(temp,sizeof(struct dogType),1,filesNew);
                    }else{//Escribimos los nulls nuevos
                        temp->id = rec;
                        temp->next = pos;
                        fwrite(temp,sizeof(struct dogType),1,filesNew);
                    }
                }
                //Escribimos en la posicion la mascota ingresada/borramos el archivo viejo y renombramos el nuevo
                fwrite(pet,sizeof(struct dogType),1,filesNew);
                fclose(files);
                remove("dataDogs.dat");
                fclose(filesNew);
                rename("dataTemp.dat","dataDogs.dat");
                free(temp);
                free(pet);
            }else{///ingresa un registro en una poscion,en este caso mayor al tam del file actual
                ///Cerramos el archivo para escribirlo en un archivo nuevo
                fclose(files);
                files=fopen("dataDogs.dat","rb");
                fseek(files, 0L,SEEK_SET);
                FILE *filesNew = fopen("dataTemp.dat","wb");
                temp = malloc(sizeof(struct dogType));
                for(int rec = 1; rec <= tam; rec++){//recorremos el archivo files escribiendo ahora en filesNew
                    if(rec<pos){//actualiza hasta la poscion previa al cual vamos a ingresar el archivo
                        temp->id = rec;
                        temp->next = pos;
                        fwrite(temp,sizeof(struct dogType),1,filesNew);
                    }else if(rec>pos){//Dado que la posicion es menor al tam reescribimos lo que ya teniamos previamente
                        fread(temp,sizeof(struct dogType),1,files);
                        fwrite(temp,sizeof(struct dogType),1,filesNew);
                    }else{//Escribimos la mascota en la posicion
                        fwrite(pet,sizeof(struct dogType),1,filesNew);
                    }
                }
                //borramos el archivo viejo y renombramos el nuevo
                fclose(files);
                remove("dataDogs.dat");
                fclose(filesNew);
                rename("dataTemp.dat","dataDogs.dat");
                free(temp);
                free(pet);
            }
        }
    }
}

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

int rehash(FILE *T1,FILE *T3,FILE *T4){
	T1 = fopen("namesIds.txt","r");
	T3 = fopen("freeIds.txt","r");
	T4 = fopen("namesPosIds.txt","r");
	fseek(T4, 0L,SEEK_END);
	int x = ftell(T4)/sizeof(int);
	//printf("%i\n",x);
	fseek(T4, 0L,SEEK_SET);
	for(int i=0; i<x; i++){
		//return T2() =
	}
	fclose(T3);
	fclose(T4);
	fclose(T1);
}

void init(){
	int i,j = 0;
    FILE *fileR = fopen("names.txt","r");//Abre el archivo en modo lectura
	FILE *fileW = fopen("namesIds.txt","w");//Abrimos un archivo de escritura para los nombres y ids
    FILE *fileW2 = fopen("freeIds.dat","w");//Abrimos un archivo de escritura solo los ids libres
	FILE *fileW3 = fopen("namesPosIds.dat","w");//Abrimos un archivo de escritura solo con los id de fileW repetidos
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
			fprintf(fileW3,"%i\n",i);
			//ids[id]=-1;
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
    char *freeId;
	j = 0;
    for(i = 0; i<1005;i++){
        id = hash_function(names[i]);				
		if(arrId[i]==0){//Si el arrId esta en 0 significa que no esta escrito una mascota en esta posicion
			fprintf(fileW2,"%i\n",i);//escribimos en el archivo W2 los ids libres
			/*if(rep[i]==-1){//quiere decir que ya acabo de leer los repetidos 
			
			}else{*/
				ids[rep[j]] = i;
				//printf("%i %i %i %i\n",i,ids[i],rep[j],ids[rep[i]]);
				j++;
			//}
		}		
        fprintf(fileW, " %d %s \n", ids[i], names[i]);//escribimos en el archivo W1 los id y su nombre  
		      
    }		
    fclose(fileW);
	fclose(fileW2);
	fclose(fW3);
	//rehash(fileW,fileW2,fileW3);	
	/*printf("--------\n");
	for(j = 0; j<maxN;j++){		
		printf("%i\n",rep[j]);
	}*/
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
    //printf("s1: %s\t s2: %s\n",petName,petName2);
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
	int i=0,idF;
	/*FILE *T3 = fopen("freeIds.txt","r");
	FILE *T4 = fopen("namesPosIds.txt","r");
	fseek(T4, 0L,SEEK_END);
	int x = ftell(T4)/sizeof(int);
	//printf("%i\n",x);
	fclose(T3);
	fclose(T4);*/
	for(;i<1000;i++){
		strcpy(pet->Name,names[i]);
        strcpy(pet->Type,randType());
        pet->Age = randAge();
        strcpy(pet->breed,randBreed());
        pet->height = randHeight();
        pet->weight = randWeight();
        pet->gender = randGender();
		idF = ids[i];
		pet->id = idF;
		pet->existe = 1;
		printRecord(pet);		
		/*int m=isFull(idF);
    	while (m==-1) {
	      printf("m = %i\n",m);
    	  idF=idF+1000;
    	  m=isFull(idF);
    	}
		*/
		//writeTable(idF,pet);
        //pet->id=pos;
	}
	printf("Archivo de prueba generado exitosamente.\n");
}

void printRecord(struct dogType *dog){
    printf("------------------------\n");
    //printf("Registro: %i %i\n", dog->record->first,dog->record->second);
    printf("Id: %i\n",dog->id);
    printf("Nombre: %s\n",dog->Name);
    printf("Tipo: %s\n", dog->Type);
    printf("Edad: %i años\n", dog->Age);
    printf("Raza: %s\n", dog->breed);
    printf("Estatura: %i cm\n", dog->height);
    printf("Peso: %3f Kg\n", dog->weight);
    printf("Sexo: %c\n", dog->gender);
    printf("------------------------\n");
}

int main(){
	init();
	/*printf("--------\n");
	for(int i = 0; i<maxN;i++){
		printf("%i\n",ids[i]);
	}
	printf("--------\n");*/
    randomStruct();
    return 0;
}
