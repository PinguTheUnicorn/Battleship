Cerinta 1.
Se deschide o fereastra pentru afisarea si interactionarea cu meniul principal.
Se alege optiunea prin intermediul sagetilor si se selecteaza aceasta prin apasarea tastei enter.
In cazul optiunii "New", se afiseaza un nou meniu de unde se poate alege harta care sa fie folosita.
De asemenea, am adaugat optiunea "Random" pentru cei nehotarati.
Harta computerului este aleasa apoi aleatoriu dintre cele ramase.
In cazul optiunii "Resume" , se verifica daca exista vreo salvarea existenta, caz in care se continua jocul de acolo.
Daca nu, se ofera jucatorului optiunea de a incepe un joc nou. In caz afirmativ, se ajunge la meniul optiunii "New".
Altfel, se iese din joc.
In cazul optiunii "Quit", se iese din joc si se afiseaza un mesaj.

Cerinta 2.
Harta jucatorului este citita diintr-un fisier ales de catre jucator sau aleatoriu.
Harta calculatorului este, de asemeanea citita dintr-un fisier, insa de fiecare data aleatoriu.
Introducerea datelor se face prin intermediul unei functii, dupa ce s-a verificat ca fisierul este valabil.

Cerinta 3.
Dupa ce s-au introdus datele, se incepe jocul.
Se afiseaza harta jucatorului, harta adversarului(doar cu pozitiile deja lovita, daca ele exista).
Se mai afiseaza un tabel cu instructiuni si alegerile facute.
Se trece de la o tura la alta prin intermediul unui input al jucatorului.
Se "da cu banul" pentru a se vedea cine incepe.
Cand este randul jucatorului, i se permite navigarea prin harta adversarului.
Se selecteaza pozitia de atac prin intermediul tastei enter. Procesul continua pana cand jucatorul selecteaza
o casuta neatacata inca sau pana cand loveste o casuta libera.
Cand este randul calculatorului, se genereaza o pozitie aleatorie pana cand aceasta nu este deja lovita.
Procesul se repeta pana cand se nimereste o casuta libera.
La fiecare lovitura, se reafiseaza hartile si se asteapta ca utiliztorul sa ia o decizie.
Decizia este aceea de a continua sau a iesi, cea din urma fiind posibila prin apasarea tastei "q".
Daca se iese, datele se salveaza in doua fisiere, pentru fiecare jucattor.
La fiecare tur se verifica daca harta vreunuia dintre jucatori mai este valida,
adica daca mai exista nave care nu au fost distruse inca.
Daca se intampla acest lucru, se iese din joc, fara a se mai salva vreun progres, si se afiseaza daca jucatorul
a castigat sau nu, dupa care se iese din joc.

Cerinta 4.
Nerezolvata

Cerinta 5.
Dupa ce se termina jocul, este afisat castigatorul si un mesaj pentru jucator.
Se iese apoi din joc.

Bonus:
    -hartile si meniurile sunt colorate
    -sunt afisate actiunile calculatorului si a jucatorului
    -se ofera optiunea de a alege random o harta pentru jucator
    -jocul este creat pentru a fi cat mai interactiv si placut

Makefile:
    -la rularea comenzii make, jocul este compilat
    -la rularea comenzii make clean este sters tot ce a fost creat in urma compilarii jocului
    -la rularea comenzii make run este rulat jocul cu toate hartile care sunt gasite
    -la rularea comenzii make keep_history se elimina tot ce a fost creat in urma compilarii jocului, cu exceptia salvarilor  