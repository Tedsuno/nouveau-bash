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
extern noeud* creerNoeud(bool est_dos, const char* nom, noeud* pere,liste_noeud * fils, noeud* racine);
extern noeud* creerRacine();
extern bool existeDeja(noeud* courant,const char *nom);
extern bool appartient(noeud* courant, const char* chem);
extern noeud* getAppartient(noeud* courant, const char* chem);
extern bool estDernier(noeud* courant,const char* chem);
extern noeud* getDernier(noeud* courant,const char* chem);
extern int nb_fils(noeud* courant);
extern void print_arbre(noeud* courant);
extern void free_chem(noeud* courant);
extern void free_noeud(noeud* n);
extern noeud* rechercher_noeud(noeud* courant,char* chem);
extern noeud* copier_noeud(noeud* src);
extern void ajouter_fils(noeud* parent, liste_noeud* fils);
extern char* NomRacine(noeud* courant);
extern bool estParent(noeud* courant, noeud* parent);
extern bool appartient_sous_arbre(noeud* racine, noeud* noeud_a_chercher);
#endif /* NOEUD_H */