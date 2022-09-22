#include <iostream>
#include <string.h>

using namespace std;

#define WYSOKOSC 21
#define SZEROKOSC 10
#define MAKSYMALNA_ILOSC_STATKOW 10
#define LICZBA_GRACZY 2
#define LICZBA_KLAS 4
#define DLUGOSC_KOMENDY 4
#define WIELKOSC_TABLICY_DOMYSLNEJ 10
#define MAKSYMALNY_ROZMIAR_STATKU 5
#define WIELKOSC_STRUKTURY_STATEK 80
#define WIELKOSC_STRUKTURY_STATEK_DLA_JEDNEGO_GRACZA 40

struct Statek
{
    char gracz, kierunek;
    int wielkosc, numer;
    int x[MAKSYMALNY_ROZMIAR_STATKU] = { -1,-1,-1,-1,-1 };
    int y[MAKSYMALNY_ROZMIAR_STATKU] = { -1,-1,-1,-1,-1 };
    bool z[MAKSYMALNY_ROZMIAR_STATKU] = { 1,1,1,1,1 };  //czy trafiony 1-nie 0-tak
    bool czy_postawiony = 0;
    int ile_razy_sie_ruszyles = 0;
    int ile_razy_sie_strzeliles = 0;
    Statek(int arg_wielkosc, int arg_numer, char arg_gracz)
    {
        this->wielkosc = arg_wielkosc;
        this->numer = arg_numer;
        this->gracz = arg_gracz;
    }
};
struct Tablica
{
    int szerokosc, wysokosc;
    char** mapa;
    Tablica(int arg_szerokosc, int arg_wysokosc)
    {
        szerokosc = arg_szerokosc;
        wysokosc = arg_wysokosc;
        mapa = new char* [wysokosc];
        for (int i = 0; i < wysokosc; i++)
        {
            mapa[i] = new char[szerokosc];
        }

    }
    //~Tablica()
    //{
    //    for (int g = 0; g < szerokosc; g++)
    //    {
    //        delete[] mapa[g];//tutaj wywala
    //    }
    //    delete[] mapa;
    //    mapa = NULL;
    //}

};

int klasa_statku(char a, char b, char c);
const char* zamien_spowrotem(int wielkosc);
void zapis(Tablica& tab, Statek** tablicastatkow, char* tab_domyslne);

//ZABEZPIECZENIA /
int kod_bledu(int y, int x, char kierunek, int numerx, int klasa, Statek** tablicastatkow, char gracz, char* tab_domyslne, Tablica& tab);//ERROR CODE
void wyswietl_blad(int y, int x, char kierunek, int numerx, int klasa, char gracz, char* c, char* tab_domyslne, int kod);//PRINT ERROR
bool czy_wychodzi_poza_mape(int y, int x, char kierunek, int klasa, char gracz, Tablica& tab, char* tab_domyslne);//CKECKS IF IS MOVING OVER THE BOARD
bool czy_nie_ma_innych_statków(int y, int x, char kierunek, int klasa, Tablica& tab);//CHECKS IF IN THIS LOCATION IS SHIP
bool init_position(char gracz, int x, int y, char* tab_domyslne);//CHECKS IF YOU CAN PLACE SHIP HERE 
bool czy_zwyciestwo(Tablica& tab, char gracz);//CHECKS IF PLAYER WIN

//KOMENDY/COMMANDS
bool rozmiesc_statek(int y, int x, char kierunek, int numerx, int klasa, Tablica& tab, Statek** tablicastatkow, char gracz, char* c, char* tab_domyslne);
char SET_FLEET(char gracz, int a, int b, int c, int d, Statek** tablicastatkow);//COMMAND THAT SET NUMER, AND TYPE OF SHIPS THAT EACH PLAYER HAVE  
void pisztablice(Tablica& tab, Statek** tablicastatkow, char* tab_domyslne);//PRINT BOARD
void pisztablice_1(Tablica& tab, Statek** tablicastatkow, char* tab_domyslne);//PRINT 1
int shoot(int y, int x, Statek** tablicastatkow, Tablica& tab, char gracz);
int shoot_advanced(int numer, int klasa, int y, int x, Statek** tablicastatkow, Tablica& tab, char gracz);


void zeruj_ruchy(Statek** tablicastatkow, char gracz);//SET NUMER OF SHOOTS AND MOVES TO 0
//FUNKCJE ZAAWAANSOWANE/ ADVANCED FUNCTIONS 
int rusz_sie(int numer, int klasa, int gdzie, char gracz, Tablica& tab, Statek** tablicastatkow, char* tab_domyslne);//MAKE MOVE
void usun_plus(int y, int x, char kierunek, Tablica& tab, int wielkosc); // REMOVE LAST POSIOTION OF THE SHIP AFTER MOVE
void radar(Tablica& tab, Statek** tablicastatkow, char gracz, char* tab_domyslne);//USING TO PRINT PLAYER PERSPACTIVE
int rozmiesc_statek_pomocnicza(int y, int x, char kierunek, int numerx, int klasa, Tablica& tab, Statek** tablicastatkow, char gracz, char* tab_domyslne);//HELPING FUNKCTION TO MOVE - USING WITH "PLACE SHIP" TO SET ALL ELEMETS STATUS TO ALIVE | IN SHIP
//MAIN COMMANDS
void state(Statek** tablicastatkow, char* tab_domyslne, Tablica& tab); //COMEND THAT ABLE TO EDIT ROLES OF THE GAME
bool player(char Gracz, Tablica& tab, Statek** tablicastatkow, char* tab_domyslne);//USE THIS COMMAND TO MAKE A TURN AS A PLAYER


