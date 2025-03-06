#include <iostream>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include <string>
#include <fstream>
#include "Funkcje.h"

using namespace std;


void losuj(int bag[7])	//Funkcja, ktora losuje 7 roznych liczb (w celu ustalenia 7 kolejnych klockow w grze
{
	for (int i = 0; i < 7; i++)
	{
		bag[i] = 7;
	}

	int wylosowaneLiczby[7] = { 7,7,7,7,7,7,7 };	//Tablica przechowujaca informacje o tym, jakie liczby zostaly juz wylosowane. Reszta miejsc jest zapelniona
													//7, bo ta liczba nigdy nie bedzie wylosowana i stanowi informacje o tym, ktory element tablicy bag jeszcze nie byl losowany
	for (int i = 0; i < 7; i++)
	{
		do
		{
			bag[i] = rand() % 7;
		} while (bag[i] == wylosowaneLiczby[0] || bag[i] == wylosowaneLiczby[1] || bag[i] == wylosowaneLiczby[2]
			|| bag[i] == wylosowaneLiczby[3] || bag[i] == wylosowaneLiczby[4] || bag[i] == wylosowaneLiczby[5]
			|| bag[i] == wylosowaneLiczby[6]);

		wylosowaneLiczby[i] = bag[i];
	}
}

int obrot(int y, int x, int nrObrotu)	//Funkcja obracajaca klocek
{
	switch (nrObrotu % 4)
	{
	case 0:
		return (y * 4 + x);//Obrot o 0 stopni
	case 1:
		return(12 + y - (x * 4)); //Obrot o 90 stopni
	case 2:
		return(15 + y * (-4) + x * (-1)); //Obrot o 180 stopni
	case 3:
		return(3 - y + (4 * x));//Obrot o 270 stopni
	}
	return 0;
}

bool kolizja(int y, int x, int nrKlocka, int nrObrotu, string klocki[7], char plansza[yPlansza * xPlansza])
//Funkcja wykrywajaca kolizje klocka z plansza w celu sprawdzenia, czy dozwolony jest dany ruch
{
	int iKlocek = 0;	//Indeks w tabeli z klockiem
	int	iPlansza = 0;	//Indeks w planszy

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			iKlocek = obrot(i, j, nrObrotu);
			iPlansza = (y + i) * xPlansza + x + j;

			if (klocki[nrKlocka][iKlocek] != '0' && plansza[iPlansza] != '0')
			{
				return true;	//Jesli wykryto kolizje, funkcja zwraca true
			}
		}
	}

	return false; //Jesli nie jest wykryta kolizja, zwraca false
}

int sprawdzLinie(char plansza[xPlansza * yPlansza])	//Funkcja ma za zadanie sprawdzic, czy na plansze zostaly ulozone linie, jesli tak,													//
{													//to usunac je, przesunac reszte planszy w dol i zwrocic ilosc usunietych lin
	int wynik = 0;

	for (int i = yPlansza - 2; i >= 0; i--)
	{
		if (plansza[i * xPlansza + 1] == '#' && plansza[i * xPlansza + 2] == '#' && plansza[i * xPlansza + 3] == '#'	//Sprawdzenie, czy rzad jest zapelniony
			&& plansza[i * xPlansza + 4] == '#' && plansza[i * xPlansza + 5] == '#' && plansza[i * xPlansza + 6] == '#'
			&& plansza[i * xPlansza + 7] == '#' && plansza[i * xPlansza + 8] == '#' && plansza[i * xPlansza + 9] == '#'
			&& plansza[i * xPlansza + 10] == '#')
		{
			wynik++;	//Dodawanie punktu za ulozozny rzad
			for (int z = i; z >= 0; z--)	//Przesuwanie reszty planszy w dol
			{
				if (z == 0)	//Jezeli z=0, czyli najwyzszy rzad, wypelniany jest on '0', aby nie wyjsc poza granice planszy
				{
					for (int x = 1; x < xPlansza - 1; x++)
					{
						plansza[z * xPlansza + x] = '0';
					}
				}
				else
				{
					for (int x = 1; x < xPlansza - 1; x++)
					{
						plansza[z * xPlansza + x] = plansza[(z - 1) * xPlansza + x];
					}
				}
				i++;	//i jest zwiekszane o 1, gdyz tam, gdzie usunieto linie, mogla pojawic sie nastepna i trzeba ja znowu sprawdzic
			}
		}
	}

	return wynik;
}

