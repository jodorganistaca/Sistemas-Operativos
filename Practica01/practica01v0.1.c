#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#define max 1000

struct dogType{
    char Name[32];
    char Type[32];
    int Age;
    char breed[16];
    int height;
    float weight;
    char gender[1];
    struct dogType *next;
};

struct node{
    int key;
    struct dogType *value;
    struct node *next;
};

struct arrayitem{
    struct node *head;
    struct node *tail;
};

struct arrayitem *array;
int countRecords = 0;
struct dogType *hash_table[max];
struct dogType *next=NULL;
char prueba[32];

int hash_function(char a[32]){
  int hash = 0;
  for (int i = 0; i < 32; i++) {
    hash = (31 * hash + a[i])%max;
  }
  return hash;
}
struct node* get_element(struct node *list, int find_index);
void deleteRecord(int key);
void rehash();
void init_array();

void loadDog(void *dog){
    struct dogType *newDog;
    malloc(sizeof(struct dogType));
    newDog = dog;
    printf("Por favor ingrese los datos pedidos a continuación\n"
           "ingrese nombre:\n"
           "Cuando haya terminado presione enter\n");
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
    char n[32];
    memcpy(n,newDog->Name,32);
    memcpy(prueba,newDog->Name,32);
    struct dogType *start;
    struct dogType *pointer;
    int adress = hash_function(n);
    if(hash_table[adress]==NULL){
      hash_table[adress] = malloc(sizeof(struct dogType));
      hash_table[adress] =newDog;
      hash_table[adress]->next=NULL;
    } else {
      pointer=hash_table[adress];
      while (pointer->next != NULL) {
        pointer = pointer->next;
      }
      pointer->next = malloc (sizeof(struct dogType));
      pointer = newDog;
      pointer = pointer->next;
      pointer->next = NULL;
    }
    printf("registro hecho\n");
    countRecords++;
    preMenu();
}

void seeRecord(){
    printf("Cantidad de registros:\t"
           "%d\n",countRecords);
    printf("Por favor ingrese el numero de registro a ver\n");
    preMenu();
}

void deleteRecord(int key){
    preMenu();
}

void searchRecord(){

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
            newPet = malloc(sizeof(struct dogType));
            loadDog(newPet);
            int adress = hash_function(prueba);
            char n[32];
            memcpy(n,hash_table[adress]->Name,32);
            printf("prueba:  %s\n",n);
            break;
        case 2:
            seeRecord();
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            return;
            break;
    };

}

int main(){
   menu();
    /*struct dogType *perri;
    perri = malloc(sizeof(struct dogType));
    memset(perri->Name,' ',sizeof(struct dogType)-(sizeof(int)+sizeof(int)+sizeof(float)+sizeof(81)));

    if(perri==NULL){
        perror("Error en el malloc");
        exit(-1);
    }

    loadDog(perri);
    FILE *file = fopen("dataDogs.dat", "w");
    if(file == NULL){
        perror("error fopen");
        exit(-1);
    }

    int t = fwrite(perri, sizeof(struct dogType), 1,file);
    if(t==0){
        perror("Error en fwrite");
        exit(-1);
    }
    int c = fclose(file);
    if(c != 0){
        perror("Error en fclose");
        exit(-1);
    }*/
}
