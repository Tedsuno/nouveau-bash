#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    g->racine = g;
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

int main(void) {
    noeud* racine = creer_noeud(true, "/", NULL);

    noeud* dossier1 = creer_noeud(true, "Cours", NULL);
    noeud* dossier2 = creer_noeud(true, "anglais", NULL);
    liste_noeud* g = malloc(sizeof(liste_noeud));
    liste_noeud* g2= malloc(sizeof(liste_noeud));
    g2->no = dossier2;
    g2->succ = NULL;
    racine->fils = g2;
    g->no = dossier1;
    g->succ = g2;
    racine->fils = g;

    ls(racine);
    printf("------------\n");
    mkdir(racine,"Nouveau dossier");
    ls(racine);
    return 0;
}