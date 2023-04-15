#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
/*-------------------------------------------*/
struct noeud ;
struct liste_noeud ;
struct noeud {
bool est_dossier ;
char nom [100];
struct noeud * pere ;
struct noeud * racine ;
struct liste_noeud * fils ;
};
struct liste_noeud {
struct noeud * no ;
struct liste_noeud * succ ;
};
typedef struct noeud noeud ;
typedef struct liste_noeud liste_noeud ;
/*-------------------------------------------*/
char* capture(const char* nom) {
    char* result = malloc(strlen(nom) + 1); // Allouer de la mémoire pour la chaîne de caractères de retour
    if (result != NULL) { // Vérifier si l'allocation a réussi
        strcpy(result, nom); // Copier la chaîne de caractères passée en paramètre dans la variable de retour
    }
    return result; // Retourner la chaîne de caractères
}
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
noeud* creerRacine(liste_noeud * fils){
       noeud* r=creerNoeud(true,"",NULL,fils,NULL);
       r->pere=r;
       r->racine=r;
       return r;
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
}
/*-------------------------------------------*/
void ls(noeud* courant){
	liste_noeud* fils=courant->fils;
	while(fils!=NULL){
		printf("%s\n",fils->no->nom);
		fils=fils->succ;
	}
}
/*-------------------------------------------*/
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
                break;
            }
        } while (token != NULL);
        free(chemin);
    } else {
        perror("No such file or directory");
    }
    return res;
}
/*-------------------------------------------*/
noeud* cd_chem(noeud* courant, const char* chem){
	//Si c'est un chemin absolu
	noeud* res=courant;
    char* c=capture(chem);
    if(c[0]=='/'){
       if(c[1]!='/'){
       	 res = courant->racine;
       	 if(courant==getDernier(courant->racine,chem)) {
	        perror("Vous etes déjà dans ce dossier");
	        return courant;
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
                break;
            }
         } while (token != NULL);
         free(chemin);
       } 
       else {
         perror("No such file or directory");
       }
      }
      else{
     	perror("No such file or directory");
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
                break;
            }
        } while (token != NULL);
        free(chemin);
      } 
      else {
         perror("No such file or directory");
      }
     }
     else{
     	perror("No such file or directory");
     }
    }
    free(c);
    return res;
}
/*-------------------------------------------*/
noeud* cd(noeud* courant){
    while(courant->pere != courant->racine){
        courant=courant->pere;
    }
    return courant;
}
/*-------------------------------------------*/
noeud* cd_point(noeud* courant){
    if(courant->pere==NULL){ perror("No such file or directory");}
    if(courant->pere!=NULL){ return courant->pere;}
    return NULL;
}
/*-------------------------------------------*/
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
/*---------------------------------------------------------------*/
void mkdir(noeud* parent, const char *nom) {
    if(estValide(nom) && !existeDeja(parent,nom)){
    char *n=capture(nom);
    noeud *newDir=creerNoeud(true,n,parent,NULL,parent->racine);
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
    free(n);
    }
    else{
        perror("Ce dossier existe déjà!");
    }
}
/*-------------------------------------------*/

void touch(noeud* courant,const char *nom){
if(estValide(nom) && !existeDeja(courant,nom)){
char *c=capture(nom);
noeud* newFic=creerNoeud(false,c,courant,NULL,courant->racine);
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
free(c);
}
else{
	perror("Le nom est invalide ou un fichier portant ce nom existe déjà dans ce dossier.");
}
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
    // Libérer et supprimer tous les fils de courant
    liste_noeud* fils_courant = courant->fils;
    while (fils_courant != NULL) {
        noeud* fils = fils_courant->no;
        // Appeler récursivement la fonction pour libérer et supprimer les sous-arbres des fils
        free_chem(fils);
        // Libérer le noeud fils et sa liste de fils
        free(fils->fils);
        free(fils);
        // Passer au fils suivant
        fils_courant = fils_courant->succ;
    }
    // Réinitialiser la liste de fils du noeud courant
    courant->fils = NULL;
}
/*-------------------------------------------*/
void rm(noeud* courant,const char* chem){
    noeud* toDelete = courant;

    //Si c'est un chemin absolu    
    char* c=capture(chem);
    if(c[0]=='/'){
       if(c[1]!='/'){
       	 toDelete = courant->racine;
       	 if(courant==getDernier(courant->racine,chem)) {
	        perror("Vous etes déjà dans ce dossier");
	        return;
	     }
    char* chemin = capture(chem);
    if (toDelete->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(toDelete, token)) {
                toDelete = getAppartient(toDelete, token);
                token = strtok(NULL, "/");
            } else {
                toDelete=courant;
                perror("No such file or directory");
                free(chemin);
                return;
            }
        } while (token != NULL);
        free(chemin);
    } else {
        perror("No such file or directory");
    }
    liste_noeud* fils_de_pere=toDelete->pere->fils;
    free_chem(toDelete);
    liste_noeud* prev_fils = NULL;
    while(fils_de_pere != NULL){
        if(fils_de_pere->no == toDelete){
            if(prev_fils == NULL){ // toDelete est en tête de la liste
                toDelete->pere->fils = fils_de_pere->succ;
            } else { // toDelete est dans la liste, mais pas en tête
                prev_fils->succ = fils_de_pere->succ;
            }
            free(fils_de_pere);
            break;
        }
        prev_fils = fils_de_pere;
        fils_de_pere = fils_de_pere->succ;
    }
    }
    else{
     	perror("No such file or directory");
      }
    }
    //Si ce n'est pas un chemin absolu
    else{
    if(c[1]!='/'){
    	char* chemin = capture(chem);
    if (toDelete->fils != NULL && chemin != NULL) {
        char* token = strtok(chemin, "/");
        do {
            if (appartient(toDelete, token)) {
                toDelete = getAppartient(toDelete, token);
                token = strtok(NULL, "/");
            } else {
                toDelete=courant;
                perror("No such file or directory");
                free(chemin);
                return;
            }
        } while (token != NULL);
        free(chemin);
    } else {
        perror("No such file or directory");
    }
    liste_noeud* fils_de_pere=toDelete->pere->fils;
    free_chem(toDelete);
    liste_noeud* prev_fils = NULL;
    while(fils_de_pere != NULL){
        if(fils_de_pere->no == toDelete){
            if(prev_fils == NULL){ // toDelete est en tête de la liste
                toDelete->pere->fils = fils_de_pere->succ;
            } else { // toDelete est dans la liste, mais pas en tête
                prev_fils->succ = fils_de_pere->succ;
            }
            free(fils_de_pere);
            break;
        }
        prev_fils = fils_de_pere;
        fils_de_pere = fils_de_pere->succ;
    }
    }
    else{
    	perror("No such file or directory");
    }
    }
    free(c);
}
/*-------------------------------------------*/
int main(void){
noeud* racine=creerRacine(NULL);
noeud* courant=racine;
mkdir(racine,"TD1");
mkdir(racine,"TD2");
mkdir(racine,"TD3");
mkdir(racine,"TD4");
mkdir(racine->fils->no,"anglais");
mkdir(racine->fils->no->fils->no,"TP1");
ls(courant);
courant=cd_chem(courant,"/TD1/anglais");
printf("---------------\n");
ls(courant);
printf("---------------\n");
rm(courant,"TP1");
ls(courant);
}