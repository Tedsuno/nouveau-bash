#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
struct noeud;
struct liste_noeud;

struct noeud {
    bool est_dossier;
    char nom[100];
    struct noeud* pere;
    struct noeud* racine;
    struct liste_noeud* fils;
};

struct liste_noeud {
    struct noeud* no;
    struct liste_noeud* succ;
};

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;
noeud* creer_noeud(bool b, char* s, noeud* fils) {
    noeud* g = malloc(sizeof(noeud));
    g->fils = NULL;
    g->racine = NULL;
    g->pere = NULL;
    strcpy(g->nom, s);
    g->est_dossier = b;
    return g;
}
char* capture(const char* nom) {
    char* result = malloc(strlen(nom) + 1); // Allouer de la mémoire pour la chaîne de caractères de retour
    if (result != NULL) { // Vérifier si l'allocation a réussi
        strcpy(result, nom); // Copier la chaîne de caractères passée en paramètre dans la variable de retour
    }
    return result; // Retourner la chaîne de caractères
}
void mkdir(noeud* parent, const char *nom) {
    noeud *newDir=creer_noeud(true,capture(nom),NULL);
    newDir->pere=parent;
    if (parent->fils == NULL) { 
        parent->fils = malloc(sizeof(liste_noeud));
        parent->fils->no = newDir;
        parent->fils->succ = NULL;
    } else {
        liste_noeud* ptr = parent->fils;
        while (ptr->succ != NULL) {
            ptr = ptr->succ;
        }
        ptr->succ = malloc(sizeof(liste_noeud));
        ptr->succ->no = newDir;
        ptr->succ->succ = NULL;
    }
}

void ls(noeud* actuel) {
    liste_noeud* fils = actuel->fils;

    while (fils != NULL) {
        printf("%s\n", fils->no->nom);
        fils = fils->succ;
    }
}
bool estValide(const char *nom){
if(strlen(capture(nom))>99 || strlen(capture(nom))<1){
 return false;
}
for(int i=0; nom[i]!='\0'; i++){
	if(!isalnum(nom[i])){
		return false;
	}
}
return true;
}
bool existeDeja(noeud* courant,const char *nom){
liste_noeud* liste=courant->fils;
while(liste!=NULL){
	if(strcmp(liste->no->nom,capture(nom))==0){
		return true;
	}
    liste=liste->succ;
}
return false;
}
void touch(noeud* courant,const char *nom){
if(estValide(nom) && !existeDeja(courant,nom)){
noeud* newFic=creer_noeud(false,capture(nom),NULL);
if (courant->fils == NULL) { 
        courant->fils = malloc(sizeof(liste_noeud));
        courant->fils->no = newFic;
        courant->fils->succ = NULL;
}
else{
liste_noeud* liste=courant->fils;
while(liste->succ!=NULL){
	liste=liste->succ;
}
liste->succ=malloc(sizeof(liste_noeud));
liste->succ->no=newFic;
liste->succ->succ=NULL;
}
}
else{
	perror("Le nom est invalide ou un fichier portant ce nom existe déjà dans ce dossier.");
}
}

void pwd(noeud* courant){
    noeud* c = courant;
    char path[1000] = "";
    char chaine[1000];
    char chaine2[99];
    if (strlen(c->nom) > 0) {
        strcat(path, "/");
        strcat(path, c->nom);
    }
    strcpy(chaine, path);
    while(c->pere != c->racine){
        c = c->pere;
        strcpy(chaine2, chaine); // chaine2 = /TD[1]
        strcpy(chaine, "");      // chaine = "" 
        if (strlen(c->nom) > 0) {
            strcat(chaine, "/");
            strcat(chaine, c->nom); // chaine = /anglais
        }
        strcat(chaine, chaine2); // chaine = /anglais/TD[1]
    }
    printf("%s\n", chaine);
}
bool appartient(noeud* courant, const char* chem){
    liste_noeud* current=courant->fils;
    if(current!=NULL){
       while(current!=NULL){
            if(strcmp(current->no->nom,capture(chem))==0){
               return true;
            }
            current=current->succ;
       }
    }
    return false;
}
noeud* getAppartient(noeud* courant, const char* chem){
    liste_noeud* current=courant->fils;
    if(current!=NULL){
       while(current!=NULL){
            if(strcmp(current->no->nom,capture(chem))==0){
               return current->no;
            }
            current=current->succ;
       }
    }
    return current->no;
}
noeud* cd_chem(noeud* courant, const char* chem){
    noeud* res = courant;
    char* chemin = capture(chem);
    if (res->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(res, token)) {
                res = getAppartient(res, token);
                token = strtok(NULL, "/");
            } else {
                perror("No such file or directory");
                break;
            }
        } while (token != NULL);
        free(chemin);
    } else {
        perror("No such file or directory");
    }
    return res;
}
noeud* cd(noeud* courant){
    while(courant->pere != courant->racine){
        courant=courant->pere;
    }
    return courant;
}

noeud* cd_point(noeud* courant){
    if(courant->pere==NULL){ perror("No such file or directory");}
    if(courant->pere!=NULL){ return courant->pere;}
    return NULL;
}

int main(void) {
    noeud* racine = creer_noeud(true, "racine", NULL);
    noeud* dossier1 = creer_noeud(true, "Cours", NULL);
    noeud* dossier2 = creer_noeud(true, "anglais", NULL);
    liste_noeud* g = malloc(sizeof(liste_noeud));
    liste_noeud* g2= malloc(sizeof(liste_noeud));
    g2->no = dossier2;
    g2->succ = NULL;
    g->no = dossier1;
    g->succ = g2;
    racine->fils = g;

    noeud* dossier3 = creer_noeud(true, "TD1", NULL);
    dossier3->pere=dossier2;
    dossier2->pere=racine;
    liste_noeud* filsDossier2= malloc(sizeof(liste_noeud));
    filsDossier2->no=dossier3;
    filsDossier2->succ=NULL;
    dossier2->fils=filsDossier2;

    noeud* courant=racine;
    printf("%s\n",courant->nom);
    //ls(racine);
    //ls(dossier2);
    printf("------------\n");
    courant=cd_chem(courant,"anglais/TD1");
    printf("%s\n",courant->nom);
    printf("------------\n");
    courant=cd_point(courant);
    printf("%s\n",courant->nom);
    //mkdir(racine,"Nouveau dossier");
    //touch(racine,"print");
    //ls(racine);
    //printf("------------\n");
    //ls(dossier2);
    //pwd(dossier3);
    //printf("------------\n");
    //noeud* test=dossier3;
    //printf("%s\n",test->nom);
    //test=cd(test);
    //printf("%s\n",test->nom);
    free(g2);
    free(g);
    free(dossier3);
    free(dossier2);
    free(dossier1);
    free(racine);
    return 0;
}