#pragma once
#include<string>

const int yPlansza = 21;	//wysokosc planszy - 20+1 na dolna krawedz
const int xPlansza = 12;	//szerokosc planszy - szerokosc pola gry to 10+2 na krawedzie planszy

const int yScreen = 30;		//wysoksoc tabeli
const int xScreen = 120;		//szerokosc tabeli, ktora bedzie wyswietlana w konsoli

struct wyniki   //struktura pozwalajaca zapisywac wynik oraz imie gracza
{
    int wynik{};
    std::string imie = "BRAK";
};


void losuj(int bag[7]);
int obrot(int y, int x, int nrObrotu);
bool kolizja(int y, int x, int nrKlocka, int nrObrotu, std::string klocki[7], char plansza[yPlansza * xPlansza]);
int sprawdzLinie(char plansza[xPlansza * yPlansza]);
int graj();

void pomocUzytkownika();

void sortowanieWynikow(wyniki tabelaWynikow[10]);
void zapisanieWynikow(wyniki tabelaWynikow[10]);
int odczytaniePliku(wyniki tabelaWynikow[10]);
void wyswietlTabelaWynikow(wyniki tabelaWynikow[10]);