#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define prime 1009

int hash_function(char a[32]){
    int hash = 0;
    for (int i = 0; i < 32; i++) {
        if(a[i]>=65&&a[i]<=90){
            a[i]+=32;
        }
        hash = (3 * hash + a[i])%prime;
    }
    printf("\n");
    return hash;
}

int main(){
    int i;
    FILE *fileR = fopen("names.txt","r");//Abre el archivo en modo lectura
    char line[256]; //buffer de tamaño 256 para leer la linea de el archivo
    char *names[1005]; //arreglo de tamaño 1000 de punteros para los nombres
    char *freeId[10];//arreglo de tamaño 10 de punteros para los ids libres
    int arrId[1010];//arreglo para saber que pos estan libres
    for(i = 0; i<1005;i++){//Reservo espacio para cada puntero e inicializo los arrId
        names[i] = malloc( 32 * sizeof(char));
        arrId[i] = 0;
        //freeId[i] = malloc( 10 * sizeof(char));
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
        i+=1;//aumenta contador
    }
    fclose(fileR);//Cierra archivo de lectura
    FILE *fileW = fopen("namesIds.txt","w");//Abrimos un archivo de escritura para los nombres y ids
    FILE *fileW2 = fopen("freeIds.txt","w");//Abrimos un archivo de escritura solo los ids libres
    for(i = 0; i<1005;i++){
        id = hash_function(names[i]);
		if(arrId[i]==0){//Si el arrId esta en 0 significa que no esta escrito una mascota en esta posicion
			fprintf(fileW2,"%i\n",i);//escribimos en el archivo W2 los ids libres
        }
        fprintf(fileW, " %d %s \n", id, names[i]);//escribimos en el archivo W1 los id y su nombre        
    }
    fclose(fileW);
	fclose(fileW2);
}