int graj()	//Funkcja, ktora jest uzyta aby rozpoczac gre, a po rozgrywce zwrocic uzyskany wynik
{
	//Utworzenie bufora w celu wyswietlania gry na ekranie
	wchar_t screen[xScreen * yScreen];	//Tablica screen zawiera wszystkie znaki ktore beda sie wyswietlac na ekranie
	for (int i = 0; i < xScreen * yScreen; i++)
	{
		screen[i] = L' ';
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	int wynik = 0;	//Koncowy wynik gry
	bool koniecGry = false;	//Informacja, czy gracz juz przegral
	srand(time(0));//Ziarno generatora losowych liczb uzytych do losowania kolejnych klockow

	string klocki[7];		//Utworzenie klockow w tablicy string
	//Tetromino "I"
	klocki[0].append("0010");
	klocki[0].append("0010");
	klocki[0].append("0010");
	klocki[0].append("0010");
	//Tetromino "L"
	klocki[1].append("0100");
	klocki[1].append("0100");
	klocki[1].append("0110");
	klocki[1].append("0000");
	//Tetromino "J"
	klocki[2].append("0010");
	klocki[2].append("0010");
	klocki[2].append("0110");
	klocki[2].append("0000");
	//Tetromino "T"
	klocki[3].append("0000");
	klocki[3].append("1110");
	klocki[3].append("0100");
	klocki[3].append("0000");
	//Tetromino "S"
	klocki[4].append("0000");
	klocki[4].append("0011");
	klocki[4].append("0110");
	klocki[4].append("0000");
	//Tetromino "Z"
	klocki[5].append("0000");
	klocki[5].append("1100");
	klocki[5].append("0110");
	klocki[5].append("0000");
	//Tetromino "O"
	klocki[6].append("0000");
	klocki[6].append("0110");
	klocki[6].append("0110");
	klocki[6].append("0000");

	char plansza[xPlansza * yPlansza];	//Utworzenie planszy gry
	for (int i = 0; i < yPlansza; i++)	//Wypelnienie pola gry znakiem '0' i zaznaczenie krawedzi '@'
	{
		for (int j = 0; j < xPlansza; j++)
		{
			if (j == 0 || j == xPlansza - 1 || i == yPlansza - 1)
			{
				plansza[i * xPlansza + j] = '@';
			}
			else
			{
				plansza[i * xPlansza + j] = '0';
			}
		}
	}

	int bag[7]{};//Tablica, ktora przechowuje 7 kolejnych (roznych) klockow
	losuj(bag);//Losowanie kolejnych 7 klockow
	int licznikBag = 1;	//Informacja o tym gdzie w tablicy bag znajduje sie nastepny klocek
	int nKlocek = bag[licznikBag]; //Informacja o klocku, ktory pojawi sie nastepny - pocztakowo ustawiony na 2 element tablicy bag

	//Informacje o obecnym stanie klocka
	int oObrot = 0;	//Informacja o obecnym obrocie klocka
	int oKlocek = bag[licznikBag - 1];//Informacja o ksztalcie obecnego klocka (gdzie jest przechowywany w tablicy klocki)
	int oy = 0;	//Pozycja y gornej lewej krawedzi obecnego klocka
	int	ox = 3;	//Pozycja x gornej lewej krawedzi obecnego klocka


	int input = 0;	//Przechowuje informacje, jaki klawisz wcisnal gracz

	int licznik = 0;//Przechowuje informacje o tym, ile odbylo sie 'tickow' od ostatniego popchniecia w dol klocka przez gre
	int odliczanie = 15;//Liczba 'tickow' do momentu, w ktorym popchniety zostanie w dol obecny klocek
	int licznikLinii = 0;//Informacja o tym, ile linii do tej pory ulozyl gracz
	int licznikKlockow = 0;//Informacja o tym, ile do tej pory spadlo klockow

	while (koniecGry == false)	//Petla gry - wykonuje sie, dopoki gracz nie przegra
	{
		Sleep(75);	//1 "tick" gry odbywa sie co 75 ms
		licznik++;

		if (licznik == odliczanie)	//Klocek przesuwany jest w dol
		{

			if (kolizja(oy + 1, ox, oKlocek, oObrot, klocki, plansza) == false)
			{
				oy++;
			}
			else//Jezeli nie da sie przesunac klocka w dol, nalezy go unieruchomic 
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (klocki[oKlocek][obrot(i, j, oObrot)] == '1')
						{
							plansza[(oy + i) * xPlansza + ox + j] = '#';
						}
					}
				}

				licznikKlockow++;
				if (licznikKlockow % 10 == 0 && odliczanie > 1)	//Co 10 klockow, gra przyspiesza o 75 ms
				{
					odliczanie--;
				}

				//Przywrocenie stanu klocka do pierwotnego stanu i wybranie nowego klocka
				oKlocek = nKlocek;//Przypisanie nastepnego klocka do obecnego klocka
				oObrot = 0;
				oy = 0;
				ox = 3;

				licznikBag++;//Wybranie nastepnego klocka z tablicy bag
				nKlocek = bag[licznikBag];
				if (licznikBag == 6)	//Jezeli nastepny klocek znajduje sie na miejscu 7 tablicy bag, nalezy wylosowac nowa tablice bag i przywrocic licznikBag do 0
				{
					licznikBag = -1; //-1 zamiast 0, bo licznik inkrementowany jest przed przypisaniem klocka do nKlocek (gdyby bylo 0, zostalby pominiety 1 klocek z tablicy bag
					losuj(bag);
				}


				if (kolizja(oy, ox, oKlocek, oObrot, klocki, plansza) == true)	//Jesli klocek nie pasuje po przywrocenie pierwotnego stanu, gra konczy sie
				{
					koniecGry = true;
				}
			}

			switch (sprawdzLinie(plansza))
			{
			case 1:	//Jesli ulozono 1 linie, dodawane jest 10 punktow
				wynik += 10;
				licznikLinii++;
				break;
			case 2://Jesli ulozono 2 linie, dodawane jest 25 punktow
				wynik += 25;
				licznikLinii += 2;
				break;
			case 3://Jesli ulozono 3 linie, dodawane jest 50 punktow
				wynik += 50;
				licznikLinii += 3;
				break;
			case 4://Jesli ulozono 4 linie, dodawane jest 100 punktow
				wynik += 100;
				licznikLinii += 4;
				break;
			default://Jesli nie ulozono zadnej linii, nie sa dodawane zadne punkty
				break;
			}

			licznik = 0;
		}

		//Sprawdzenie, czy gracz nacisnal jakis klawisz
		if (GetAsyncKeyState(VK_UP))	//Strzalka w gore
		{
			input = 1;
		}
		else if (GetAsyncKeyState(VK_DOWN))	//Strzalka w dol
		{
			input = 2;
		}
		else if (GetAsyncKeyState(VK_RIGHT))	//Strzalka w prawo
		{
			input = 3;
		}
		else if (GetAsyncKeyState(VK_LEFT))	//Strzalka w lewo
		{
			input = 4;
		}

		//Sterowanie klockiem
		switch (input)
		{
		case 1:	//Obrot
			if (kolizja(oy, ox, oKlocek, oObrot + 1, klocki, plansza) == false)	//Zanim zostanie wykonany ruch, trzeba sprawdzic czy jest mozliwy
			{																	//(czyli czy klocek nie wyjdzie poza dozwolone puste pole)	
				oObrot++;
			}
			break;
		case 2:	//Popchniecie w dol
			if (kolizja(oy + 1, ox, oKlocek, oObrot, klocki, plansza) == false)
			{
				oy++;
			}
			break;
		case 3:	//Przesuniecie w prawo
			if (kolizja(oy, ox + 1, oKlocek, oObrot, klocki, plansza) == false)
			{
				ox++;
			}
			break;
		case 4:	//Przesuniecie w lewo
			if (kolizja(oy, ox - 1, oKlocek, oObrot, klocki, plansza) == false)
			{
				ox--;
			}
			break;
		default:
			break;
		}
		input = 0; //Przypisanie 'input' 0, aby ruch nie powtorzyl sie w nastepnym 'ticku'

		//Rysowanie 
		for (int i = 0; i < yPlansza; i++)	//Rysowanie planszy 
		{
			for (int j = 0; j < xPlansza; j++)
			{
				if (plansza[i * xPlansza + j] == '0')
				{
					screen[i * xScreen + j] = ' ';
				}
				else
				{
					screen[i * xScreen + j] = plansza[i * xPlansza + j];
				}
			}
		}

		for (int i = 0; i < 4; i++)	//Rysowanie obecnego klocka 
		{
			for (int j = 0; j < 4; j++)
			{
				if (klocki[oKlocek][obrot(i, j, oObrot)] == '1')
				{
					screen[(oy + i) * xScreen + ox + j] = '%';	//Klocek jest odzwierciedlany na tabeli "screen"
				}
			}
		}

		swprintf_s(&screen[1 * xScreen + xPlansza + 2], 22, L"WYNIK: %14d", wynik);	//Rysowanie wyniku
		swprintf_s(&screen[2 * xScreen + xPlansza + 2], 22, L"ULOZONE LINIE: %6d", licznikLinii);//Rysowanie licznika linii
		swprintf_s(&screen[3 * xScreen + xPlansza + 2], 17, L"NASTEPNY KLOCEK:");	//Rysowanie nastepnego klocka
		for (int i = 0; i < 4; i++)	//Rysowanie ksztaltu nastepnego klocka obok planszy
		{
			for (int j = 0; j < 4; j++)
			{
				if (klocki[nKlocek][i * 4 + j] == '0')
				{
					screen[(5 + i) * xScreen + xPlansza + 7 + j] = ' ';
				}
				else if (klocki[nKlocek][i * 4 + j] != '0')
				{
					screen[(5 + i) * xScreen + xPlansza + 7 + j] = '%';
				}
			}
		}
		WriteConsoleOutputCharacter(hConsole, screen, xScreen * yScreen, { 0,0 }, &dwBytesWritten);		//Wyswietlanie gry na ekranie
	}

	CloseHandle(hConsole);
	cout << "Koniec gry! Twoj wynik: " << wynik << endl;
	cout << "Wcisnij dowolny klawisz, aby kontynuowac " << endl;
	Sleep(1000);//Krotki odstep czasowy, zeby gracz mogl zobaczyc wynik zanim funkcja sie zakonczy
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Aby przyciski naciskane podczas gry nie aktywowaly getch()
	_getch();

	return wynik;
}

