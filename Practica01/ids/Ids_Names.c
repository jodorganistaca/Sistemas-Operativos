#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define prime 1009

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

int main(){
    int i;
    FILE *fileR = fopen("names.txt","r");
    char line[256];
    char *names[1000];
    for(i = 0; i<1000;i++){
        names[i] = malloc( 32 * sizeof(char));
    }
    i =0;
    while(fgets(line, sizeof(line)-1,fileR)){
        strtok(line, "\n");
        /*printf("%s\n",line);*/
        strcpy(names[i],line);
        printf("%i\n",hash_function(names[i]));
        i+=1;
    }
    fclose(fileR);
    FILE *fileW = fopen("namesIds.txt","w");
    for(i = 0; i<1000;i++){
        int id = hash_function(names[i]);
        char *idN[10] = ;
        fwrite(id,sizeof(int),1,fileW);
        //fwrite('\n',sizeof(char),1,fileW);
    }
    fclose(fileW);
}
