#include "../include/cmd.h"
#include "../include/noeud.h"
#include "../include/others.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_commande(char* ch){
    if(ch!=NULL){
    if(strcmp(ch,"ls")!=0 && strcmp(ch,"pwd")!=0 && strcmp(ch,"mkdir")!=0 && strcmp(ch,"cp")!=0 && strcmp(ch,"rm")!=0
        && strcmp(ch,"cd")!=0 && strcmp(ch,"mv")!=0 && strcmp(ch,"touch")!=0 && strcmp(ch,"print")!=0 )
        return false;
    }
    return true; 
}

char* commande_terminale(char* phrase, int count) {
    char* resultat = strdup(phrase);
    char* token = NULL;
    
    for (int i = 0; i < count; i++) {
        if (resultat != NULL) {
            token = strtok(resultat, " \n");
            resultat = strtok(NULL, "");
        } else {
            return NULL;  // Renvoie NULL si le nombre de mots est dépassé
        }
    }
    
    if (token != NULL) {
        return strdup(token);
    } else {
        return NULL;
    }
}

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
    FILE* flux = fopen(argv[1], "r");

    if (flux == NULL) {
    perror("Probleme ouverture de fichier");
    }

    while (fgets(ch, 50, flux) != NULL) {

    if(!is_commande(commande_terminale(ch,1))){
        printf("Vous avez entré une commande inconnue : %s",commande_terminale(ch,1));
        free_chem(racine);
        free(racine);
        exit(EXIT_FAILURE);
    }
    if(commande_terminale(ch,1)!=NULL){

    if(strcmp(commande_terminale(ch,1),"mkdir")==0){
        if(commande_terminale(ch,2)!=NULL && commande_terminale(ch,3)==NULL){
            mkdir(courant,commande_terminale(ch,2));
        }
        else{
            perror("erreur mkdir, ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE);  
        }
    }
     if(strcmp(commande_terminale(ch,1),"print")==0){
        if(commande_terminale(ch,2)==NULL){
            printf("-----print-----\n");
            noeud* bis=courant;
            courant=courant->racine;
            print_arbre(courant);
            courant=bis;
            printf("---------------\n");
        }
        else{
            perror("erreur print, ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE);  
        }
    }
    if(strcmp(commande_terminale(ch,1),"mv")==0){
        if(commande_terminale(ch,2)!=NULL && commande_terminale(ch,3)!=NULL && commande_terminale(ch,4)==NULL) mv(courant,commande_terminale(ch,2),commande_terminale(ch,3));
        else{ perror("erreur, mv ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE); 
        }
    }

    if(strcmp(commande_terminale(ch,1),"cp")==0){
        if(commande_terminale(ch,2)!=NULL && commande_terminale(ch,3)!=NULL && commande_terminale(ch,4)==NULL) cp(courant,commande_terminale(ch,2),commande_terminale(ch,3));
        else{ perror("erreur, cp ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE); 
        }
    } 

    if(strcmp(commande_terminale(ch,1),"ls")==0){
        if(commande_terminale(ch,2)==NULL){
            printf("-----ls-----\n");
            ls(courant);
            printf("---------------\n");
        }
        else{
            perror("erreur ls, ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE);  
        }
    }
    if(strcmp(commande_terminale(ch,1),"rm")==0){
        if(commande_terminale(ch,2)!=NULL && commande_terminale(ch,3)==NULL){
            rm(courant,commande_terminale(ch,2));
        }
        else{
            perror("erreur rm, ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE);  
        }
   }

    if(strcmp(commande_terminale(ch,1),"pwd")==0){
        if(commande_terminale(ch,2)==NULL){
            printf("------pwd------\n");
            pwd(courant);
            printf("---------------\n");
        }
        else{
            printf("%s",commande_terminale(ch,1));
            perror("erreur pwd, ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE);  
        }
    }
    if(strcmp(commande_terminale(ch,1),"touch")==0){
        if(commande_terminale(ch,2)!=NULL && commande_terminale(ch,3)==NULL){
        touch(courant,commande_terminale(ch,2));
        }
        else{
            perror("erreur touch, ne respecte pas le format de base."); free_chem(racine);
            free(racine); exit(EXIT_FAILURE);  
        }
    }

    if(strcmp(commande_terminale(ch,1), "cd") == 0){
        if(commande_terminale(ch,2)==NULL){
            courant=cd(courant);
        }
        else if(commande_terminale(ch,3)==NULL){
            if(strcmp(commande_terminale(ch,2), "..") == 0) courant=cd_point(courant);
            if(strcmp(commande_terminale(ch,2), "..") != 0) courant=cd_chem(courant,commande_terminale(ch,2));
        } 
        else{
            perror("erreur cd, ne respecte pas le format de base.");
            free_chem(racine);
            free(racine);
            exit(EXIT_FAILURE);
        }  
    }
  }
}
    int r = fclose(flux);
    if (r != 0) {
        perror("Probleme fermeture de fichier");
    }
    free_chem(racine);
    free(racine);
    return 0;
}
