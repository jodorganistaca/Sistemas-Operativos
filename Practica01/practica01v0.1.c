#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

int countRecords = 0;

struct dogType{
    char Name[32];
    char Type[32];
    int Age;
    char breed[16];
    int height;
    float weight;
    char gender[1];
};

void loadDog(void *dog){
    struct dogType *newDog;
    malloc(sizeof(struct dogType));
    newDog = dog;
    printf("Por favor ingrese los datos pedidos a continuación\n"
           "ingrese nombre:\n"
           "Cuando haya terminado presione enter\n");
    scanf("%s", newDog->Name);
    //char a[32];
    //memcpy(a,newDog->Name,32);
    //printf("%s\n",a);
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
    printf("registro hecho\n");
    countRecords++;
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

void seeRecord(){
    printf("%d\n",&countRecords);
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
