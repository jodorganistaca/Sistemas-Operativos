#include<stdio.h>
#include<stdlib.h>

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
    newDog = dog;
    scanf("Name: %s", newDog->Name);
    scanf("Type: %s", newDog->Type);
    scanf("Name: %i", newDog->Age);
    scanf("Name: %s", newDog->breed);
    scanf("Name: %i", newDog->height);
    scanf("Name: %f", newDog->weight);
    scanf("Name: %s", newDog->gender);
}

void menu(int option){
    printf("Please ");
    switch(option){
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;

    }

}

int main(){
    struct dogType *perri;
    perri = malloc(sizeof(struct dogType));

    if(perri==NULL){
        perror("Error en el malloc");
        exit(-1);
    }

    loadDog(perri);
    FILE *fileap = fopen()

}
