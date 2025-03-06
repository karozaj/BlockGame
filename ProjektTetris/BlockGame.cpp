#include <iostream>
#include <conio.h>
#include "Funkcje.h"

using namespace std;

int main()
{
    wyniki tabelaWynikow[10];//Tablica, gdzie przechowywane jest 10 najlepszych wynikow
    wyniki obecnyWynik;//Wynik obecnej rozgrywki
    odczytaniePliku(tabelaWynikow);//Wpisanie wynikow zapisanych w pliku do tabeli, jesli taki plik juz istnieje

    char wybor = 0;//Wybor opcji z menu
    int wynik = 0;//Wynik z gry
    //glowne menu gry
    do
    {
        system("cls");
        std::cout << "BLOCK GAME" << std::endl;
        std::cout << std::endl;
        std::cout << "1. Graj" << std::endl;
        std::cout << "2. Pomoc" << std::endl;
        std::cout << "3. Tabela wynikow" << std::endl;
        std::cout << "4. Zakoncz" << std::endl;

        wybor = _getch();

        system("cls");

        switch (wybor)
        {
        case '1':
            obecnyWynik.wynik=graj();


            if (obecnyWynik.wynik > tabelaWynikow[9].wynik) //Jesli wynik jest wystarczajaco wysoki, zostanie zapisany w tabeli i nadpisze obecny najnizszy wynik
            {
                cout << "Podaj swoje imie:" << endl;
                getline(cin, obecnyWynik.imie);
                tabelaWynikow[9] = obecnyWynik;
            }

            sortowanieWynikow(tabelaWynikow);

            break;
        case '2':
            pomocUzytkownika();
            break;
        case '3':
            wyswietlTabelaWynikow(tabelaWynikow);
            break;
        default:
            break;
        }


    } while (wybor != '4');
    zapisanieWynikow(tabelaWynikow);//Zapisanie wynikow w pliku
}
