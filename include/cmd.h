#ifndef CMD_H
#define CMD_H

#include "noeud.h"
extern void ls(noeud* courant);
extern noeud* cd_chem(noeud* courant, const char* chem);
extern noeud* cd(noeud* courant);
extern noeud* cd_point(noeud* courant);
extern void pwd(noeud* courant);
extern void mkdir(noeud* parent, const char *nom);
extern void touch(noeud* courant, const char* nom);
extern void rm(noeud* courant,const char* chem);
extern void cp(noeud* courant,char* chemin_src, char* chemin_dest);
extern void mv(noeud* courant,char* chem1, char* chem2);
#endif