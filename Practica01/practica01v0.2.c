#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
//#include "randomStruct.c"
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

/*int countRecords = 0;
struct dogType *hash_table[max];*/
struct dogType *next=NULL;

struct registro{
    struct dogType *hash_table[max];
    int sizes[max];
    int countRecords;
};
struct registro* newArray( ){
    struct registro *table = malloc( sizeof( struct registro ) );
    return table;
}

void readTable(struct registro *reg, char *filePath) {
    printf("%s\n",filePath);
    FILE *reading = fopen(filePath,"r");
    if(!reading){
        perror("error abriendo el archivo: ");
    }
    printf("alright x5.2\n");
    int i = 0;
    for (; i < max; i++){
        if (&reg->hash_table[i]==NULL) {
            printf("entro al null ReadTable\n");
            reg->hash_table[i] = malloc(sizeof(struct dogType));
        }

        int readedSize;

        struct dogType *pointer=reg->hash_table[i], temp;
        /*char n[32];
        memcpy(n,pointer->Name,32);
        printf("%s\n",n);*/

        fread(&readedSize,sizeof(int),1,reading);
        //memcpy(&temp,pointer, sizeof(struct dogType));
        while (readedSize-- > 1) {
            pointer->next = malloc(sizeof(struct dogType));
            fread(&temp,sizeof(struct dogType),1,reading);
            pointer=pointer->next;
            memcpy(pointer,&temp,sizeof(struct dogType));
        }
    }
    fclose(reading);
}

int writeTable( struct registro *reg,char *filePath){
    int written = 0;
    struct dogType *dogCurrent=reg->hash_table[i];
    FILE *writing = fopen(filePath,"w");


    int i = 0;
    for (; i < max; i++) {
        struct dogType *pointer=reg->hash_table[i], temp;
        if (&reg->hash_table[i]==NULL) {
            printf("entro al if WriteTable");
        //reg->hash_table[i] = malloc(sizeof(struct dogType));
        }else{
            int s = reg->sizes[i];
            fwrite(&s,sizeof(int),1,writing);
            while (pointer!=NULL) {
                printf("alright x4...\n");
                fwrite(pointer,sizeof(struct dogType),1,writing);
                pointer=pointer->next;
                written++;
            }
        }
    }
    printf("alright x3\n");
    printf("written: %i\n",written);
    fclose(writing);
    return written;
}

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

void loadDog(struct registro *reg,void *dog){
    struct dogType *newDog = dog;
    char n[32];
    memcpy(n,newDog->Name,32);
    int address = hash_function(n);
    struct dogType *pointer = reg->hash_table[address];
    int addressSec = 0;
    if(pointer==NULL){
        printf("entro al null\n");
        newDog->record = malloc(sizeof(struct pair_int));
        newDog->record->first = address;
        newDog->record->second = addressSec;
        reg->hash_table[address] = newDog;
    }else {
        printf("no es null\n");
        pointer=reg->hash_table[address];
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
    reg->countRecords++;
    reg->sizes[address]++;
    preMenu();
}

void seeRecord(struct registro *reg){
    printf("Cantidad de registros:\t"
            "%d\n",reg->countRecords);
    printf("Por favor ingrese la dirección 1 de registro a ver\n");
    int record;
    scanf("%i",&record);
    printf("Por favor ingrese la dirección 2 de registro a ver\n");
    int record2;
    scanf("%i",&record2);
    struct dogType *pointer = reg->hash_table[record];
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

void deleteRecord(struct registro *reg){
    printf("Por favor ingrese la dirección 1 de registro a ver\n");
    int record;
    scanf("%i",&record);
    printf("Por favor ingrese la dirección 2 de registro a ver\n");
    int record2;
    scanf("%i",&record2);
    struct dogType *pointer = reg->hash_table[record];
    if(pointer==NULL){
        printf("paila no hay nothing\n");
    }else{
        int pos=0;
        while (pointer->next!=NULL && pos!=record2-1) {
            pointer=pointer->next;
            pos++;
        }
        if (pos==record2-1) {
            system("gedit dataDogs.dat");
            printRecord(pointer);
        }else{
            printf("Registro no existente\n");
        }
    }
    preMenu();
}

void searchRecord(struct registro *reg){
    char n[32];
    printf("Por favor digite el nombre de su mascota\n"
           "Seguida la tecla ENTER\n");
    memset(n,32,32);
    scanf("%s",n);
    int addres = hash_function(n);
    printf("%i\n",addres);
    struct dogType *pointer = reg->hash_table[addres];
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
    FILE *fp1,*fp2;
    struct registro *copyReg = newArray();
    char str[15];
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

            copyReg = newArray();

            int address = hash_function(newPet->Name);
            //printf("%s\n",newPet->Name);
            //printf("%i\n",address);
            sprintf(str, "%d", address);
            //printf("...%s\n", str);
            strcat(str,".dat");
            FILE *prueb = fopen(str,"r");
            if(!prueb){
                perror("error abriendo el archivo: ");

                printf("alright\n");
                writeTable(copyReg,str);
                readTable(copyReg, str);
                printf("alright x6i\n");
                loadDog(copyReg,newPet);
            }else{
                fclose (prueb);
                readTable(copyReg, str);
                printf("alright x6e\n");
                loadDog(copyReg,newPet);
                writeTable(copyReg,str);
            }
            printf("Cantidad de registros:\t"
            "%d\n",copyReg->countRecords);
            break;
        case 2:
            copyReg = newArray();
            //printf("funciona\n");
            //int address2 = hash_function(newPet->Name);
            //printf("ya no funciona\n");
            //sprintf(str, "%d", address2);
            //strcat(str,".dat");
            //readTable(copyReg, str);
            seeRecord(copyReg);
            break;
        case 3:
            copyReg = newArray();
            int address3 = hash_function(newPet->Name);
            sprintf(str, "%d", address3);
            strcat(str,".dat");
            readTable(copyReg, str);
            deleteRecord(copyReg);
            writeTable(copyReg,str);
            break;
        case 4:
            copyReg = newArray();
            int address4 = hash_function(newPet->Name);
            sprintf(str, "%d", address4);
            strcat(str,".dat");
            readTable(copyReg, str);
            searchRecord(copyReg);
            break;
        case 5:
            return;
            break;
    };

}

int main(){
    menu();

    //fillName();
    /**
    char str[15];
    int address;
    scanf("%i",&address);
    printf("%i\n",address);
    sprintf(str, "%d", address);
    strcat(str,".dat");
    printf("%s\n",str);
    */
    return 0;
}
