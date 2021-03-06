#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "randomStruct.c"
#define max 1000

struct dogType{
    struct pair_int *record;
    char Name[32];
    char Type[32];
    int Age;
    char breed[16];
    int height;
    float weight;
    char gender[1];
    struct dogType *next;
};

struct pair_int{
    int first;
    int second;
};

int countRecords = 0;
struct dogType *hash_table[max];
struct dogType *next=NULL;
char prueba[32];

int hash_function(char a[32]){
  int hash = 0;
  for (int i = 0; i < 32; i++) {
    //printf("%i\t",a[i]);
    if(a[i]>=65&&a[i]<=90){
        a[i]+=32;
    }
    hash = (31 * hash + a[i])%max;
  }
  printf("\n");
  return hash;
}

struct dogType *createDog(){
  struct dogType *newDog = malloc(sizeof(struct dogType));
  if (newDog!=NULL) {
    printf("Por favor ingrese los datos pedidos a continuación\n"
           "ingrese nombre:\n"
           "Cuando haya terminado presione enter\n");
    memset(newDog->Name,32,32);
    scanf("%s", newDog->Name);
    printf("ingrese especie:\n"
           "Cuando haya terminado presione enter\n");
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
    scanf("%s", newDog->gender);
    newDog->next=NULL;
  }
  return newDog;
}

void loadDog(void *dog){
    struct dogType *newDog = dog;
    char n[32];
    memcpy(n,newDog->Name,32);
    memcpy(prueba,newDog->Name,32);
    int address = hash_function(n);
    struct dogType *pointer = hash_table[address];
    int addressSec = 0;
    if(pointer==NULL){
        printf("entro al null\n");
        hash_table[address] = newDog;
        newDog->record = malloc(sizeof(struct pair_int));
        newDog->record->first = address;
        newDog->record->second = addressSec;
    }else {
      printf("no es null\n");
      pointer=hash_table[address];
      addressSec+=1;
      while (pointer->next != NULL) {
        pointer = pointer->next;
        addressSec+=1;
      }
      pointer->next = malloc (sizeof(struct dogType));
      pointer->next = newDog;
      newDog->record = malloc(sizeof(struct pair_int));
      newDog->record->first = address;
      newDog->record->second = addressSec;
    }
    printf("registro hecho\n");
    countRecords++;
    preMenu();
}

void seeRecord(){
    printf("Cantidad de registros:\t"
           "%d\n",countRecords);
    printf("Por favor ingrese la dirección 1 de registro a ver\n");
    int record;
    scanf("%i",&record);
    printf("Por favor ingrese la dirección 2 de registro a ver\n");
    int record2;
    struct dogType *pointer = hash_table[record];
    scanf("%i",&record2);
    if(pointer==NULL){
        printf("paila no hay nothing\n");
    }else{
        int pos=0;
        while (pointer->next!=NULL && pos!=record2) {
          pointer=pointer->next;
          pos++;
        }
        if (pos==record2) {
          system("gedit dataDogs.dat");
          printRecord(pointer);
        }else{
          printf("Registro no existente\n");
        }
    }
    preMenu();
}
void deleteRecord(){
    printf("Cantidad de registros:\t"
           "%d\n",countRecords);
    printf("Por favor ingrese el numero de registro a borrar\n");
    int record;
    scanf("%i",&record);
    preMenu();
}

void searchRecord(){
    char n[32];
    printf("Por favor digite el nombre de su mascota\n"
           "Seguida la tecla ENTER\n");
    memset(n,32,32);
    scanf("%s",n);
    int addres = hash_function(n);
    printf("%i\n",addres);
    struct dogType *pointer = hash_table[addres];
    printRecord(pointer);
    while (pointer->next != NULL) {
        pointer = pointer->next;
        printRecord(pointer);
    }
    preMenu();
}

void printRecord(struct dogType *dog){
    printf("------------------------\n");
    printf("Registro: %i %i\n", dog->record->first,dog->record->second);
    printf("Nombre: %s\n",dog->Name);
    printf("Tipo: %s\n", dog->Type);
    printf("Edad: %i años\n", dog->Age);
    printf("Raza: %s\n", dog->breed);
    printf("Estatura: %i cm\n", dog->height);
    printf("Peso: %3f Kg\n", dog->weight);
    printf("Sexo: %s\n", dog->gender);
    printf("------------------------\n");
}

void preMenu(){
    char c;
    int p;
    printf("Oprima cualquier tecla para volver al menu\n"
           "seguida la tecla ENTER\n");
    do{
        char c2;
        scanf("%s", &c);
        p = (int)c;
    }while(p<0||p>255);
    menu();
}

//Menu principal
void menu(){
    printf("Por favor seleciona un numero de las sgtes opciones seguido de la tecla enter\n"
           "1. Ingresar registro\n"
           "2. Ver registro\n"
           "3. Borrar registro\n"
           "4. Buscar registro\n"
           "5. Salir\n");
    int option;
    do{
        scanf("%i",&option);
        bool x=(option>5 || option < 1);
        if(x){
            printf("Por favor seleciona un numero de las sgtes opciones seguido de la tecla enter\n"
           "1. Ingresar registro\n"
           "2. Ver registro\n"
           "3. Borrar registro\n"
           "4. Buscar registro\n"
           "5. Salir\n");
        }
    }while(option>5 || option < 1);
    struct dogType *newPet;
    switch(option){
        case 1:
            newPet = createDog();
            //memset(newPet->Name,' ',sizeof(struct dogType)-(sizeof(int)+sizeof(int)+sizeof(float)+sizeof(81)));
            loadDog(newPet);
            FILE *file = fopen("dataDogs.dat","w+");
            if(file == NULL){
                perror("error fopen");
                exit(-1);
            }
            int t = fwrite(newPet,sizeof(struct dogType),1,file);
            if(t == 0){
                perror("Error en fwrite");
                exit(-1);
            }
            int c = fclose(file);
            if(c!=0){
                perror("Error en fclose");
                exit(-1);
            }
            break;
        case 2:
            seeRecord();
            break;
        case 3:
            deleteRecord();
            break;
        case 4:
            searchRecord();
            break;
        case 5:
            return;
            break;
    };

}

int main(){
    //menu();
    int r,h;
    scanf("%i",&h);
    do{
        r = randAge();
        printf("%i\n",r);
        scanf("%i",&h);
    }while(h != 0);

    return 0;
}
