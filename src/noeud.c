#include "../include/cmd.h"
#include "../include/noeud.h"
#include "../include/others.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*-------------------------------------------*/
noeud* creerNoeud(bool est_dos, const char* nom, noeud* pere,liste_noeud * fils, noeud* racine){
       noeud* n=malloc(sizeof(noeud));
       n->est_dossier=est_dos;
       strcpy(n->nom,nom);
       n->pere=pere;
       n->fils=fils;
       n->racine=racine;
       return n;
}
/*-------------------------------------------*/
noeud* creerRacine(){
       noeud* r=creerNoeud(true,"",NULL,NULL,NULL);
       r->pere=r;
       r->racine=r;
       return r;
}
/*---------------------------------------------------------------*/
bool existeDeja(noeud* courant,const char *nom){
liste_noeud* liste=courant->fils;
char *c=capture(nom);
while(liste!=NULL){
	if(strcmp(liste->no->nom,c)==0){
		return true;
	}
    liste=liste->succ;
}
free(c);
return false;
}
/*---------------------------------------------------------------*/
bool appartient(noeud* courant, const char* chem){
    liste_noeud* current=courant->fils;
    if(current!=NULL){
       while(current!=NULL){
            char *c=capture(chem);
            if(strcmp(current->no->nom,c)==0){
               return true;
            }
            free(c);
            current=current->succ;
       }
    }
    return false;
}
/*---------------------------------------------------------------*/
noeud* getAppartient(noeud* courant, const char* chem){
    liste_noeud* current=courant->fils;
    if(current!=NULL){
       while(current!=NULL){
            char *c=capture(chem);
            if(strcmp(current->no->nom,c)==0){
               return current->no;
            }
            free(c);
            current=current->succ;
       }
    }
    return current->no;
}/*-------------------------------------------*/
bool estDernier(noeud* courant,const char* chem){
	char* nom=NULL;
	char* chemin=capture(chem);
	char* token = strtok(chemin, "/");
        do {
        	nom=token;
            token = strtok(NULL, "/");
        }
    while (token != NULL);
    if(strcmp(courant->nom,nom)==0){
    	return true;
    }
    else{
    	return false;
    }
}
/*-------------------------------------------*/
noeud* getDernier(noeud* courant,const char* chem){
	noeud* res = courant;
    char* chemin = capture(chem);
    if (res->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(res, token)) {
                res = getAppartient(res, token);
                token = strtok(NULL, "/");
            } else {
                res=courant;
                perror("No such file or directory");
                exit(EXIT_FAILURE);
                break;
            }
        } while (token != NULL);
        free(chemin);
    } else {
        perror("No such file or directory");
        exit(EXIT_FAILURE);
    }
    return res;
}
/*---------------------------------------------------------------*/
int nb_fils(noeud* courant) {
    if(courant->fils==NULL) return 0;
    int count=0;
    liste_noeud* fils=courant->fils;
    while (fils!=NULL) {
        count++;
        fils=fils->succ;
    }
      return count;
}
/*---------------------------------------------------------------*/
void print_arbre(noeud* courant) {
    printf("Noeud %s (%s), %d fils : ", courant->nom, courant->est_dossier ? "D" : "F", nb_fils(courant));
    if (courant->fils == NULL) {
        printf("Aucun fils\n");
        return;
    }
    puts("");
    liste_noeud *fils=courant->fils;
    while (fils!=NULL) {
        printf("Noeud %s (%s), pere : %s, ", fils->no->nom, fils->no->est_dossier ? "D" : "F", courant->nom);
        print_arbre(fils->no);
        fils=fils->succ;
    }
}
/*-------------------------------------------*/
void free_chem(noeud* courant){
    liste_noeud* fils_courant = courant->fils;
    while (fils_courant != NULL) {
        noeud* fils = fils_courant->no;
        free_chem(fils);
        free(fils->fils);
        free(fils);
        fils_courant = fils_courant->succ;
    }
    courant->fils = NULL;
}
/*-------------------------------------------*/
void free_noeud(noeud* n) {
    if (n == NULL) {
        return;
    }
    liste_noeud* fils = n->fils;
    while (fils != NULL) {
        liste_noeud* suivant = fils->succ;
        free_noeud(fils->no);
        free(fils);
        fils = suivant;
    }
    free(n);
}
/*-------------------------------------------*/
noeud* rechercher_noeud(noeud* courant,char* chem) {
    //Si c'est un chemin absolu
    noeud* res=courant;
    char* c=capture(chem);
    if(c[0]=='/' && courant==courant->racine){
       if(c[1]!='/'){
         res = courant->racine;
         if(courant==getDernier(courant->racine,chem)) {
            free(c);
            perror("Vous etes déjà dans ce dossier");
            return courant;
            exit(EXIT_FAILURE);
         }
       char* chemin = capture(chem);
       if (res->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(res, token)) {
                res = getAppartient(res, token);
                token = strtok(NULL, "/");
            } else {
                res=courant;
                perror("No such file or directory");
                free(chemin);
                free(c);
                exit(EXIT_FAILURE);
                break;
            }
         } while (token != NULL);
         free(chemin);
       } 
       else {
         free(chemin);
         free(c);
         perror("No such file or directory");
         exit(EXIT_FAILURE);
       }
      }
      else{
        free(c);
        perror("No such file or directory");
        exit(EXIT_FAILURE);
      }
    }
    /*+++++++++++++++++++++++++++++++++++++++*/
    //Si ce n'est pas un chemin absolu
    else{
      if(c[1]!='/'){
      res = courant;
      char* chemin = capture(chem);
      if (res->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(res, token)) {
                res = getAppartient(res, token);
                token = strtok(NULL, "/");
            } else {
                res=courant;
                perror("No such file or directory");
                free(chemin);
                free(c);
                exit(EXIT_FAILURE);
                break;
            }
        } while (token != NULL);
        free(chemin);
      } 
      else {
         free(chemin);
         free(c);
         perror("No such file or directory");
         exit(EXIT_FAILURE);
      }
     }
     else{
        free(c);
        perror("No such file or directory");
        exit(EXIT_FAILURE);
     }
    }
    free(c);
    return res;
}
/*-------------------------------------------*/
noeud* copier_noeud(noeud* src) {
    if (src == NULL) {
        return NULL;
        exit(EXIT_FAILURE);
    }
    noeud* copie = (noeud*)malloc(sizeof(noeud));
    copie->est_dossier = src->est_dossier;
    strncpy(copie->nom, src->nom, 100);
    copie->pere = src->pere;
    copie->racine = src->racine;
    copie->fils = NULL;
    liste_noeud* src_fils = src->fils;
    liste_noeud* copie_fils = NULL;
    liste_noeud* prev_fils = NULL;
    while (src_fils != NULL) {
        copie_fils = (liste_noeud*)malloc(sizeof(liste_noeud));
        copie_fils->no = copier_noeud(src_fils->no);
        copie_fils->succ = NULL;
        if (prev_fils == NULL) {
            copie->fils = copie_fils;
        } else {
            prev_fils->succ = copie_fils;
        }
        prev_fils = copie_fils;
        src_fils = src_fils->succ;
    }

    return copie;
}
/*-------------------------------------------*/
void ajouter_fils(noeud* parent, noeud* fils) {
    if (parent == NULL || fils == NULL) {
        return;
        exit(EXIT_FAILURE);
    }
    liste_noeud* nouvel_element = (liste_noeud*)malloc(sizeof(liste_noeud));
    nouvel_element->no = copier_noeud(fils);
    nouvel_element->succ = NULL;
    liste_noeud* dernier_element = parent->fils;
    if (dernier_element == NULL) {
        parent->fils = nouvel_element;
    } else {
        while (dernier_element->succ != NULL) {
            dernier_element = dernier_element->succ;
        }
        dernier_element->succ = nouvel_element;
    }
}