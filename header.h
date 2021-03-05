#ifndef header
#define A

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int suma(int a);

int input_date(int v[10][10], int nave[4], char *sir);

int afisare_meniu(WINDOW **menuwin, char ***sir, int n, int p);

void citire_meniu(char ***sir);

void free_all(char ***sir, int n);

void afisare_harta(int v[10][10], WINDOW **harta);

void afisare_inamic(int v[10][10], WINDOW **win);

char navigare(int v[10][10], WINDOW **harta, int *raspuns);

int random_generate(int n, int diff, int i);

int verificare_harta(int v[10][10]);

#endif