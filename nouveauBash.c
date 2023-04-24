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
char* capture(const char* nom){
    char* result= malloc(strlen(nom)+1);
    if(result!=NULL){
        strcpy(result,nom);
    }
    return result;
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
noeud* creerRacine(){
       noeud* r=creerNoeud(true,"",NULL,NULL,NULL);
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
/*-------------------------------------------*/
noeud* cd_chem(noeud* courant, const char* chem){
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
noeud* cd(noeud* courant){
    return courant->racine;
}
/*-------------------------------------------*/
noeud* cd_point(noeud* courant){
    if(courant->pere==NULL){ perror("No such file or directory"); exit(EXIT_FAILURE);}
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
        strcpy(chaine2, chaine);
        strcpy(chaine, "");
        if (strlen(c->nom) > 0) {
            strcat(chaine, "/");
            strcat(chaine, c->nom);
        }
        strcat(chaine, chaine2);
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
        exit(EXIT_FAILURE);
    }
}
/*-------------------------------------------*/
void touch(noeud* courant, const char* nom) {
    if (estValide(nom) && !existeDeja(courant, nom)) {
        char* c = capture(nom);
        noeud* newFic = creerNoeud(false, c, courant, NULL, courant->racine);
        if (courant->fils == NULL) {
            courant->fils = malloc(sizeof(liste_noeud));
            courant->fils->no = newFic;
            courant->fils->succ = NULL;
        }
        else {
            liste_noeud* liste = courant->fils;
            while (liste->succ != NULL) {
                liste = liste->succ;
            }
            liste->succ = malloc(sizeof(liste_noeud));
            liste->succ->no = newFic;
            liste->succ->succ = NULL;
        }
        free(c);
    }
    else {
        perror("Le nom est invalide ou un fichier portant ce nom existe déjà dans ce dossier.");
        exit(EXIT_FAILURE);
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
void rm(noeud* courant,const char* chem){
    noeud* toDelete = courant;

    //Si c'est un chemin absolu    
    char* c=capture(chem);
    if(c[0]=='/' && courant==courant->racine){
       if(c[1]!='/'){
       	 toDelete = courant->racine;
       	 if(courant==getDernier(courant->racine,chem)) {
	        perror("Vous etes déjà dans ce dossier");
            free(c);
	        exit(EXIT_FAILURE);
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
                free(c);
                exit(EXIT_FAILURE);
            }
        } while (token != NULL);
        free(chemin);
    } else {
        free(chemin);
        free(c);
        perror("No such file or directory");
        exit(EXIT_FAILURE);
    }
    liste_noeud* fils_de_pere=toDelete->pere->fils;
    free_chem(toDelete);
    liste_noeud* prev_fils = NULL;
    while(fils_de_pere != NULL){
        if(fils_de_pere->no == toDelete){
            if(prev_fils == NULL){
                toDelete->pere->fils = fils_de_pere->succ;
            } else {
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
        free(c);
     	perror("No such file or directory");
        exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
            }
        } while (token != NULL);
        free(chemin);
    } else {
        perror("No such file or directory");
        exit(EXIT_FAILURE);
    }
    liste_noeud* fils_de_pere=toDelete->pere->fils;
    free_chem(toDelete);
    liste_noeud* prev_fils = NULL;
    while(fils_de_pere != NULL){
        if(fils_de_pere->no == toDelete){
            if(prev_fils == NULL){
                toDelete->pere->fils = fils_de_pere->succ;
            } else {
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
        exit(EXIT_FAILURE);
    }
    }
    free(c);
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
/*-------------------------------------------*/
void cp(noeud* courant,char* chemin_src, char* chemin_dest) {
    if (chemin_src == NULL || chemin_dest == NULL) {
        return;
        exit(EXIT_FAILURE);
    }
    noeud* src = rechercher_noeud(courant, chemin_src);
    if (src == NULL) {
        printf("Erreur : Le chemin source n'existe pas.\n");
        exit(EXIT_FAILURE);
    }
    noeud* copie_src = copier_noeud(src);
    noeud* dest = rechercher_noeud(courant, chemin_dest);
    if (dest == NULL) {
        printf("Erreur : Le chemin destination n'existe pas.\n");
        free_noeud(copie_src);
        exit(EXIT_FAILURE);
    }
    ajouter_fils(dest, copie_src);
}
/*--------------------------------------------------------*/
void mv(noeud* courant,char* chem1, char* chem2){
    cp(courant,chem1,chem2);
    rm(courant,chem1);
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
   char temp[100];
   char *token[100];
   char *token2;
   char *token3[100];
   FILE* flux = fopen(argv[1], "r");
    if (flux == NULL) {
    perror("Probleme ouverture de fichier");
   } else {
    int c = 0;
    while (fgets(ch, 50, flux) != NULL) {
        strcpy(temp, ch);
        token[c] = strtok(temp, " ");
        token3[c] = temp;
        int lentok=strlen(token[c]);
        token2=token[c]; 
        token2 = strchr(ch, ' '); 
        if (token2 != NULL) {   
        token2++;
        int len=strlen(token2);
        if (len > 0) {
        token2[len - 1] = '\0';
        }        
    }
    if(strcmp(token[c],"mkdir")==0){
            mkdir(courant,token2);
    }
    if(strcmp(token[c],"mv")==0){
           char *str1 = NULL;
           char *str2 = NULL;
           char *token_bis = strtok(token2, " ");
           if (token_bis != NULL) {
              str1 = strdup(token_bis);
              token_bis = strtok(NULL, " ");
              if (token != NULL) {
              str2 = strdup(token_bis);
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
              if (token != NULL) {
              str2 = strdup(token_bis);
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
    if(strcmp(token3[c],"pwd")==0){
            pwd(courant);
    }
    if(strcmp(token3[c],"touch")==0){
            touch(courant,token2);
    }    
    if(strlen(token3[c])==3){
    	if(strcmp(token3[c],"cd\n")==0) courant=cd(courant);
        if(strcmp(token3[c],"ls\n")==0){
        	ls(courant);
        	puts("-------------");
        }
    }
    if(strcmp(token3[c],"cd")==0){
        if(strcmp(token2,"..")==0) courant=cd_point(courant);
        else { courant=cd_chem(courant,token2); }
    }
    else if(strcmp(token3[c],"cd")!=0 && strcmp(token3[c],"touch")!=0 && strcmp(token3[c],"pwd")!=0 && 
            strcmp(token3[c],"rm")!=0 && strcmp(token3[c],"ls")!=0 && strcmp(token3[c],"cp")!=0
            && strcmp(token3[c],"mv")!=0 && strcmp(token3[c],"mkdir")!=0 && strcmp(token3[c],"cd\n")!=0 && 
            strcmp(token3[c],"ls\n")!=0){
        printf("command not found : %s",token3[c]);
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