void pomocUzytkownika()
{
	system("cls");
	cout << "1. GRA" << endl;
	cout << endl;
	cout << "Sterowanie: " << endl;
	cout << "Strzalka w gore - obrocenie klocka" << endl;
	cout << "Strzalka w dol - popchniecie klocka w dol" << endl;
	cout << "Strzalka w prawo - popchniecie klocka w prawo" << endl;
	cout << "Strzalka w lewo - popchniecie klocka w lewo" << endl;
	cout << endl;
	cout << "Celem gry jest zdobycie jak najwiekszej liczby punktow." << endl;
	cout << "Aby zdobyc punkty, nalezy ulozyc klocki w taki sposob, aby pozioma linia na planszy byla zapelniona." << endl;
	cout << "Wtedy ta linia zniknie, a klocki ponad nia przesuna sie w dol." << endl;
	cout << "Im wiecej lin zostanie usunietych w jednym ruchu, tym wiecej zostanie przydzielonych punktow." << endl;
	cout << endl;
	cout<<"UWAGA: Aby gra wyswietlala sie poprawnie, szerokosc okna konsoli musi byc rowna 120," << endl;
	cout << "a wysokosc nie mniejsza niz 30." << endl;
	cout << endl;
	cout << "2. TABELA WYNIKOW" << endl;
	cout << endl;
	cout << "W tabeli wynikow przechowywane jest 10 najwiekszych wynikow." << endl;
	cout << "Sa one zapisywane w pliku \"wyniki.txt\"." << endl;
	cout << endl;
	cout << "Wcisnij dowolny przycisk, aby kontynuowac" << endl;
	_getch();
}

