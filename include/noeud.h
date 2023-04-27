#ifndef NOEUD_H
#define NOEUD_H

#include <stdbool.h>

#define MAX_NAME_LENGTH 100

struct noeud {
    bool est_dossier;
    char nom[MAX_NAME_LENGTH];
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
//Prototypes
noeud* creerNoeud(bool est_dos, const char* nom, noeud* pere,liste_noeud * fils, noeud* racine);
noeud* creerRacine();
bool existeDeja(noeud* courant,const char *nom);
bool appartient(noeud* courant, const char* chem);
noeud* getAppartient(noeud* courant, const char* chem);
bool estDernier(noeud* courant,const char* chem);
noeud* getDernier(noeud* courant,const char* chem);
int nb_fils(noeud* courant);
void print_arbre(noeud* courant);
void free_chem(noeud* courant);
void free_noeud(noeud* n);
noeud* rechercher_noeud(noeud* courant,char* chem);
noeud* copier_noeud(noeud* src);
void ajouter_fils(noeud* parent, liste_noeud* fils);
#endif /* NOEUD_H */