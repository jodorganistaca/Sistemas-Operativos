#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
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


char *names[1000];

char *types[] = {"perro","gato","tortuga","conejo",
"elefante","Caballo","gallina","hamster","vaca",
"ganso","oveja","loro","pavo","codorniz","gallo",
"pajaro","mono","jirafa","ardilla","iguana","aguila"};

char *breed[] = {"Akita","Azawakh","Basenji","Beagle","Bulldog",
 "Caniche","Chihuahua","Dogo","Foxhound","Galgo","Greyhound",
"Harrier","Husky", "Kishu","Komondor","Labrador","Landseer",
"Lebrel","Leonberger","Mudi","Pumi",};
//extern struct dogType *hash_table[max];

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
    while(fgets(line, sizeof(line),file)){
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
    int u=0,registers = 11,i;
    fillName();
    FILE * fp;
    char * line = NULL;
    ///Este ciclo llena los más de 10.000.000 de registros del archivo
     //La razon de estos numeros es que hay 1716 nombres de mascotas, entonces 1716*10*582
    FILE* fout = fopen("dataDogs.dat", "w+"); //Creando la "base de datos" en modo de escritura, se hara en modo binario.
    if (fout == NULL){
        exit(EXIT_FAILURE);
        printf("El archivo con los nombres no ha podido ser cargado");
     }
    for(;u<registers;u++){
        size_t len = 0;
        size_t read;
        fp = fopen("nombresMascotas.txt", "r"); //Abriendo el archivo fuente dado para la practica

        if (fp == NULL){
            exit(EXIT_FAILURE);
            printf("El archivo con los nombres no ha podido ser cargado"); //Validacion de la apertura del archivo
            }

        while ((read = getline(&line, &len, fp)) != -1) {
        //La siguiente linea imprimira la linea obtenida y su longitud en caso de necesitarse para depuracion
        // printf("Tamano de la linea %zu : , linea obtenida: %s \n", read, line);

                for(i = 0; i<583; i++){
                        strtok(line, "\n");
                        strcpy(currentDog.nombre,line); //Se asigna el nombre recogido en la base de datos de los nombres al nombre del dogType
                        currentDog.edad = rand() % 21; //Se asigna una edad al azar maxima en 20.
                        strcpy(currentDog.raza, razas[rand()%21]); //Se asigna una de las razas previstas
                        strcpy(currentDog.tipo, tipos[rand()%21]); //Se asigna uno de los tipos previstas
                        currentDog.estatura = rand_interval(16,86); //Las estaturas de los perros estan en este rango, 16-86, se asigna al azar
                        currentDog.peso = rand_interval_float(20,40); //Los pesos dados en decimal estan en esta razon
                        if(i%2 == 0) currentDog.sexo = 'H';
                            else currentDog.sexo = 'M'; //Se especifica que salga H o M con un patron para que queden distribuidos
                        currentDog.hash = -1;
                        int ii =0;
                        for( ii = 0; line[ii]; ii++){
                              line[ii] = tolower(line[ii]);
                            }
                        addElem(arrayList[hashCode(line)],currentDog);
                        counter++;
                        //fwrite(&currentDog, sizeof(dogType), 1, fout);
                    }
        }
        fclose(fp); //Se cierra el archivo de donde se sacaron los nombres
        if (line)
        free(line); //Se libera el espacio de memoria requerido por la cadena "line"
    }
printf("Generando listas enlazadas...\n");
fclose(fout);

    /*int r,h =0;
    scanf("%i",&h);
    while(h < 10){
        char *c = randName();
        printf("%s\n",c);
        c = randType();
        printf("%s\n",c);
        r = randAge(0,16);
        printf("%i\n",r);
        scanf("%i",&h);
        h++;
    }*/
    return 0;
}
