#include "../include/cmd.h"
#include "../include/noeud.h"
#include "../include/others.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*--------------------------------------------------------*/
int main(int argc, char *argv[]){
    noeud* racine=creerRacine();
    noeud* courant=racine;
    if (argc != 2) {
        printf("Usage: %s fichier\n", argv[0]);
        free(racine);
        return 1;
    }
   char ch[100];
   char temp[100];
   char *token[100];
   char *token2;
   char *token3[200];
   FILE* flux = fopen(argv[1], "r");
    if (flux == NULL) {
    perror("Probleme ouverture de fichier");
   } else {
    int c = 0;
    while (fgets(ch, 50, flux) != NULL) {
        strcpy(temp, ch);
        token[c] = strtok(temp, " ");
        token3[c] = temp;
        token2=token[c]; 
        token2 = strchr(ch, ' '); 
        if (token2 != NULL) {   
        token2++;
        int len=strlen(token2);
        if (len > 0) {
        if (token2[len - 1] == '\n') {
                    token2[len - 1] = '\0';
                }
            }        
        }
    
    if(strcmp(token[c],"mkdir")==0){
            mkdir(courant,token2);
    }
     if(strcmp(token[c],"print")==0 || strcmp(token[c],"print\n")==0){
            char *token_bis = strtok(token2, " ");
            if(token_bis!=NULL && strlen(token_bis)!=0){
                perror("error noms invalides pour print");
                exit(EXIT_FAILURE);
            }
            noeud* bis=courant;
            courant=courant->racine;
            print_arbre(courant);
            courant=bis;
            puts("-------------");
    }
    if(strcmp(token[c],"mv")==0){
           char *str1 = NULL;
           char *str2 = NULL;
           char *token_bis = strtok(token2, " ");
           if (token_bis != NULL) {
              str1 = strdup(token_bis);
              token_bis = strtok(NULL, " ");
              str2 = strdup(token_bis);
              token_bis = strtok(NULL, " ");
              if(token_bis!=NULL && strlen(token_bis)!=0){
                perror("error noms invalides pour mv");
                exit(EXIT_FAILURE);
              }
           }
           mv(courant,str1,str2);
    }
    if(strcmp(token[c],"cp")==0){
    	   char *str1 = NULL;
           char *str2 = NULL;
           char *token_bis = strtok(token2, " ");
           if (token_bis != NULL) {
              str1 = strdup(token_bis);
              token_bis = strtok(NULL, " ");

              str2 = strdup(token_bis);
              token_bis = strtok(NULL, " ");
              if(token_bis!=NULL && strlen(token_bis)!=0){
                perror("error noms invalides pour cp");
                exit(EXIT_FAILURE);
              }
           }
           cp(courant,str1,str2);
    }    
    if(strcmp(token3[c],"ls")==0){
            ls(courant);
            puts("-------------");
    }
    if(strcmp(token3[c],"rm")==0){
            rm(courant,token2);
   }
    if(strcmp(token3[c],"pwd")==0 || strcmp(token3[c],"pwd\n")==0){
            pwd(courant);
            puts("-------------");
    }
    if(strcmp(token3[c],"touch")==0){
            touch(courant,token2);
    }    
    if(strcmp(token3[c], "cd\n") == 0)courant=cd(courant);
    if(strcmp(token3[c],"ls\n")==0){ ls(courant); puts("-------------"); }
    if(strcmp(token3[c],"cd")==0 && token2!=NULL){ 
        //printf("aaaa");   
        if(strcmp(token2,"..")==0 || strcmp(token2,"..\n")==0){ courant=cd_point(courant); }
        else { courant=cd_chem(courant,token2); }
    }
    else if((strcmp(token3[c],"\n")!=0 && strcmp(token3[c],"cd")!=0 && strcmp(token3[c],"touch")!=0 && strcmp(token3[c],"pwd")!=0 && strcmp(token3[c],"pwd\n")!=0 && 
            strcmp(token3[c],"rm")!=0 && strcmp(token3[c],"ls")!=0 && strcmp(token3[c],"cp")!=0
            && strcmp(token3[c],"mv")!=0 && strcmp(token3[c],"mkdir")!=0 && strcmp(token3[c],"print")!=0 && strcmp(token3[c],"print\n")!=0
             && strcmp(token3[c],"cd\n")!=0 && strcmp(token3[c],"ls\n")!=0)){
        printf("command not found :%s",token3[c]);
        exit(EXIT_FAILURE); 
    }
    c++;  
    }
    int r = fclose(flux);
    if (r != 0) {
        perror("Probleme fermeture de fichier");
    }
    }
    free_chem(racine);
    free(racine);
    return 0;
}