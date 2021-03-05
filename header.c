#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <curses.h>
#include "header.h"


//functie pentru calculul unei functii
//aceasta ajuta la pozitionarea punctelor pe harta
//pentru a un spatiu intre fiecare spatiu
int suma(int a)
{
    if( a == 0 )
    {
        return 1;
    }
    else
    {
        return suma(a - 1) + 2;
    }
}

//functie pentru introducerea datelor de joc
int input_date(int v[10][10], int nave[4], char *sir)
{
    //se deschide fisierul unde sunt salvate datele
    int i, j;
    FILE *input_date = fopen(sir, "r");
    //daca fisierul nu poate fi deschis, se intoarce 0
    if(input_date == NULL)
    {
        return 0;
    }
    else
    {
        //in caz contrar, se introduc datele
        for(i = 0; i < 10; i++)
        {
            for(j = 0; j < 10; j++)
            {
                fscanf(input_date, "%d", &v[i][j]);
            }
        }
        for(i = 0; i < 4; i++)
        {
            fscanf(input_date, "%d", &nave[i]);
        }
    }
    //se inchide fiserul deschis
    fclose(input_date);

    //daca totul decurge cum trebuie, se intoarce valoarea 1 
    return 1;    
}

//functie pentru afisarea meniului
int afisare_meniu(WINDOW **menuwin, char ***sir, int n, int p)
{
    //p este folosit pentru a se afisa datele de la o anumita intrare a sirului
    //se creaza bordura pentru meniu
    box(*menuwin, 0, 0);
    refresh();

    int choice, highlight = p;

    while(1)
    {
        //se declare pozitia de unde incepe sa se afiseze
        int k = 1;
        int i;
        for(i = p; i < n; i++)
        {
            //daca pozitia actuala coincide cu optiunea unde ma aflu
            //se face reverse pe respectivul rand
            //pentru a se distinge optiunea
            //la care se afla acum jucatorul 
            if(highlight == i)
            {
                wattron(*menuwin, A_REVERSE);
            }
            //se afiseaza optiunea
            mvwprintw(*menuwin, k, 1, (*sir)[i]);
            //se anuleaza revers-ul
            wattroff(*menuwin, A_REVERSE);
            k++;
        }

        //daca se afiseaza meniul din sir incepand cu pozitia 1
        //se ofera si optiunea de Random
        //pentru ca in cazul asta se afiseaza meniul cu hartile valabile 
        if(p == 1)
        {
            if(highlight == n)
            {
                wattron(*menuwin, A_REVERSE);
            }
            mvwprintw(*menuwin, k, 1, "Random");
            wattroff(*menuwin, A_REVERSE);
        }

        //se preia input-ul utilizatorului
        //de la tastatura
        choice = wgetch(*menuwin);

        switch(choice)
        {
            //daca se introduce KEY_UP
            //cursorul se duce in sus
            case KEY_UP:
            {
                highlight--;
                //daca se afla deja la prima optiune si nu mai poate merge in sus
                //se intoarce la ultima optiune
                if(highlight < p)
                {
                    highlight = n - 1 + p;
                }
                break;
            }
            //daca se introduce KEY_DOWN
            //cursorul se duce in jos
            case KEY_DOWN:
            {
                highlight++;
                //daca se afla deja la ultima optiune si nu mai poate merge in jos
                //se intoarce la prima optiune
                if(highlight > n - 1 + p)
                {
                    highlight = p;
                }
                break;
            }
            default:
            {
                break;
            }
        }

        //daca sa introduce tasta enter
        //se afiseaza optiunea aleasa 
        if(choice == 10)
        {
            if(p == 1 && highlight == n - 1 + p)
            {
                mvwprintw(*menuwin, n + 1, 1, "Ati ales optiunea Random");
            }
            else
            {
                mvwprintw(*menuwin, n + 1, 1, "Ati ales %s", (*sir)[highlight]);
            }
            
            mvwprintw(*menuwin, n + 2, 1, "Press any key to continue");
            break;
        }
    }

    //se asteapta un input de de utilizator
    //pentru a se continua 
    wrefresh(*menuwin);
    getch();
    wclear(*menuwin);

    return highlight;
}

//functie pentru citirea meniului
void citire_meniu(char ***sir)
{
    FILE *meniu;
    meniu = fopen("meniu_principal.txt", "r");
    int i, n;
    fscanf(meniu, "%d", &n);
    (*sir) = (char **) malloc(n * sizeof(char *));
    for(i = 0; i < n; i++)
    {
        (*sir)[i] = (char *) malloc(10 * sizeof(char));
        fscanf(meniu, "%s", (*sir)[i]);
    }
    fclose(meniu);
} 

