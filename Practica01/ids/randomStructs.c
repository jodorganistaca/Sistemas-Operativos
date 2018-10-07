#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#define max 1005
#define prime 10009

struct dogType{
    int id; //Posicion en la cual esta guardada el registro la cual se calcula con la funcion hash(Name)
    int next;//Posicion en la cual se guardaria otro registro si se tiene el mismo nombre (colision) calculado con la funcion reHash(Name)
    char Name[32];
    char Type[32];
    int Age;
    int height;
    char breed[16];
    float weight;
    char gender[1];

};

char *names[1000];//arreglo de tamaño 1000 de punteros para los nombres

char *types[] = {"perro","gato","tortuga","conejo",
"elefante","Caballo","gallina","hamster","vaca",
"ganso","oveja","loro","pavo","codorniz","gallo",
"pajaro","mono","jirafa","ardilla","iguana","aguila"};

char *breed[] = {"Akita","Azawakh","Basenji","Beagle","Bulldog",
 "Caniche","Chihuahua","Dogo","Foxhound","Galgo","Greyhound",
"Harrier","Husky", "Kishu","Komondor","Labrador","Landseer",
"Lebrel","Leonberger","Mudi","Pumi",};

int fillName(){
    int i;
    for(i = 0; i<1000;i++){
        names[i] = malloc( 32 * sizeof(char));
    }
    char *str = (char *) malloc(sizeof(char) * 32);
    FILE *file;
    file = fopen("names.txt", "r");
    char line[256];
    i =0;
    while(fgets(line, sizeof(line)-1,file)){
        strtok(line, "\n");
        /*printf("%s\n",line);*/
        strcpy(names[i],line);
        i+=1;
    }
    fclose(file);
  //  return 0;
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
    printf("s1: %s\t s2: %s\n",petName,petName2);
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
    fillName();
	FILE *fp;
	struct dogType* pet=malloc(sizeof(struct dogType));
	fp=fopen("dataDogs.dat","ab");
	if(fp==NULL){
		printf("Error abriendo dataDogs.dat.\n");
		return;
	}
	char petnames[1000][32];
	FILE *fnew = fopen("names.txt", "r");
	if(fnew==NULL){
		printf("Error abriendo nombresMascotas.txt.\n");
		return;
	}
	//se pasan los datos del archivo a una matriz dentro del programa
	int counter = 0;
	while(!feof(fnew)){
		fscanf(fnew, "%s", petnames[counter]);
		counter++;
	}/**/
	//se meten los datos en el archivo
	for(int i=0;i<1000;i++){
		int randindex = rand()%1000;
		strcpy(pet->Name,randName());
        strcpy(pet->Type,randType());
        pet->Age = randAge();
        strcpy(pet->breed,randBreed());
        pet->height = randHeight();
        pet->weight = randWeight();
        pet->gender = randGender();
	fseek(fp, 0L, SEEK_END);
        int size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        int pos=size/sizeof(struct dogType)+1;
        pet->id=pos;
        fwrite(pet,sizeof(struct dogType),1,fp);
	}
	fclose(fp);
	printf("Archivo de prueba generado exitosamente.\n");
}

/*
void printRecord(struct dogType *dog){
    printf("------------------------\n");
   // printf("Registro: %i %i\n", dog->record->first,dog->record->second);
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
*/

int main(){
    randomStruct();
    return 0;
}