int main()
{
    Tablica tab = *(new Tablica(10, 21));
    Statek* tablicastatkow[MAKSYMALNA_ILOSC_STATKOW * LICZBA_GRACZY * LICZBA_KLAS] = { nullptr };
    for (int c = 0; c < tab.wysokosc; c++)
    {
        for (int d = 0; d < tab.szerokosc; d++)
        {
            tab.mapa[c][d] = ' ';
        }
    }
    int quit = 0;
    int domyslne = 0;
    char poprzedni_gracz = ' ';
    char Gracz = 'A';
    char tab_domyslne[25] = { NULL };
    //0-1 (A,B) czy domyslne 3 - EXTENDED 4-kto teraz wykonuje ruch 5wysokosc 6szerokosc tablicy 7- ruch 8-jaki ruch 9-KONIEC PROGRAMU init position 10-18 10-A 11-x1 12-y1 13-y2 14-x2 15-B 16-y1 17-x1 18-y2 19-x2 20-0/1
    char komenda[20];
    while (quit == 0)
    {
        for (int f = 0; f < 20; f++)
            komenda[f] = 0;
        int dlugosc_komendy = 20;

        for (int i = 0; i < dlugosc_komendy; i++)
        {
            cin >> komenda[i];
            if (komenda[0] == '\0')
            {
                quit = 1;
                break;
            }

            //[PLAYER A]
            if (komenda[0] == '[' && komenda[1] == 'p' && komenda[2] == 'l' && komenda[3] == 'a' && komenda[4] == 'y' && komenda[5] == 'e' && komenda[6] == 'r' && komenda[8] == ']' && (komenda[7] == 'A' || komenda[7] == 'B'))
            {
                Gracz = komenda[7];
                dlugosc_komendy = 9;
                if (poprzedni_gracz == Gracz || (tab_domyslne[4] != Gracz && tab_domyslne[4] != NULL))
                {
                    cout << "INVALID OPERATION \"[player" << Gracz << "] \": THE OTHER PLAYER EXPECTED" << endl;
                    quit = 1;
                    break;
                }
                if (tab_domyslne[3] == NULL)
                {
                    poprzedni_gracz = Gracz;
                    if (tab_domyslne[0] != Gracz && tab_domyslne[1] != Gracz)
                    {
                        char e = SET_FLEET(Gracz, 1, 2, 3, 4, tablicastatkow);
                        if (e == 'A')
                            tab_domyslne[0] = 'A';
                        else if (e == 'B')
                            tab_domyslne[1] = 'B';
                    }
                }
                if (player(Gracz, tab, tablicastatkow, tab_domyslne) == 1)
                    quit = 1;
            }
            //[state]
            else if (komenda[0] == '[' && komenda[1] == 's' && komenda[2] == 't' && komenda[3] == 'a' && komenda[4] == 't' && komenda[5] == 'e' && komenda[6] == ']')
            {
                //cout << "state" << endl;
                dlugosc_komendy = 7;
                state(tablicastatkow, tab_domyslne, tab);
                if (tab_domyslne[9] == '1')
                    quit = 1;
                break;

            }
            else if (komenda[0] == 'q' && komenda[1] == 'u' && komenda[2] == 'i' && komenda[3] == 't')
            {
                dlugosc_komendy = 4;
                cout << "wyjscie";
                quit = 1;
                break;

            }
        }
    }
    //Usum wszssystko
    for (int i = 0; i < WIELKOSC_STRUKTURY_STATEK; i++)
    {
        if (tablicastatkow[i] != nullptr)
        {
            delete tablicastatkow[i];
        }
    }
    for (int g = 0; g < tab.wysokosc; g++)
    {
        delete[] tab.mapa[g];
    }
    delete[] tab.mapa;
    tab.mapa = nullptr;
    return 0;
}
const char* zamien_spowrotem(int wielkosc)
{
    if (wielkosc == 5)
        return "CAR";
    else if (wielkosc == 4)
        return "BAT";
    else if (wielkosc == 3)
        return "CRU";
    else if (wielkosc == 2)
        return "DES";
    else
        return "err";
}
int klasa_statku(char a, char b, char c)
{
    int wielkosc = 0;
    if (a == 'C' && b == 'A' && c == 'R')
    {
        wielkosc = 5;
        return wielkosc;
    }
    else if (a == 'B' && b == 'A' && c == 'T')
    {
        wielkosc = 4;
        return wielkosc;
    }
    else if (a == 'C' && b == 'R' && c == 'U')
    {
        wielkosc = 3;
        return wielkosc;
    }
    else if (a == 'D' && b == 'E' && c == 'S')
    {
        wielkosc = 2;
        return wielkosc;
    }
    cout << "zla klasa";
    return 0;
}
bool czy_wychodzi_poza_mape(int y, int x, char kierunek, int klasa, char gracz, Tablica& tab, char* tab_domyslne)
{
    switch (kierunek)
    {
    case 'N':
        for (int i = 0; i < klasa; i++)
        {
            if (y + i > tab.wysokosc)
                return 1;
            else
            {
                if (tab_domyslne[7] != 'M')
                {
                    if (gracz == 'A' && y + i > (tab.wysokosc - 1) / 2)
                        return 1;
                    else if (gracz == 'B' && y + i < (tab.wysokosc - 1) / 2)
                        return 1;
                }
            }

        }
        return 0;
    case 'S':
        for (int i = 0; i < klasa; i++)
            if (y - i < 0)
                return 1;
            else
            {
                if (tab_domyslne[7] != 'M')
                {
                    if (gracz == 'A' && y - i > (tab.wysokosc - 1) / 2)
                        return 1;
                    else if (gracz == 'B' && y - i < (tab.wysokosc - 1) / 2)
                        return 1;
                }
            }
        return 0;
    case 'E':
        for (int i = 0; i < klasa; i++)
            if (x - i < 0 || x - i > tab.szerokosc)
                return 1;
        return 0;
    case 'W':
        for (int i = 0; i < klasa; i++)
            if (x + i > tab.szerokosc || x + i < 0)
                return 1;
        return 0;
    }
    return 1;
}
bool czy_nie_ma_innych_statków(int y, int x, char kierunek, int klasa, Tablica& tab)

