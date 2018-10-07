#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#define max 10000000
#define prime 10009

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
    char gender[1];

};

struct dogType *hash_table[max];

int hash_function(char a[32]){
    int hash = 0;
    for (int i = 0; i < 32; i++) {
        //printf("%i\t",a[i]);
        if(a[i]>=65&&a[i]<=90){
            a[i]+=32;
        }
        hash = (3 * hash + a[i])%prime;
    }
    printf("\n");
    return hash;
}

void preMenu(){
    printf("Oprima [Enter] para volver al menu\n");
    fflush(stdin);
	getchar();
	getchar();
}

void printRecord(struct dogType *dog){
    printf("------------------------\n");
   // printf("Registro: %i %i\n", dog->record->first,dog->record->second);
//    printf("Id: %i\n",dog->id);
    printf("Nombre: %s\n",dog->Name);
    printf("Tipo: %s\n", dog->Type);
    printf("Edad: %i años\n", dog->Age);
    printf("Raza: %s\n", dog->breed);
    printf("Estatura: %i cm\n", dog->height);
    printf("Peso: %3f Kg\n", dog->weight);
    printf("Sexo: %s\n", dog->gender);
    printf("------------------------\n");
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

int hash_file(char n[32]){
    int found = 0,sol,temp=0;
    FILE *fp=fopen("dataDogs.dat","rb");
    struct dogType* pet=malloc(sizeof(struct dogType));
    if(fp==NULL){
		printf("Error abriendo archivo dataDogs.dat\n");
		return 0;
	}else{
        printf("entro al else\n");
		fread(pet,sizeof(struct dogType),1,fp);
		fseek(fp, 0L, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		sol=(size/sizeof(struct dogType)) + 1;
		while (!feof(fp)) {
			if(equals(pet->Name,n)==1){
				found=1;
				sol = temp;
				//printRecord(pet);
			}temp+=1;
			fread(pet,sizeof(struct dogType),1,fp);
		}
		fclose(fp);
	}
	free(pet);
	if(found==1) printf("Búsqueda concluida exitosamente.\n");
	if(found==0) printf("Mascota no encontrada.\n");
	preMenu();
	return sol;
}

//struct dogType *createDog(){
void writeTable(int pos, struct dogType *pet){
    long int wr=0;
    int tam;
    int ingre;
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

void insertRecord(){
    //Se llenan los campos pedidos en la estructura NewDog
    struct dogType *newDog = malloc(sizeof(struct dogType));
    printf("Por favor ingrese los datos pedidos a continuación\n"
        "ingrese nombre:\n"
        "Cuando haya terminado presione enter\n");
    memset(newDog->Name,32,32);
    char n[32];
    scanf("%s", newDog->Name);
    printf("ingrese especie:\n"
        "Cuando haya terminado presione enter\n");
    // memcpy(n,newDog->Name,32);
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
    //Se llenan todos los campos menos id y Next

    int h = hash_function(newDog->Name);
    newDog->existe=1;
    //En WriteTable si se llenan estos campos ya que se calculan
    int m=isFull(h);
    while (m==-1) {
      printf("m = %i\n",m);
      h=h+1000;
      m=isFull(h);
    }
    newDog->id = h;
    writeTable(h,newDog);
    printf("El id del registro%i\n",h);
    printf("registro hecho\n");

    preMenu();
}


void seeRecord(){
	FILE *files=fopen("dataDogs.dat","rb");
	struct dogType* dog=malloc(sizeof(struct dogType));
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
           int buscar=0;
           scanf("%i\n",&buscar);

           if (buscar>totalRecords) {
             printf("Posicion erronea\n");
           }
           fseek(files,(sizeof(int)+(sizeof(struct dogType)*(buscar-1))),SEEK_SET);
           fread(dog,sizeof(struct dogType),1,files);
           printf("Nombre %s\n",dog->Name);
           printf("id %i\n",dog->id);
           printf("next %i\n",dog->next);
           /*while (!feof(files)) {
             if (dog->id==buscar) {
               FILE * fh = fopen("history.txt","w");
               fprintf(fh, "ID: %i\nNombre: %s\nTipo: %s\nEdad: %i\nRaza: %s\nEstatura: %i\nPeso: %lf\nSexo: %c\n", dog->id, dog->Name, dog->Type, dog->Age, dog->breed, dog->height, dog->weight, dog->gender);
				fclose(fh);
        system("nano history.txt");
        return;
  }
  fread(dog,sizeof(struct dogType),1,files);
}*/
fseek(files,0L,SEEK_END);

fclose(files);
}
free(dog);
printf("Información mostrada exitosamente.\n");

    preMenu();
}

void deleteRecord(){
/*
    printf("Cantidad de registros:\t"
           "%d\n",countRecords);*/
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
    int found = 0;
    FILE *fp=fopen("dataDogs.dat","rb");
    struct dogType* pet=malloc(sizeof(struct dogType));
    if(fp==NULL){
		printf("Error abriendo archivo dataDogs.dat\n");
		return;
	}else{
		fread(pet,sizeof(struct dogType),1,fp);
		while (!feof(fp)) {
			if(equals(pet->Name,n)==1){
				found=1;
				//printRecord(pet);
			}
			fread(pet,sizeof(struct dogType),1,fp);
		}
		fclose(fp);
	}
	free(pet);
	if(found==1) printf("Búsqueda concluida exitosamente.\n");
	if(found==0) printf("Mascota no encontrada.\n");
	preMenu();
}

//Menu principal
void menu(){
    int option;
    do{
        printf("Por favor seleciona un numero de las sgtes opciones seguido de la tecla enter\n"
           "1. Ingresar registro\n"
           "2. Ver registro\n"
           "3. Borrar registro\n"
           "4. Buscar registro\n"
           "5. Salir\n");
        scanf("%i",&option);

        struct dogType *newPet;
        switch(option){
            case 1:
                //newPet = createDog();
                insertRecord();
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
            case 6:
                ;
                char nombre[32];
                printf("Ingrese el nombre de la mascota a calcular la hash Function\n");
                memset(nombre,32,32);
                scanf("%s",nombre);
                printf("%s\n",nombre);
                int h = hash_function(nombre);
                printf("%i\n", h);
                break;
        };
    }while(option!=5);

}

int main(){
menu();
    return 0;
}
