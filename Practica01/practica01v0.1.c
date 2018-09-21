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
    printf("%i\t",a[i]);
    hash = (31 * hash + a[i])%max;
  }
  printf("\n");
  return hash;
}

struct node* get_element(struct node *list, int find_index);
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
    int address = hash_function(n);
    if(hash_table[address]==NULL){
        printf("entro al null\n");
        hash_table[address] = malloc(sizeof(struct dogType));
        hash_table[address] = newDog;
        hash_table[address]->next=NULL;
    }else {
      printf("no es null\n");
      pointer=hash_table[address];
      while (pointer->next != NULL) {
        printf("+1\n");
        pointer = pointer->next;
      }
      pointer->next = malloc (sizeof(struct dogType));
      pointer->next = newDog;
      /*pointer->next = NULL;*/
    }
    printf("registro hecho\n");
    countRecords++;
    preMenu();
}

void seeRecord(){
    printf("Cantidad de registros:\t"
           "%d\n",countRecords);
    printf("Por favor ingrese el numero de registro a ver\n");
    int record;
    scanf("%i",&record);
    if(hash_table[record]==NULL){
        printf("paila no hay nothing\n");
    }else{
        system("gedit dataDogs.dat");
        printf("hay algo\n");
        char n[32];
        char t[32];
        memcpy(n,hash_table[record]->Name,32);
        memcpy(t,hash_table[record]->Type,32);

        printf("nombre:  %s\n",n);
        printf("tipo: %s\n",t);
        printf("Edad: %i\n",hash_table[record]->Age);
        struct dogType *dogNext;
        if(hash_table[record]->next!=NULL){
            dogNext = hash_table[record]->next;
            printf("hay algo despues del next\n");
            char n2[32];
            char t2[32];
            memcpy(n2,dogNext->Name,32);
            memcpy(t2,dogNext->Type,32);
            printf("nombre:  %s\n",n2);
            printf("tipo: %s\n",t2);
            printf("Edad: %i\n",dogNext->Age);
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
    printf("%s\n",prueba);
    printf("%d\n",hash_function(prueba));
    printf("Por favor digite el nombre de su mascota\n"
           "Seguida la tecla ENTER\n");
    memset(n,0,32);
    scanf("%s",n);
    int addres = hash_function(n);
    printf("%s\n",n);
    printf("%i\n",addres);
    preMenu();
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
    menu();
    return 0;
}
