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
bool existeDeja(noeud* courant,const char *nom){ //fonction permettant de faire la meme chose que appartient 
liste_noeud* liste = courant->fils;
char *c = capture(nom);
while(liste != NULL){
    if(strcmp(liste->no->nom,c)==0){
        return true;
    }
    liste = liste->succ;
}
free(c);
return false;
}
/*---------------------------------------------------------------*/
bool appartient(noeud* courant, const char* chem){ //fonction servant à savoir si un noeud est dans la liste des fils par rapport à un bout de chemin
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
noeud* getAppartient(noeud* courant, const char* chem){ //pareil que appartient sauf qu'il renvoie ce noeud s'il existe
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
}
/*---------------------------------------------------------------*/
int nb_fils(noeud* courant) {
    if(courant->fil == NULL) return 0;
    int count = 0;
    liste_noeud* fils = courant->fils;
    while (fils != NULL) {
        count++;
        fils = fils->succ;
    }
      return count;
}
/*-------------------------------------------*/
char* NomRacine(noeud* courant) {
    char* nom;
    if (courant == courant->racine) {
        nom = malloc(2);
        strcpy(nom, "/");
    } else {
        nom = malloc(strlen(courant->nom) + 1);
        strcpy(nom, courant->nom);
    }
    return nom;
}

/*---------------------------------------------------------------*/
void print_arbre(noeud* courant) {
    printf("Noeud %s (%s), père : %s, %d fils : ", NomRacine(courant), courant->est_dossier ? "D" : "F", NomRacine(courant->pere), nb_fils(courant));
    if (courant->fils == NULL) {
        puts("");
        return;
    }
    liste_noeud *fils=courant->fils;
    while (fils!=NULL) {
        printf("%s (%s), ", fils->no->nom, fils->no->est_dossier ? "D" : "F");
        fils=fils->succ;
    }
    puts("");
    fils=courant->fils;
    while (fils!=NULL) {
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
noeud * rechercher_noeud(noeud * courant, char * chem) {
    //Si c'est un chemin absolu
    noeud * res = courant;
    char * c = capture(chem);
    if (strlen(c) == 0) return courant -> racine;
    if (c[0] == '/') res = courant -> racine; else res = courant;
      if (c[1] != '/') {
        char * chemin = capture(chem);
        if (res -> fils != NULL && chemin != NULL) {
          char * token = strtok(chemin, "/");
          do {
            if (appartient(res, token)) {
              res = getAppartient(res, token);
              token = strtok(NULL, "/");
            } else {
              res=courant;
                mkdir(res,token);
                free(chemin);
                free(c);
                return res;
                break; 
            }
          } while (token != NULL);
          free(chemin);
        } else {
          free(chemin);
          free(c);
          perror("No such file or directory GGGG");
          exit(EXIT_FAILURE);
        }
      } else {
        free(c);
        perror("No such file or directory TTTT");
        exit(EXIT_FAILURE);
      }
    free(c);
    return res;
}
/*-------------------------------------------*/
noeud* copier_noeud(noeud* src) { //fonction qui copie un noeud et tout ses fils (et les fils de ses fils etc)
    if (src == NULL) {
        return NULL;
        exit(EXIT_FAILURE);
    }
    noeud* copie = (noeud*)malloc(sizeof(noeud)); //on alloue de la mémoire pour un futur noeud copie
    copie->est_dossier = src->est_dossier;        //si src est un dossier, copie le sera également et inversement
    strncpy(copie->nom, src->nom, 100);           //on copie le nom de src dans le noeud copie dans la limite de 100 caractères
    copie->pere = src->pere;                      //le père de src sera également le père de copie
    copie->racine = src->racine;                  //sa racine aussi
    copie->fils = NULL;                           //copie n'a pas de fils
    liste_noeud* src_fils = src->fils;            //on créée une liste de noeud src_fils qui est égale au fils de src
    liste_noeud* copie_fils = NULL;               
    liste_noeud* prev_fils = NULL;
    //on a copié le noeud courant complétement, maintenant on va passer à ses fils(et les fils de ses fils etc)

    while (src_fils != NULL) {                                  //tant que src à toujours un fils
        copie_fils = (liste_noeud*)malloc(sizeof(liste_noeud)); //on alloue de la mémoire pour une liste de noeud de copie
        copie_fils->no = copier_noeud(src_fils->no);            //récursivement on ajoute au fils de copie le fils de src
        copie_fils->succ = NULL;                                //son succésseur est null pour tant qu'on a pas encore ajouté d'autre fils
        if (prev_fils == NULL) {                                //ensuite on regarde si c'est le premier fils ajouté ou non et on manipule les succésseur en fonction de ça
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
void ajouter_fils(noeud* parent, liste_noeud* fils) {
    // Parcours de la liste des fils à copier
    liste_noeud* p = fils;
    while (p != NULL) {
        // Création d'un nouveau nœud fils avec les mêmes propriétés que celui à copier
        noeud* new_fils = (noeud*)malloc(sizeof(noeud));
        new_fils->est_dossier = p->no->est_dossier;
        strcpy(new_fils->nom, p->no->nom);
        new_fils->pere = parent;
        new_fils->racine = parent->racine;
        new_fils->fils = NULL; // initialise la liste des fils à NULL

        // Ajout du nouveau fils à la liste des fils du parent
        liste_noeud* new_liste_noeud = (liste_noeud*)malloc(sizeof(liste_noeud));
        new_liste_noeud->no = new_fils;
        new_liste_noeud->succ = NULL;

        if (parent->fils == NULL) {
            parent->fils = new_liste_noeud;
        }
        else {
            liste_noeud* last = parent->fils;
            while (last->succ != NULL) {
                last = last->succ;
            }
            last->succ = new_liste_noeud;
        }

        // Copie récursive des descendants
        ajouter_fils(new_fils, p->no->fils);

        // Passage au fils suivant dans la liste à copier
        p = p->succ;
    }
}