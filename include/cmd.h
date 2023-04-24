#ifndef CMD_H
#define CMD_H

#include "noeud.h"
void ls(noeud* courant);
noeud* cd_chem(noeud* courant, const char* chem);
noeud* cd(noeud* courant);
noeud* cd_point(noeud* courant);
void pwd(noeud* courant);
void mkdir(noeud* parent, const char *nom);
void touch(noeud* courant, const char* nom);
void rm(noeud* courant,const char* chem);
void cp(noeud* courant,char* chemin_src, char* chemin_dest);
void mv(noeud* courant,char* chem1, char* chem2);
#endif