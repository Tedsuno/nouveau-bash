#ifndef OTHERS_H
#define OTHERS_H
#include <stdbool.h>
#include <ctype.h>
char* capture(const char* nom);
bool estValide(const char *nom);
char* chemin_sans_dernier(const char* chem);
char* chemin_dernier(const char* chem);
#endif