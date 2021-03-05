#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

//structura pentru datele jucatorului
typedef struct player
{
    int ply[10][10]; //harta jucatorului
    int nave[4]; //numarul de nave inca existente
} ply;

//functie pentru salvarea datelor
//in cazul in care se iese din joc
//fara ca acesta sa se fi terminat 
void salvare_date(ply v1, ply v2)
{
    //se deschid, respectiv creeaza fisierele unde se salveaza
    FILE *fisier1 = fopen("resume_grill_player.txt", "w+");
    FILE *fisier2 = fopen("resume_grill_computer.txt", "w+");
    int i, j;
    //se printeaza in fisiere hartile jucatorului
    //si a PC-ului 
    for (i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
            fprintf(fisier1, "%d ", v1.ply[i][j]);
            fprintf(fisier2, "%d ", v2.ply[i][j]);
        }
        fprintf(fisier1, "\n");
        fprintf(fisier2, "\n");
    }

    //se printeaza datele despre navele inca existente a fiecarui jucator
    for(i = 0; i < 4; i++)
    {
        fprintf(fisier1, "%d ", v1.nave[i]);
        fprintf(fisier2, "%d ", v2.nave[i]);
    }

    fclose(fisier1);
    fclose(fisier2);
}

int main(int argc, char **argv)
{
    //se verifca daca au fost date argumente la rularea executabilului
    //in caz contraar, se afiseaza un mesaj de eroare si se intoarce 1 
    if(argc == 1)
    {
        printf("[Eroare]: Nu s-au adaugat argumente de comanda.\n");
        return 1;
    }

    ply player_1, computer;
    srand(time(0));

    //se citesc datele pentru meniul principal
    char **sir;
    citire_meniu(&sir);

    initscr();
    noecho();
    cbreak();

    //se genereaza combinatii de culori
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_GREEN);
    init_pair(2, COLOR_CYAN, COLOR_BLUE);
    init_pair(3, COLOR_RED, COLOR_MAGENTA);

    //se declara fereastra unde se va afisa meniul
    WINDOW *menuwin = newwin(31, 31, 10, 10);
    wbkgd(menuwin, COLOR_PAIR(1));
    keypad(menuwin, true);

    //se afiseaza meniul si se retine alegerea
    int k = afisare_meniu(&menuwin, &sir, 3, 0);
    int file;
    FILE *grill;

    switch (k)
    {
    //daca s-a ales "New" se afiseaza hartile valabile
    //de asemenea, se ofera optiunea de Random
    //in cazul acesteia, se alege o harta aletorie 
    case 0:
        file = afisare_meniu(&menuwin, &argv, argc, 1);
        if(file == argc)
        {
            file = random_generate(argc - 1, 0, 1);
        }
        int p = input_date(player_1.ply, player_1.nave, argv[file]);
        //daca fisierul nu poate fi deschis, se afiseaza un mesaj de eroare
        if(p == 0)
        {
            endwin();
            printf("[Eroare]: Fisierul %s nu s-a putut deschide", argv[file]);
            return 1;
        }
        //harta computerului se alege random
        //astfel incat sa fie diferita de cea a jucatorului 
        file = random_generate(argc - 1, file, 1);
        p = input_date(computer.ply, computer.nave, argv[file]);
        //daca fisierul nu poate fi deschis, se afiseaza un mesaj de eroare
        if(p == 0)
        {
            endwin();
            printf("[Eroare]: Fisierul %s nu s-a putut deschide", argv[file]);
            return 1;
        }

        break;
    
    //daca s-a ales "Resume" se verifica daca exista o salvare anterioara
    case 1:
        grill = fopen("resume_grill_player.txt", "r");
        if(grill == NULL)
        {
            //daca nu exista nicio salvare, se ofera optiunea de a incecpe un joc nou
            //in cazul unui raspuns afirmativ, se reia algoritmul optiunii "New"
            mvprintw(0, 0, "Nu exista nicio salvare anterioara.\nDoriti sa incepeti un joc nou? (y/n)");
            refresh();
            char d = getch();
            if(d == 'y')
            {
                clear();
                file = afisare_meniu(&menuwin, &argv, argc, 1);
                if(file == argc)
                {
                    file = random_generate(argc - 1, 0, 1);
                }
                input_date(player_1.ply, player_1.nave, argv[file]);
                file = random_generate(argc - 1, file, 1);
                input_date(computer.ply, computer.nave, argv[file]);
            }
            else
            {
                //daca raspunsul este negativ, se inchide jocul
                clear();
                endwin();
                return 0;
            }
        }
        else
        {
            //daca exista o salvare anterioara, se introduc datele
            input_date(player_1.ply, player_1.nave, "resume_grill_player.txt");
            input_date(computer.ply, computer.nave, "resume_grill_computer.txt");
            refresh();
            fclose(grill);
            mvprintw(0, 0, "Press any key to continue");
            refresh();
            getch();
            clear();
        }

        break;
    //daca s-a ales optiunea "Quit", se afiseaza un mesaj de iesire
    //si se inchide jocul
    case 2:
        clear();
        endwin();
        printf("Multumim pentru timpul petrecut.");
        printf("Va mai asteptam.\n");
        return 0;

    
    default:
        break;
    }
    
    //se inchide fereastra meniului
    keypad(menuwin, false);
    delwin(menuwin);
    clear();

    //se declara ferestrele pentru afisarea hartilor playerului si a PC-ului
    //si o fereastra pentru afisarea informatiilor suplimentare
    WINDOW *playerwin = newwin(21, 22, 10, 10), *computerwin = newwin(21, 22, 10, 35);
    WINDOW *scortabel = newwin(21, 52, 10, 60);
    
    //se aplica contur pentru fiecare fereastra
    box(scortabel, 0, 0);
    box(playerwin, 0, 0);
    box(computerwin, 0, 0);

    int raspuns, ban = rand() % 2; //se da cu banul pentru a vedea cine incepe
    char c;
    int i, j;
    keypad(computerwin, true);
    do
    {
        //se afiseaza hartile si tabela pentru informatii
        afisare_inamic(computer.ply, &computerwin);
        afisare_harta(player_1.ply, &playerwin);
        mvprintw(32, 11, "Harta player");
        mvprintw(32, 36, "Harta computer");
        mvwprintw(scortabel, 5, 1, "Folositi sagetile pentru a va misca");
        mvwprintw(scortabel, 6, 1, "O inseamna o casuta unde este apa");
        mvwprintw(scortabel, 7, 1, "V inseamna o casuta unde se afla o nava");
        mvwprintw(scortabel, 8, 1, "X inseamna o nava lovita");
        mvwprintw(scortabel, 9, 1, "A inseamna o casuta lovita cu apa");

        //se verifica al cui este randul
        //si se afiseaza pe ecran
        if(ban == 1)
        {
            mvwprintw(scortabel, 1, 1, "||||||Este randul jucatorului||||||");
        }
        if(ban == 0)
        {
            mvwprintw(scortabel, 1, 1, "|||||Este randul PC-ului|||||");
        }
        mvwprintw(scortabel, 2, 1, "|||||||Press Enter|||||||");
        mvwprintw(scortabel, 3, 1, "                            ");

        refresh();
        wrefresh(playerwin);
        wrefresh(computerwin);
        wrefresh(scortabel);
        getch();

        //in cazul in care este randul jucatorului, i se permite sa atace
        //procesul se repeta pana cand jucatorul alege un spatiu
        //care nu a fost atacat deja
        if(ban == 1)
        {
            do
            {
                navigare(computer.ply, &computerwin, &raspuns);
                if(raspuns == 1)
                {
                    mvwprintw(scortabel, 1, 1, "Ati nimerit un vas                  ");
                }
                if(raspuns == 0)
                {
                    mvwprintw(scortabel, 1, 1, "Ati nimerit un spatiu gol           ");
                }
            } while (raspuns == 2 || raspuns == 3);
              
        }

        //in cazul in care este randul calculatorului 
        if(ban == 0)
        {
            //se genreaza coordonate aleatorii pana cand se gaseste o pozitie
            //care nu a fost inca atacata
            do
            {
                i = rand() % 10;
                j = rand() % 10;
                raspuns = player_1.ply[i][j];
            } while (player_1.ply[i][j] == 2 || player_1.ply[i][j] == 3);
            
            //dupa ce s-a gasit acesta casuta
            //se marcheaza ca fiind atacata
            //in functie de ce se afla acolo inainte
            if(raspuns == 1)
            {
                player_1.ply[i][j] = 2;
            }
            if(raspuns == 0)
            {
                player_1.ply[i][j] = 3;
            }
            //se afiseaza pozitia unde a lovit calculatorul
            //si ce se afla acolo
            if(raspuns == 1)
            {
                mvwprintw(scortabel, 1, 1, "Acesta a lovit o nava la %d:%d    ", i, j);
            }
            else
            {
                mvwprintw(scortabel, 1, 1, "Acesta a lovit un spatiu gol la %d:%d    ", i, j);
            }
            

        }
        
        //se ofera indicatii pentru optiunile valabile
        //se afiseaza hartile cu noile modificari
        mvwprintw(scortabel, 2, 1, "Press any key to continue");
        mvwprintw(scortabel, 3, 1, "Press q to quit");
        afisare_harta(player_1.ply, &playerwin);
        afisare_inamic(computer.ply, &computerwin);
        refresh();
        wrefresh(playerwin);
        wrefresh(computerwin);
        wrefresh(scortabel);
        //se retine tasta apasata
        c = getch();

        //se schimba randul
        switch (ban)
        {
        case 0:
            //daca este randul calculatorului si nu a nimerit
            //se trece la randul jucatorului 
            if(raspuns == 0)
            {
                ban = 1;
            }
            
            break;
        
        case 1:
            //daca este randul jucatorului si nu a nimerit
            //se trece la randul calculatorului
            if(raspuns == 0)
            {
                ban = 0;
            }
            
            break;

        default:
            break;
        }

        //se verifica daca jucatorul mai are nave care nu au fost distruse
        if(verificare_harta(player_1.ply) == 0)
        {
            //in caz afirmativ, se afiseaza un mesaj si se iese din joc
            mvwprintw(scortabel, 11, 1, "Castigator: Calculatorul");
            mvwprintw(scortabel, 12, 1, "Ne pare rau, ati pierdutut. Mult noroc data viitoare");
            mvwprintw(scortabel, 13, 1, "Press any key to exit");
            getch();
            clear();
            endwin();
            break;
        }

        //daca jucatorul nu a pierduta, se repeta procesul pentru calculator
        if(verificare_harta(computer.ply) == 0)
        {
            //in caz afirmativ, se afiseaza un mesaj si se iese din joc
            mvwprintw(scortabel, 11, 1, "Castigator: Playerul");
            mvwprintw(scortabel, 12, 1, "Felicitari, ati castigat!\n");
            mvwprintw(scortabel, 13, 1, "Press any key to exit");
            clear();
            endwin();
            break;
        }

    //daca jocul nu s-a terminat, insa jucatorul nu doreste sa continue
    //adica a apasat tasta "q"
    //se iese din joc
    } while (c != 113);

    //in cazul in care s-a iesit din joc fara ca acesta sa se fi terminat
    //se creeaza o salvare
    if(verificare_harta(player_1.ply) == 1 && verificare_harta(computer.ply) == 1)
    {
        salvare_date(player_1, computer);
    }
    
    //se inchide interfata grafica si se afiseaza un mesaj
    endwin();
    printf("Multumim pentru timpul petrecut.");
    printf("Va mai asteptam.\n");

    //se elibereaza memoria folosita pentru meniu
    free_all(&sir, 3);
    
    return 0;
}