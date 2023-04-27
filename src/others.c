#include "../include/cmd.h"
#include "../include/noeud.h"
#include "../include/others.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*-------------------------------------------*/
char* capture(const char* nom){
    char* result= malloc(strlen(nom)+1);
    if(result!=NULL){
        strcpy(result,nom);
    }
    return result;
}
/*---------------------------------------------------------------*/
bool estValide(const char *nom){
char *c=capture(nom);
if(strlen(c)>99 || strlen(c)<1){
 return false;
}
for(int i=0; nom[i]!='\0'; i++){
    if(!isalnum(nom[i])){
        return false;
    }
}
free(c);
return true;
}
/*---------------------------------------------------------------*/
char* chemin_sans_dernier(const char* chem) {
    int len = strlen(chem);
    char* res = malloc(len + 1);
    if (res == NULL) {
        perror("Allocation mémoire impossible");
        exit(EXIT_FAILURE);
    }
    strcpy(res, chem);
    char* last_slash = strrchr(res, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
    }
    return res;
}
/*-------------------------------------------*/
char* chemin_dernier(const char* chem) {
    char* dernier = NULL;
    char* token = strtok((char*)chem, "/");
    while (token != NULL) {
        dernier = token;
        token = strtok(NULL, "/");
    }
    return dernier;
}