void sortowanieWynikow(wyniki tabelaWynikow[10]) //Funkcja sortujaca tablice z wynikami (sortowanie babelkowe)
{
	wyniki temp;
	for (int a = 0; a < 10; a++)
	{
		for (int b = a + 1; b < 10; b++)
		{
			if (tabelaWynikow[b].wynik > tabelaWynikow[a].wynik)
			{
				temp = tabelaWynikow[b];
				tabelaWynikow[b] = tabelaWynikow[a];
				tabelaWynikow[a] = temp;
			}
		}
	}
}

void zapisanieWynikow(wyniki tabelaWynikow[10])	//funkcja zapisujaca do pliku wyniki.txt obecne wyniki
{

	fstream plik;
	plik.open("wyniki.txt", ios::out);
	for (int i = 0; i < 10; i++)
	{
		plik << tabelaWynikow[i].wynik << endl;
		plik << tabelaWynikow[i].imie << endl;
	}
	plik.close();
}

int odczytaniePliku(wyniki tabelaWynikow[10])	//Jesli juz istnieje plik wyniki.txt, funkcja wpisuje je do tabeli wynikow
{
	int nrLinii = 1;
	int i = 0;
	string linia;
	fstream plik;
	plik.open("wyniki.txt", ios::in);

	if (plik.good() == false)
	{
		return 0;
	}

	while (getline(plik, linia))
	{
		switch (nrLinii % 2)
		{
		case 0:
			tabelaWynikow[i].imie = linia;
			i++;
			break;
		case 1:
			tabelaWynikow[i].wynik = stoi(linia);
			break;
		}
		nrLinii++;
	}
	plik.close();

	return i;
}

void wyswietlTabelaWynikow(wyniki tabelaWynikow[10])	//Funkcja do wyswietlenia tabeli wynikow
{
	cout << "Najwyzsze wyniki:" << endl;
	for (int x = 0; x < 10; x++)
	{
		cout << tabelaWynikow[x].wynik << " - " << tabelaWynikow[x].imie << endl;
	}
	cout << "Wcisnij dowolny przycisk, aby kontynuowac" << endl;
	_getch();
}