//functie pentru eliberarea memoriei alocate
//unui sir 
void free_all(char ***sir, int n)
{
    int i;
    for(i = 1; i < n; i++)
    {
        free((*sir)[i]);
    }
    free(*sir);
}

//functie pentru afisarea hartii jucatorului
void afisare_harta(int v[10][10], WINDOW **harta)
{
    //se coloreaza fereastra unde se afiseaza
    wbkgd(*harta, COLOR_PAIR(3));
    int i, j;
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
            //in functie de ce se afla la pozitia respectiva
            //se afiseaza un caracter simbolic
            switch (v[i][j])
            {
            case 0:
                mvwprintw(*harta, suma(i), suma(j), "O");
                break;

            case 1:
                mvwprintw(*harta, suma(i), suma(j), "V");
                break;

            case 2:
                mvwprintw(*harta, suma(i), suma(j), "X");
                break;
            
            case 3:
                mvwprintw(*harta, suma(i), suma(j), "A");
            
            default:
                break;
            }   
        }
    }
}

//functie pentru afisarea hartii inamicului
void afisare_inamic(int v[10][10], WINDOW **win)
{
    //se coloreaza fereastra unde se afiseaza
    wbkgd(*win, COLOR_PAIR(2));
    int i, j;
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
            //in functie de ce se afla la pozitia respectiva
            //se afiseaza un caracter simbolic
            //daca pozitia respectiva trebuie afisata
            if(v[i][j] == 2)
            {
                mvwprintw(*win, suma(i), suma(j), "X");
            }
            if(v[i][j] == 3)
            {
                mvwprintw(*win, suma(i), suma(j), "A");
            }
        }
    }
}

//functie pentru navigare in harta inamicului
//procesul se aseamana cu cel al functiei afiare_meniu
char navigare(int v[10][10], WINDOW **harta, int *raspuns)
{
    box(*harta, 0, 0);
    wbkgd(*harta, COLOR_PAIR(2));
    int choice, highlight_x = 0, highlight_y = 0;
    while(1)
    {
        int i, j;

        for(i = 0; i < 10; i++)
        {
            for(j = 0; j < 10; j++)
            {
                if(highlight_x == i && highlight_y == j)
                {
                    wattron(*harta, A_REVERSE);
                }

                if(v[i][j] == 2)
                {
                    mvwprintw(*harta, suma(i), suma(j), "X");
                }
                else
                {
                    if(v[i][j] == 3)
                    {
                        mvwprintw(*harta, suma(i), suma(j), "A");
                    }
                    else
                    {
                        mvwprintw(*harta, suma(i), suma(j), " ");
                    }
                    
                }
                
                wattroff(*harta, A_REVERSE);
            }
        }

        choice = wgetch(*harta);

        switch (choice)
        {
        //in functie de input, se mutra cursorul in directie indicata
        case KEY_UP:
            highlight_x--;
            if(highlight_x < 0)
            {
                highlight_x = 9;
            }

            break;

        case KEY_DOWN:
            highlight_x++;
            if(highlight_x > 9)
            {
                highlight_x = 0;
            }

            break;

        case KEY_LEFT:
            highlight_y--;
            if(highlight_y < 0)
            {
            highlight_y = 9;
            }

            break;

       case KEY_RIGHT:
            highlight_y++;
            if(highlight_y > 9)
            {
                highlight_y = 0;
            }

            break;

        default:
            break;
        }

        //daca se apasa enter
        //se pastraza ce se afla initial in casuta raspectiva
        //si i se schimba valoarea
        if(choice == 10)
        {
            *raspuns = v[highlight_x][highlight_y];
            if(v[highlight_x][highlight_y] == 1)
            {
                v[highlight_x][highlight_y] = 2;
            }
            if(v[highlight_x][highlight_y] == 0)
            {
                v[highlight_x][highlight_y] = 3;
            }

            break;
        }
    }

    //se da refesh la fereastra
    //si se asteapta un input de la jucator
    refresh();
    wrefresh(*harta);
    char c = getch();

    return c;
}


//functie pentru generarea aleatorie a unui numar
//care se afla cuprins intre n si i
//si este diferit de diff
int random_generate(int n, int diff, int i)
{
    int p;
    do
    {
        p = (rand() % n) + i;
    } while (p == diff);
    
    return p;
}

//fuunctie pentru a verifica daca o harta
//mai este valida
int verificare_harta(int v[10][10])
{
    int i, j, k = 0;
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
            if(v[i][j] == 1)
            {
                k = 1;
            }
        }
    }

    return k;
}