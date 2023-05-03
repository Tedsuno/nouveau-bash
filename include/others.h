#ifndef OTHERS_H
#define OTHERS_H
#include <stdbool.h>
#include <ctype.h>
extern char* capture(const char* nom);
extern bool estValide(const char *nom);
extern char* chemin_sans_dernier(const char* chem);
extern char* chemin_dernier(const char* chem);
#endif