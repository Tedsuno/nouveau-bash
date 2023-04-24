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