{
    int N, jeden, E, W;
    for (int i = 0; i < klasa; i++)
    {
        jeden = 1;
        E = 1;
        W = 1;
        N = i;
        if (kierunek == 'N' || kierunek == 'S')
        {
            if (x + E == tab.szerokosc)
                E = 0;
            if (x - W < 0)
                W = 0;
            if (kierunek == 'N')
            {
                if (y + N >= tab.wysokosc)
                    N = 0;
                if (y - jeden < 0)
                    jeden = 0;
                if (tab.mapa[y + N][x] == '+' || tab.mapa[y - jeden][x] == '+' || tab.mapa[y + N][x + E] == '+' || tab.mapa[y + N][x - W] == '+')
                    return 1;
            }

            if (kierunek == 'S')
            {
                if (y - N < 0)
                    N = 0;
                if (y + jeden == tab.wysokosc)
                    jeden = 0;
                if (tab.mapa[y - N][x] == '+' || tab.mapa[y + jeden][x] == '+' || tab.mapa[y - N][x + E] == '+' || tab.mapa[y - N][x - W] == '+')
                    return 1;
            }
        }
        if (kierunek == 'E' || kierunek == 'W')
        {
            if (y + E == tab.wysokosc)
                E = 0;
            if (y - W < 0)
                W = 0;
            if (kierunek == 'E')
            {
                if (x - N < 0)
                    N = 0;
                if (x + jeden == tab.szerokosc)
                    jeden = 0;
                if (tab.mapa[y][x - N] == '+' || tab.mapa[y][x + jeden] == '+' || tab.mapa[y + E][x - N] == '+' || tab.mapa[y - W][x - N] == '+')
                    return 1;

            }
            if (kierunek == 'W')
            {
                if (x + N == tab.szerokosc)
                    N = 0;
                if (x - jeden < 0)
                    jeden = 0;
                if (tab.mapa[y][x + N] == '+' || tab.mapa[y][x - jeden] == '+' || tab.mapa[y + E][x + N] == '+' || tab.mapa[y - W][x + N] == '+')
                    return 1;

            }
        }

    }
    return 0;
}
bool init_position(char gracz, int x, int y, char* tab_domyslne)
{
    if (tab_domyslne[10] != NULL || tab_domyslne[15] != NULL)
    {
        if (gracz == 'A')
        {
            if (x <= (int)(tab_domyslne[14]) && x >= (int)(tab_domyslne[12]) && y <= (int)(tab_domyslne[13]) && y >= (int)(tab_domyslne[11]))//-48 chyba nie dziala nie wiem czemu??
                return 0;
            else
                return 1;
        }
        else if (gracz == 'B')
        {
            if (x <= (int)(tab_domyslne[19]) && x >= (int)(tab_domyslne[17]) && y <= (int)(tab_domyslne[18]) && y >= (int)(tab_domyslne[16]))//-48 chyba nie dziala nie wiem czemu??
                return 0;
            else
                return 1;
        }
        else
            return 1;
    }
    else
        return 0;
}
int kod_bledu(int y, int x, char kierunek, int numerx, int klasa, Statek** tablicastatkow, char gracz, char* tab_domyslne, Tablica& tab)
{
    int temp = 0;
    if (gracz == 'A')
        temp = 0;
    else if (gracz == 'B')
        temp = 4;

    if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx] == nullptr)//nie został z SETowany
        return 1;
    if (tab_domyslne[10] != 'A' && tab_domyslne[14] != 'B')
    {
        if (czy_wychodzi_poza_mape(y, x, kierunek, klasa, gracz, tab, tab_domyslne) != 0)
            return 2;
    }
    if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->czy_postawiony == 1)//statek już postawiony został
        return 3;
    if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->numer != numerx)//numer statuku nieistnieje
        return 4;
    if (kierunek != 'N' && kierunek != 'S' && kierunek != 'E' && kierunek != 'W')//Bład parametru kierunku
        return 5;
    if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->ile_razy_sie_ruszyles > 3)
        return 9;
    return 0;
}
void wyswietl_blad(int y, int x, char kierunek, int numerx, int klasa, char gracz, char* c, char* tab_domyslne, int kod)
{
    char komenda[10];
    int dlugosc_kom = 0;
    if (tab_domyslne[7] == 'S')
    {
        dlugosc_kom = 4;
        komenda[0] = 'S', komenda[1] = 'H', komenda[2] = 'I', komenda[3] = 'P';
    }
    else if (tab_domyslne[7] == 'P')
    {
        dlugosc_kom = 10;
        komenda[0] = 'P', komenda[1] = 'L', komenda[2] = 'A', komenda[3] = 'C', komenda[4] = 'E', komenda[5] = '_', komenda[6] = 'S', komenda[7] = 'H', komenda[8] = 'I', komenda[9] = 'P';
    }
    else if (tab_domyslne[7] == 'M')
    {
        dlugosc_kom = 4;
        komenda[0] = 'M', komenda[1] = 'O', komenda[2] = 'V', komenda[3] = 'E';
    }
    const char* nazwa_statku = zamien_spowrotem(klasa);

    // [DES]TROYER - 2 [CRU]ISER - 3 [BAT]TLESHIP - 4 [CAR]RIER - 5
    if (kod == 1 || kod == 4)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        cout << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;

    }
    else if (kod == 2)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        if (tab_domyslne[7] == 'M')
            cout << "\": SHIP WENT FROM BOARD" << endl;
        else
            cout << "\": NOT IN STARTING POSITION" << endl;


    }
    else if (kod == 3)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        cout << "\": SHIP ALREADY PRESENT" << endl;

    }
    else if (kod == 5)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        cout << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;

    }
    else if (kod == 6)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        cout << "\": WRONG COMMAND" << endl;

    }
    else if (kod == 7)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        cout << "\": PLACING SHIP ON REEF" << endl;

    }
    else if (kod == 8)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        cout << "\": INNIT POS" << endl;
    }
    else if (kod == 9)
    {
        cout << "INVALID OPERATION \"";
        for (int i = 0; i < dlugosc_kom; i++)
            cout << komenda[i];
        if (tab_domyslne[7] == 'M')
            cout << " " << numerx << " " << nazwa_statku << " " << tab_domyslne[8];
        else if (tab_domyslne[7] == 'P')
            cout << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku;
        if (tab_domyslne[7] == 'S')
        {
            cout << " " << gracz << " " << y << " " << x << " " << kierunek << " " << numerx << " " << nazwa_statku << " ";
            for (int o = 0; o < klasa; o++)
                cout << c[o];
        }
        cout << "\": SHIP MOVED ALREADY" << endl;
    }
    tab_domyslne[9] = '1';

}
bool rozmiesc_statek(int y, int x, char kierunek, int numerx, int klasa, Tablica& tab, Statek** tablicastatkow, char gracz, char* c, char* tab_domyslne)
{
    //cout << "rozmiesc_statek" << endl;
    int temp = 0;
    if (gracz == 'A')
        temp = 0;
    else if (gracz == 'B')
        temp = 4;
    int kod = 0;
    int f = kod_bledu(y, x, kierunek, numerx, klasa, tablicastatkow, gracz, tab_domyslne, tab);
    if (f != 0)
    {
        kod = f;
        wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
        return 1;
    }

    if (init_position(gracz, x, y, tab_domyslne) != 0)
    {
        kod = 8;
        wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
        return 1;

    }

    if (czy_nie_ma_innych_statków(y, x, kierunek, klasa, tab) != 0)
    {
        kod = 5;
        wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
        return 1;
    }


    tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->czy_postawiony = 1;
    for (int i = 0; i < klasa; i++)
    {
        tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->kierunek = kierunek;



        if (kierunek == 'N' && tab.mapa[y + i][x] != '+' && tab.mapa[y + i][x] != 'x')
        {
            if (tab.mapa[y + i][x] == '#')
            {
                kod = 7;
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }


            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->x[i] = x;
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->y[i] = (y + i);
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->z[i] = (int)(c[i]) - 48;
            if ((int)(c[i]) - 48 == 0)
                tab.mapa[y + i][x] = 'x';
            else if ((int)(c[i]) - 48 == 1)
                tab.mapa[y + i][x] = '+';
            else
            {
                kod = 6;//Bład parametru
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }


        }
        else if (kierunek == 'S')
        {
            if (tab.mapa[y - i][x] == '#')
            {
                kod = 7;
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }

            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->x[i] = x;
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->y[i] = (y - i);
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->z[i] = (int)(c[i]) - 48;
            if ((int)(c[i]) - 48 == 0)
                tab.mapa[y - i][x] = 'x';
            else if ((int)(c[i]) - 48 == 1)
                tab.mapa[y - i][x] = '+';
            else
            {
                kod = 6;//Bład parametru
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }

        }
        else if (kierunek == 'E')
        {
            if (tab.mapa[y][x - i] == '#')
            {
                kod = 7;
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }

            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->x[i] = (x - i);
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->y[i] = y;
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->z[i] = (int)(c[i]) - 48;
            if ((int)(c[i]) - 48 == 0)
                tab.mapa[y][x - i] = 'x';
            else if ((int)(c[i]) - 48 == 1)
                tab.mapa[y][x - i] = '+';
            else
            {
                kod = 6;//Bład parametru
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }

        }
        else if (kierunek == 'W')
        {
            if (tab.mapa[y][x + i] == '#')
            {
                kod = 7;
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->x[i] = (x + i);
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->y[i] = y;
            tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numerx]->z[i] = (int)(c[i]) - 48;
            if ((int)(c[i]) - 48 == 0)
                tab.mapa[y][x + i] = 'x';
            else if ((int)(c[i]) - 48 == 1)
                tab.mapa[y][x + i] = '+';
            else
            {
                kod = 6;//Bład parametru
                wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
                return 1;
            }
        }
        else
        {
            kod = 5;//Bład parametru
            wyswietl_blad(y, x, kierunek, numerx, klasa, gracz, c, tab_domyslne, kod);
            return 1;
        }
    }
    return 0;
}
int rozmiesc_statek_pomocnicza(int y, int x, char kierunek, int numerx, int klasa, Tablica& tab, Statek** tablicastatkow, char gracz, char* tab_domyslne)
{
    char jedynki_zera[MAKSYMALNY_ROZMIAR_STATKU];
    for (int i = 0; i < 5; i++)
        jedynki_zera[i] = '1';
    return rozmiesc_statek(y, x, kierunek, numerx, klasa, tab, tablicastatkow, gracz, jedynki_zera, tab_domyslne);
}
bool czy_zwyciestwo(Tablica& tab, char gracz)
{
    int liczbaA = 0;
    int liczbaB = 0;
    for (int i = 0; i < WYSOKOSC; i++)
    {
        for (int j = 0; j < SZEROKOSC; j++)
        {
            if (i < 10 && tab.mapa[i][j] == '+')
                liczbaA++;
            else if (i > (WYSOKOSC - 1) / 2 && i < WYSOKOSC && tab.mapa[i][j] == '+')
                liczbaB++;
        }
    }
    if (liczbaA == 0 || liczbaB == 0)
    {
        cout << gracz << " won" << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}
char SET_FLEET(char gracz, int a, int b, int c, int d, Statek** tablicastatkow)
{

    int temp = 1;
    if (gracz == 'A')
        temp = 0;
    else if (gracz == 'B')
        temp = LICZBA_KLAS;

    for (int j = 0; j < d; j++)//numer statku
        tablicastatkow[(temp + 0) * MAKSYMALNA_ILOSC_STATKOW + j] = new Statek(2, j, gracz);
    for (int j = 0; j < c; j++)//numer statku
        tablicastatkow[(temp + 1) * MAKSYMALNA_ILOSC_STATKOW + j] = new Statek(3, j, gracz);
    for (int j = 0; j < b; j++)//numer statku
        tablicastatkow[(temp + 2) * MAKSYMALNA_ILOSC_STATKOW + j] = new Statek(4, j, gracz);
    for (int j = 0; j < a; j++)//numer statku
        tablicastatkow[(temp + 3) * MAKSYMALNA_ILOSC_STATKOW + j] = new Statek(5, j, gracz);
    //cout << "Sukces "<<gracz<<endl;
//tablicastatkow[temp * h * MAKSYMALNA_ILOSC_STATKOW + j]->wielkosc = h;
//tablicastatkow[temp * h * MAKSYMALNA_ILOSC_STATKOW + j]->numer = j;
//tablicastatkow[temp * h * MAKSYMALNA_ILOSC_STATKOW + j]->gracz = gracz;
    if (gracz == 'A')
        return 'A';
    else if (gracz == 'B')
        return 'B';
    return 'Q';
}
void pisztablice(Tablica& tab, Statek** tablicastatkow, char* tab_domyslne)
{
    int liczbaA = 0;
    int liczbaA2 = 0;
    int liczbaB = 0;
    int liczbaB2 = 0;
    for (int i = 0; i < tab.wysokosc; i++)
    {

        for (int j = 0; j < tab.szerokosc; j++)
            cout << tab.mapa[i][j];
        cout << endl;

    }
    for (int g = 0; g < MAKSYMALNA_ILOSC_STATKOW * LICZBA_GRACZY * LICZBA_KLAS; g++)
    {

        if (tablicastatkow[g] != nullptr)
            for (int s = 0; s < tablicastatkow[g]->wielkosc; s++)
            {
                if (tablicastatkow[g]->z[s] == 1 && tablicastatkow[g]->czy_postawiony == 1)
                {
                    if (tablicastatkow[g]->gracz == 'A')
                        liczbaA2++;
                    else if (tablicastatkow[g]->gracz == 'B')
                        liczbaB2++;
                }
            }

    }
    if (tab_domyslne[20] == NULL)
    {
        cout << "PARTS REMAINING:: A : " << liczbaA2 << " B : " << liczbaB2 << endl;
    }

}
int shoot(int y, int x, Statek** tablicastatkow, Tablica& tab, char gracz)
{
    if (x > SZEROKOSC || y > WYSOKOSC || y < 0 || x < 0)
        return 1;
    if (x < SZEROKOSC && y < WYSOKOSC)
    {
        for (int i = 0; i < MAKSYMALNA_ILOSC_STATKOW * LICZBA_GRACZY * LICZBA_KLAS; i++)
        {
            if (tablicastatkow[i] != nullptr && tablicastatkow[i]->czy_postawiony == 0)
                return 2;
            for (int s = 0; s < LICZBA_KLAS + 1; s++)
            {
                if (tablicastatkow[i] != nullptr && tablicastatkow[i]->x[s] == x && tablicastatkow[i]->y[s] == y)
                {
                    if (tab.mapa[y][x] == '+')
                    {
                        tab.mapa[y][x] = 'x';
                        tablicastatkow[i]->z[s] = 0;
                    }

                    if (czy_zwyciestwo(tab, gracz) == 1)
                        return 10;
                }
            }
        }

        //else
        //    cout << "pudlo" << endl;
    }
    return 0;

}
int shoot_advanced(int numer, int klasa, int y, int x, Statek** tablicastatkow, Tablica& tab, char gracz) {
    int temp = 1;
    if (gracz == 'A')
        temp = 0;
    else if (gracz == 'B')
        temp = LICZBA_KLAS;
    char temtab[3] = { 'X','X','X' };
    if (klasa == 5)
        temtab[0] = 'C', temtab[1] = 'A', temtab[2] = 'R';
    else if (klasa == 4)
        temtab[0] = 'B', temtab[1] = 'A', temtab[2] = 'T';
    else if (klasa == 3)
        temtab[0] = 'C', temtab[1] = 'R', temtab[2] = 'U';
    else if (klasa == 2)
        temtab[0] = 'D', temtab[1] = 'E', temtab[2] = 'S';

    if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->ile_razy_sie_strzeliles <= 3)
    {
        if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->z[1] == 1)
        {
            int x1 = tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->x[1];
            int y1 = tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->y[1];
            float rowananie = sqrt(pow(abs(x - x1), 2) + pow(abs(y - y1), 2));
            if (rowananie <= klasa)
            { // float rowananie = sqrt(pow(abs(x-x1), 2) + pow(abs(y-y1), 2));
                //cout << "w zasiegu" << endl;
                tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->ile_razy_sie_strzeliles++;
                for (int i = 0; i < MAKSYMALNA_ILOSC_STATKOW * LICZBA_GRACZY * LICZBA_KLAS; i++)
                {
                    if (tablicastatkow[i] != nullptr && tablicastatkow[i]->czy_postawiony == 0)
                        return 2;
                    for (int s = 0; s < LICZBA_KLAS + 1; s++)
                    {
                        if (tablicastatkow[i] != nullptr && tablicastatkow[i]->x[s] == x && tablicastatkow[i]->y[s] == y)
                        {
                            if (tab.mapa[y][x] == '+')
                            {
                                //cout << "trafione!" << endl;
                                tab.mapa[y][x] = 'x';
                                tablicastatkow[i]->z[s] = 0;//Trafione!
                            }

                            if (czy_zwyciestwo(tab, gracz) == 1)
                                return 10;
                        }
                        //else
                            //cout << "pudlo" << endl;
                    }
                }

            }
            else
            {

                cout << "INVALID OPERATION \"SHOOT " << numer << " " << temtab[0] << temtab[1] << temtab[2] << " " << y << " " << x;
                cout << "\": SHOOTING TOO FAR" << endl;
                return 4;
            }
        }
        else
        {
            cout << "INVALID OPERATION \"SHOOT " << numer << " " << temtab[0] << temtab[1] << temtab[2] << " " << y << " " << x;
            cout << "\": SHIP CANNOT SHOOT" << endl;
            return 4;
        }
    }
    else
    {
        cout << "INVALID OPERATION \"SHOOT " << numer << " " << temtab[0] << temtab[1] << temtab[2] << " " << y << " " << x;
        cout << "\": TOO MANY SHOOTS" << endl;
        return 4;
    }
    return 0;
}
void state(Statek** tablicastatkow, char* tab_domyslne, Tablica& tab)
{
    // Pierwsze dwa na wartosci domyslne 9-na przypadek \0
    tab_domyslne[20] = NULL;
    bool state = 0;
    int ilosc_SET = 0;
    //int jakiego_gracza_set[2];
    int dlugosc_komendy = 20;
    int typy[4] = { 2,3,4,5 }; // [DES]TROYER - 2 [CRU]ISER - 3 [BAT]TLESHIP - 4 [CAR]RIER - 5
    while (state == 0)
    {
        char komenda[20] = { 0 };

        for (int i = 0; i < dlugosc_komendy; i++)
        {
            dlugosc_komendy = 20;
            cin >> komenda[i];
            if (komenda[0] == '\0')
            {
                state = 1;
                tab_domyslne[9] = '1';
            }
            //state
            if (komenda[0] == '[' && komenda[1] == 's' && komenda[2] == 't' && komenda[3] == 'a' && komenda[4] == 't' && komenda[5] == 'e' && komenda[6] == ']')
            {
                dlugosc_komendy = 7;
                state = 1;
                //cout << "koniec state" << endl;
            }
            // NEXT_PLAYER P
            else if (komenda[0] == 'N' && komenda[1] == 'E' && komenda[2] == 'X' && komenda[3] == 'T' && komenda[4] == '_' && komenda[5] == 'P' && komenda[6] == 'L' && komenda[7] == 'A' && komenda[8] == 'Y' && komenda[9] == 'E' && komenda[10] == 'R')
            {
                dlugosc_komendy = 11;
                //cout << "NEXT_PLAYER" << endl;
                cin >> tab_domyslne[4];

            }
            else if (komenda[0] == 'P' && komenda[1] == 'R' && komenda[2] == 'I' && komenda[3] == 'N' && komenda[4] == 'T' && komenda[5] == '1')
            {
                dlugosc_komendy = 6;
                pisztablice_1(tab, tablicastatkow, tab_domyslne);
            }
            //INIT_POSITION 
            else if (komenda[0] == 'I' && komenda[1] == 'N' && komenda[2] == 'I' && komenda[3] == 'T' && komenda[4] == '_' && komenda[5] == 'P' && komenda[6] == 'O' && komenda[7] == 'S' && komenda[8] == 'I' && komenda[9] == 'T' && komenda[10] == 'I' && komenda[11] == 'O' && komenda[12] == 'N')
            {
                dlugosc_komendy = 13;
                char gra;
                int y1, x1, y2, x2;
                cin >> gra >> y1 >> x1 >> y2 >> x2;
                if (gra == 'A')
                {
                    tab_domyslne[10] = 'A';
                    tab_domyslne[11] = y1;
                    tab_domyslne[12] = x1;
                    tab_domyslne[13] = y2;
                    tab_domyslne[14] = x2;

                }
                else if (gra == 'B')
                {
                    tab_domyslne[15] = 'B';
                    tab_domyslne[16] = y1;
                    tab_domyslne[17] = x1;
                    tab_domyslne[18] = y2;
                    tab_domyslne[19] = x2;
                }
                else
                    cout << "ERROR INIT_POSI" << endl;

            }
            //PRINT x
            else if (komenda[0] == 'P' && komenda[1] == 'R' && komenda[2] == 'I' && komenda[3] == 'N' && komenda[4] == 'T' && komenda[5] == '0')
            {
                dlugosc_komendy = 6;
                //cout << "PRINT" << endl;
                pisztablice(tab, tablicastatkow, tab_domyslne);
                break;
            }
            //SET_FLEET A 1 1 2 1
            else if (komenda[0] == 'S' && komenda[1] == 'E' && komenda[2] == 'T' && komenda[3] == '_' && komenda[4] == 'F' && komenda[5] == 'L' && komenda[6] == 'E' && komenda[7] == 'E' && komenda[8] == 'T')
            {
                dlugosc_komendy = 9;
                if (i == dlugosc_komendy - 1)
                {

                    char gracz;
                    int a, b, c, d;
                    char e;
                    cin >> gracz;
                    cin >> a;
                    cin >> b;
                    cin >> c;
                    cin >> d;
                    e = SET_FLEET(gracz, a, b, c, d, tablicastatkow);
                    if (e == 'A')
                        tab_domyslne[0] = 'A';
                    else if (e == 'B')
                        tab_domyslne[1] = 'B';
                    ilosc_SET++;
                    //0-39 Gracz A 40-79 Gracz B
                }
            }
            //BOARD_SIZE
            else if (komenda[0] == 'B' && komenda[1] == 'O' && komenda[2] == 'A' && komenda[3] == 'R' && komenda[4] == 'D' && komenda[5] == '_' && komenda[6] == 'S' && komenda[7] == 'I' && komenda[8] == 'Z' && komenda[9] == 'E')
            {
                dlugosc_komendy = 10;
                //cout << "BOARD_SIZE" << endl;
                int y, x;//nowe wymiary
                cin >> y;
                cin >> x;

                //delete &tab;
                tab = *(new Tablica(y, x)); // tworze nowy obiekt 

                for (int c = 0; c < tab.wysokosc; c++)
                {
                    for (int d = 0; d < tab.szerokosc; d++)
                    {
                        tab.mapa[c][d] = ' ';
                    }

                }
                //cout << "Ustawione" << endl;
            }
            //SAVE
            else if (komenda[0] == 'S' && komenda[1] == 'A' && komenda[2] == 'V' && komenda[3] == 'E')
            {
                dlugosc_komendy = 4;
                zapis(tab, tablicastatkow, tab_domyslne);
            }
            //SHIP A 0 0 N 0 CAR 11101
            else if (komenda[0] == 'S' && komenda[1] == 'H' && komenda[2] == 'I' && komenda[3] == 'P')
            {
                //musze zsetowac

                dlugosc_komendy = 4;

                char gracz, kierunek;
                int y, x, c;
                char temtab[3];
                char temtab2[6];//11111

                cin >> gracz >> y >> x >> kierunek >> c;
                for (int u = 0; u < 3; u++)
                    cin >> temtab[u];
                cin >> temtab2;
                //cout << "SHIP" << endl;

                if (tab_domyslne[0] != gracz && tab_domyslne[1] != gracz)
                {
                    char e = SET_FLEET(gracz, 1, 2, 3, 4, tablicastatkow);
                    if (e == 'A')
                        tab_domyslne[0] = 'A';
                    else if (e == 'B')
                        tab_domyslne[1] = 'B';
                }

                tab_domyslne[7] = 'S';
                int klasa = klasa_statku(temtab[0], temtab[1], temtab[2]);
                int zmienna = rozmiesc_statek(y, x, kierunek, c, klasa, tab, tablicastatkow, gracz, temtab2, tab_domyslne);
                if (zmienna != 0)
                {
                    state = 1;
                    break;
                }
            }
            //ROZSZERZONA
            //EXTENDED_SHIPS
            else if (komenda[0] == 'E' && komenda[1] == 'X' && komenda[2] == 'T' && komenda[3] == 'E' && komenda[4] == 'N' && komenda[5] == 'D' && komenda[6] == 'E' && komenda[7] == 'D' && komenda[8] == '_' && komenda[9] == 'S' && komenda[10] == 'H' && komenda[11] == 'I' && komenda[12] == 'P' && komenda[13] == 'S')
            {
                dlugosc_komendy = 14;
                tab_domyslne[3] = 'R';
            }
            //REEF 0 1
            else if (komenda[0] == 'R' && komenda[1] == 'E' && komenda[2] == 'E' && komenda[3] == 'F')
            {
                dlugosc_komendy = 4;
                int x, y;
                cin >> y >> x;
                tab.mapa[y][x] = '#';
            }
        }
    }
}
void usun_plus(int y, int x, char kierunek, Tablica& tab, int wielkosc)
{
    for (int i = 0; i < wielkosc; i++)
    {
        if (kierunek == 'N')
            tab.mapa[y + i][x] = ' ';
        else if (kierunek == 'S')
            tab.mapa[y - i][x] = ' ';
        else if (kierunek == 'E')
            tab.mapa[y][x - i] = ' ';
        else if (kierunek == 'W')
            tab.mapa[y][x + i] = ' ';
    }
}
void zeruj_ruchy(Statek** tablicastatkow, char gracz)
{
    int temp = 1;
    if (gracz == 'A')
        temp = 0;
    else if (gracz == 'B')
        temp = 4;
    for (int i = temp; i < (MAKSYMALNA_ILOSC_STATKOW * LICZBA_GRACZY * LICZBA_KLAS) - temp; i++)
    {
        if (tablicastatkow[i] != nullptr)
        {
            tablicastatkow[i]->ile_razy_sie_ruszyles = 0;
        }

    }

}
int rusz_sie(int numer, int klasa, int gdzie, char gracz, Tablica& tab, Statek** tablicastatkow, char* tab_domyslne)
{
    int temp;
    if (gracz == 'A')
        temp = 0;
    else if (gracz == 'B')
        temp = 4;
    else
        return 1;
    char kierunek = tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->kierunek;
    int wielkosc = tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->wielkosc;
    int x_poczatek = 0;
    int y_poczatek = 0;
    char nowy_kierunek = ' ';
    int yy = tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->y[0];
    int xx = tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->x[0];
    if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->czy_postawiony == 1 && tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->z[wielkosc - 1])
    {
        if (gdzie == 'F')
        {
            if (kierunek == 'N')
            {
                y_poczatek = yy - 1;
                x_poczatek = xx;
                usun_plus(yy, xx, kierunek, tab, wielkosc);
                nowy_kierunek = 'N';
            }

            else if (kierunek == 'S')
            {
                y_poczatek = yy + 1;
                x_poczatek = xx;
                usun_plus(yy, xx, kierunek, tab, wielkosc);
                nowy_kierunek = 'S';
            }
            else if (kierunek == 'E')
            {
                y_poczatek = yy;
                x_poczatek = xx + 1;
                usun_plus(yy, xx, kierunek, tab, wielkosc);
                nowy_kierunek = 'E';
            }
            else if (kierunek == 'W')
            {
                y_poczatek = yy;
                x_poczatek = xx - 1;
                usun_plus(yy, xx, kierunek, tab, wielkosc);
                nowy_kierunek = 'W';
            }
        }
        else if (gdzie == 'R')
        {
            if (kierunek == 'S')
            {
                y_poczatek = yy + 1;
                x_poczatek = xx - wielkosc + 1;
                nowy_kierunek = 'W';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else if (kierunek == 'N')
            {
                y_poczatek = yy - 1;
                x_poczatek = xx + wielkosc - 1;
                nowy_kierunek = 'E';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else if (kierunek == 'W')
            {
                y_poczatek = yy - wielkosc + 1;
                x_poczatek = xx - 1;
                nowy_kierunek = 'N';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else if (kierunek == 'E')
            {
                y_poczatek = yy + wielkosc - 1;
                x_poczatek = xx + 1;
                nowy_kierunek = 'S';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else
                return 1;
        }
        else if (gdzie == 'L')
        {
            if (kierunek == 'S')
            {
                y_poczatek = yy + 1;
                x_poczatek = xx + wielkosc - 1;
                nowy_kierunek = 'E';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else if (kierunek == 'N')
            {
                y_poczatek = yy - 1;
                x_poczatek = xx - wielkosc + 1;
                nowy_kierunek = 'W';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else if (kierunek == 'W')
            {
                y_poczatek = yy + wielkosc - 1;
                x_poczatek = xx - 1;
                nowy_kierunek = 'S';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else if (kierunek == 'E')
            {
                y_poczatek = yy - wielkosc + 1;
                x_poczatek = xx + 1;
                nowy_kierunek = 'N';
                usun_plus(yy, xx, kierunek, tab, wielkosc);
            }
            else
                return 1;
        }
        else
            return 1;
        tab_domyslne[7] = 'M';
        tab_domyslne[8] = gdzie;
        tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->ile_razy_sie_ruszyles = tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->ile_razy_sie_ruszyles + 1;
        tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->czy_postawiony = 0;
        char c[5];
        for (int i = 0; i < klasa; i++)
        {
            if (tablicastatkow[(temp + (klasa - 2)) * MAKSYMALNA_ILOSC_STATKOW + numer]->z[i] == 1)
                c[i] = '1';
            else
                c[i] = '0';
        }
        if (rozmiesc_statek(y_poczatek, x_poczatek, nowy_kierunek, numer, klasa, tab, tablicastatkow, gracz, c, tab_domyslne) != 0)
            return 1;
        else
            return 0;
    }
    else
        return 1;

    return 0;

}
void radar(Tablica& tab, Statek** tablicastatkow, char gracz, char* tab_domyslne)
{
    tab_domyslne[20] = ' ';
    int temp = 0;
    int temp2 = 0;
    if (gracz == 'A')
    {
        temp = 0;
        temp2 = 1;
    }
    else if (gracz == 'B')
    {
        temp2 = 0;
        temp = 1;
    }

    for (int i = 0; i < tab.wysokosc; i++)//w tym miejscu zamieniam wszystkie ' ' na ?
    {
        for (int j = 0; j < tab.szerokosc; j++)
        {
            if (tab.mapa[i][j] == ' ')
                tab.mapa[i][j] = '?';


        }
    }
    for (int h = temp2 * WIELKOSC_STRUKTURY_STATEK_DLA_JEDNEGO_GRACZA; h < (temp2 + 1) * WIELKOSC_STRUKTURY_STATEK_DLA_JEDNEGO_GRACZA; h++)//tutaj konwertuje wszystkie + gracza przeciwnego na ?
    {
        if (tablicastatkow[h] != nullptr && tablicastatkow[h]->gracz != gracz)
        {
            int wielkosc = tablicastatkow[h]->wielkosc;
            for (int i = 0; i < wielkosc; i++)
            {
                int x = tablicastatkow[h]->x[i];
                int y = tablicastatkow[h]->y[i];
                tab.mapa[y][x] = '?';
            }
        }
    }
    for (int i = temp * WIELKOSC_STRUKTURY_STATEK_DLA_JEDNEGO_GRACZA; i < (temp + 1) * LICZBA_KLAS * MAKSYMALNA_ILOSC_STATKOW; i++)// tutaj zamienia ? na " "
    {
        if (tablicastatkow[i] != nullptr && tablicastatkow[i]->czy_postawiony == 0)
        {
            cout << "NOT ALL SHIPS PLACED";
            break;
        }

        if (tablicastatkow[i] != nullptr)
        {
            int x = tablicastatkow[i]->x[0];
            int y = tablicastatkow[i]->y[0];
            int wielkosc = tablicastatkow[i]->wielkosc;

            for (int a = 0; a < wielkosc + 1; a++)
            {
                for (int b = 0; b < wielkosc + 1; b++)
                {
                    int aa = a;
                    int bb = b;
                    float rowananie = sqrt(pow(abs(b), 2) + pow(abs(a), 2));
                    if (rowananie <= wielkosc)
                    {
                        if (x - a < 0)
                            aa = 0;
                        if (y - b < 0)
                            bb = 0;
                        if (tab.mapa[y - bb][x - aa] == '?')
                            tab.mapa[y - bb][x - aa] = ' ';
                        bb = b;
                        if (y + b > tab.wysokosc)
                            bb = 0;
                        if (tab.mapa[y + bb][x - aa] == '?')
                            tab.mapa[y + bb][x - aa] = ' ';
                        aa = a;
                        if (x + a > tab.szerokosc)
                            aa = 0;
                        if (tab.mapa[y - bb][x + aa] == '?')
                            tab.mapa[y - bb][x + aa] = ' ';
                        bb = b;
                        if (y + b > tab.wysokosc)
                            bb = 0;
                        if (tab.mapa[y + bb][x + aa] == '?')
                            tab.mapa[y + bb][x + aa] = ' ';

                    }
                }
            }
        }
    }
    for (int p = 0; p < WIELKOSC_STRUKTURY_STATEK; p++)//tutaj zamieniam " " na + gdy jest widoczny
    {
        if (tablicastatkow[p] != nullptr)
        {
            for (int piac = 0; piac < tablicastatkow[p]->wielkosc; piac++)
            {

                int xx = tablicastatkow[p]->x[piac];
                int yy = tablicastatkow[p]->y[piac];
                if (tablicastatkow[p]->z[piac] == true && tab.mapa[yy][xx] == ' ')
                {
                    tab.mapa[yy][xx] = '+';

                }
                else if (tablicastatkow[p]->z[piac] == false && tab.mapa[yy][xx] == ' ')
                    tab.mapa[yy][xx] = 'x';

            }
        }
    }
    pisztablice(tab, tablicastatkow, tab_domyslne);
    for (int i = 0; i < tab.wysokosc; i++)//TUTAJ ZMIENIAM WSZYSTKIE ? Z POWROTEM NA ' '
    {
        for (int j = 0; j < tab.szerokosc; j++)
        {
            if (tab.mapa[i][j] == '?')
                tab.mapa[i][j] = ' ';


        }
    }

    for (int h = 0; h < WIELKOSC_STRUKTURY_STATEK; h++)//TUTAJ DODAJE WSZYSTKIE STATKI NA PLANSZE
    {
        if (tablicastatkow[h] != nullptr)
        {
            int wielkosc = tablicastatkow[h]->wielkosc;
            for (int i = 0; i < wielkosc; i++)
            {
                int x = tablicastatkow[h]->x[i];
                int y = tablicastatkow[h]->y[i];
                int z = tablicastatkow[h]->z[i];
                if (z == 1)
                    tab.mapa[y][x] = '+';
                else if (z == 0)
                {
                    tab.mapa[y][x] = 'x';
                }
            }


        }
    }
}
bool player(char Gracz, Tablica& tab, Statek** tablicastatkow, char* tab_domyslne)
{

    bool state = 0;
    int dlugosc_komendy = 20;
    int typy[4] = { 2,3,4,5 }; // [DES]TROYER - 2 [CRU]ISER - 3 [BAT]TLESHIP - 4 [CAR]RIER - 5
    //cout << "in"<<endl;

    while (state == 0)
    {
        char komenda[20] = { 0 };
        dlugosc_komendy = 20;
        for (int i = 0; i < dlugosc_komendy; i++)
        {
            cin >> komenda[i];
            if (komenda[0] == '\0')
            {
                state = 1;
                break;
            }
            //MOVE i C x
            else if (komenda[0] == 'M' && komenda[1] == 'O' && komenda[2] == 'V' && komenda[3] == 'E')
            {
                dlugosc_komendy = 4;
                int numer;
                char klasa[3];
                char gdzie;
                cin >> numer >> klasa[0] >> klasa[1] >> klasa[2] >> gdzie;
                if (rusz_sie(numer, klasa_statku(klasa[0], klasa[1], klasa[2]), gdzie, Gracz, tab, tablicastatkow, tab_domyslne) != 0)
                    return 1;
            }
            //PLACE _SHIP y x D i C
            else if (komenda[0] == 'P' && komenda[1] == 'L' && komenda[2] == 'A' && komenda[3] == 'C' && komenda[4] == 'E' && komenda[5] == '_' && komenda[6] == 'S' && komenda[7] == 'H' && komenda[8] == 'I' && komenda[9] == 'P')
            {
                //cout << "PLACE _SHIP" << endl;
                dlugosc_komendy = 10;
                if (i == dlugosc_komendy - 1)
                {
                    int x, y, h;
                    char d;
                    char temtab[3];
                    cin >> y;
                    cin >> x;
                    cin >> d;
                    cin >> h;
                    for (int u = 0; u < 3; u++)
                        cin >> temtab[u];
                    tab_domyslne[7] = 'P';
                    //cout << klasa_statku(komenda[14], komenda[15], komenda[16]) << endl;
                    //int y, int x, char kierunek, int numerx, int klasa, char tymtab[WYSOKOSC][SZEROKOSC], Statek* tablicastatkow[MAKSYMALNA_ILOSC_STATKOW*LICZBA_GRACZY*LICZBA_KLAS], char gracz
                    int zmienna = rozmiesc_statek_pomocnicza(y, x, d, h, klasa_statku(temtab[0], temtab[1], temtab[2]), tab, tablicastatkow, Gracz, tab_domyslne);
                    if (zmienna != 0)
                        return 1;
                }

            }

            //SHOOT y x
            else if (komenda[0] == 'S' && komenda[1] == 'H' && komenda[2] == 'O' && komenda[3] == 'O' && komenda[4] == 'T')
            {
                dlugosc_komendy = 5;
                if (tab_domyslne[3] != 'R')
                {
                    int y, x;
                    cin >> y;
                    cin >> x;
                    int test_strzelanie = shoot(y, x, tablicastatkow, tab, Gracz);
                    if (test_strzelanie == 1)
                    {
                        cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": FIELD DOES NOT EXIST" << endl;
                        return 1;
                    }
                    else if (test_strzelanie == 2)
                    {
                        cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": NOT ALL SHIPS PLACED" << endl;
                        return 1;
                    }
                    else if (test_strzelanie == 10)
                        return 1;
                }
                else
                {
                    int numer, y, x;
                    char temtab[3];
                    cin >> numer;
                    for (int u = 0; u < 3; u++)
                        cin >> temtab[u];
                    cin >> y;
                    cin >> x;
                    int test_strzelanie = shoot_advanced(numer, klasa_statku(temtab[0], temtab[1], temtab[2]), y, x, tablicastatkow, tab, Gracz);
                    if (test_strzelanie == 1)
                    {
                        cout << "//INVALID OPERATION \"SHOOT" << numer << temtab[0] << temtab[1] << temtab[2] << " " << y << " " << x << " ";
                        cout << "\"FIELD DOES NOT EXIST" << endl;
                        return 1;
                    }
                    else if (test_strzelanie == 2)
                    {
                        cout << "//INVALID OPERATION \"SHOOT" << numer << temtab[0] << temtab[1] << temtab[2] << " " << y << " " << x << " ";
                        cout << "\"NOT ALL SHIPS PLACED" << endl;
                        return 1;
                    }
                    else if (test_strzelanie == 10)
                        return 1;
                    else if (test_strzelanie == 4)
                    {
                        return 1;
                    }
                }


            }
            //[playerA}
            else if (komenda[0] == '[' && komenda[1] == 'p' && komenda[2] == 'l' && komenda[3] == 'a' && komenda[4] == 'y' && komenda[5] == 'e' && komenda[6] == 'r' && komenda[7] == Gracz && komenda[8] == ']')
            {
                dlugosc_komendy = 9;
                state = 1;
                if (Gracz == 'A')
                    tab_domyslne[4] = 'B';
                else if (Gracz == 'B')
                    tab_domyslne[4] = 'A';
                zeruj_ruchy(tablicastatkow, Gracz);
                //cout << "koniec Player" << endl;
                return 0;
            }
            //PRINT 0
            else if (komenda[0] == 'P' && komenda[1] == 'R' && komenda[2] == 'I' && komenda[3] == 'N' && komenda[4] == 'T' && komenda[5] == '0')
            {
                dlugosc_komendy = 6;
                radar(tab, tablicastatkow, Gracz, tab_domyslne);
            }
        }
    }
    return 0;
}

void zapis(Tablica& tab, Statek** tablicastatkow, char* tab_domyslne)
{
    int par[8] = { 0,0,0,0,0,0,0,0 };
    cout << "[state]" << endl;
    cout << "BOARD_SIZE" << ' ' << tab.wysokosc << ' ' << tab.szerokosc << endl;
    cout << "NEXT_PLAYER" << ' ' << tab_domyslne[4] << endl;
    if (tab_domyslne[10] != NULL)
        cout << "INIT_POSITION A" << tab_domyslne[10] << " " << tab_domyslne[11] << " " << tab_domyslne[12] << " " << tab_domyslne[13] << " " << tab_domyslne[14] << endl;
    else
        cout << "INIT_POSITION A " << '0' << " " << '0' << " " << tab.wysokosc / 2 - 1 << " " << tab.szerokosc - 1 << endl;
    for (int i = 0; i < WIELKOSC_STRUKTURY_STATEK; i++)
    {
        if (tablicastatkow[i] != NULL)
        {
            if (i / 10 == 0)
                par[0]++;
            else if (i / 10 == 1)
                par[1]++;
            else if (i / 10 == 2)
                par[2]++;
            else if (i / 10 == 3)
                par[3]++;
            else if (i / 10 == 4)
                par[4]++;
            else if (i / 10 == 5)
                par[5]++;
            else if (i / 10 == 6)
                par[6]++;
            else if (i / 10 == 7)
                par[7]++;
        }
    }
    cout << "SET_FLEET A" << " " << par[3] << " " << par[2] << " " << par[1] << " " << par[0] << endl;
    for (int j = WIELKOSC_STRUKTURY_STATEK_DLA_JEDNEGO_GRACZA; j >= 0; j--)
    {
        if (tablicastatkow[j] != 0)
        {

            const char* nazwa_statku = zamien_spowrotem(tablicastatkow[j]->wielkosc);
            cout << "SHIP A " << tablicastatkow[j]->y[0] << " " << tablicastatkow[j]->x[0] << " " << tablicastatkow[j]->kierunek << " " << tablicastatkow[j]->numer << " " << nazwa_statku << " ";
            for (int klasy = 0; klasy < tablicastatkow[j]->wielkosc; klasy++)
                cout << tablicastatkow[j]->z[klasy];
            cout << endl;
        }
    }
    if (tab_domyslne[15] != NULL)
        cout << "INIT_POSITION" << tab_domyslne[15] << " " << tab_domyslne[16] << " " << tab_domyslne[17] << " " << tab_domyslne[18] << " " << tab_domyslne[19] << endl;
    else
        cout << "INIT_POSITION B " << (tab.wysokosc + 2) / 2 << " " << '0' << " " << tab.wysokosc - 1 << " " << tab.szerokosc - 1 << endl;
    cout << "SET_FLEET B" << " " << par[7] << " " << par[6] << " " << par[5] << " " << par[4] << endl;

    for (int j = 79; j >= WIELKOSC_STRUKTURY_STATEK_DLA_JEDNEGO_GRACZA; j--)
    {
        if (tablicastatkow[j] != 0)
        {
            const char* nazwa_statku = zamien_spowrotem(tablicastatkow[j]->wielkosc);
            cout << "SHIP B " << tablicastatkow[j]->y[0] << " " << tablicastatkow[j]->x[0] << " " << tablicastatkow[j]->kierunek << " " << tablicastatkow[j]->numer << " " << nazwa_statku << " ";
            for (int klasy = 0; klasy < tablicastatkow[j]->wielkosc; klasy++)
                cout << tablicastatkow[j]->z[klasy];
            cout << endl;
        }
    }
    cout << "[state]" << endl;
}
void pisztablice_1(Tablica& tab, Statek** tablicastatkow, char* tab_domyslne)
{
    int ile_zer_y = floor(log10(abs(tab.wysokosc)));
    int ile_zer_x = floor(log10(abs(tab.szerokosc)));
    int licznik = 0;
    for (int i = 0; i < ile_zer_y + 1; i++)
    {
        cout << " ";
    }
    for (int iter = 0; iter < tab.szerokosc; iter++)
    {
        cout << iter % 10;
    }
    cout << endl;
    for (int i = 0; i < tab.wysokosc; i++)
    {
        for (int j = 0; j < tab.szerokosc; j++)
        {
            if (j == 0)
            {
                for (int k = 0; k < ile_zer_y; k++)
                {
                    cout << 0;
                }
                cout << licznik;
                licznik++;
                if (licznik % 10 == 0)
                {
                    ile_zer_y--;
                }
            }
            if (tab.mapa[i][j] == ' ')
                cout << " ";
            else if (tab.mapa[i][j] == 'x')
                cout << 'x';
            else if (tab.mapa[i][j] == '#')
                cout << '#';
            else if (tab.mapa[i][j] == '+')
            {
                for (int g = 0; g < WIELKOSC_STRUKTURY_STATEK; g++)
                {
                    if (tablicastatkow[g] != nullptr)
                    {

                        int wielkosc = tablicastatkow[g]->wielkosc;
                        for (int index = 0; index < wielkosc; index++)
                        {
                            int x = tablicastatkow[g]->x[index];
                            int y = tablicastatkow[g]->y[index];
                            if (x == j && i == y)
                            {
                                if (index == 0)
                                {
                                    cout << '@';
                                }
                                else if (wielkosc > 2 && index == 1)
                                {
                                    cout << '!';
                                }
                                else if (index + 1 == wielkosc)
                                    cout << '%';
                                else
                                    cout << "+";
                            }

                        }
                    }
                }

            }

        }
        cout << endl;
    }
    int liczbaA2 = 0;
    int liczbaB2 = 0;
    for (int g = 0; g < MAKSYMALNA_ILOSC_STATKOW * LICZBA_GRACZY * LICZBA_KLAS; g++)
    {

        if (tablicastatkow[g] != nullptr)
            for (int s = 0; s < tablicastatkow[g]->wielkosc; s++)
            {
                if (tablicastatkow[g]->z[s] == 1 && tablicastatkow[g]->czy_postawiony == 1)
                {
                    if (tablicastatkow[g]->gracz == 'A')
                        liczbaA2++;
                    else if (tablicastatkow[g]->gracz == 'B')
                        liczbaB2++;
                }
            }

    }
    if (tab_domyslne[20] == NULL)
    {
        cout << "PARTS REMAINING:: A : " << liczbaA2 << " B : " << liczbaB2 << endl;
    }
}