/*
Autor programu: Filip Bienkowski
Numer indeksu: 407686

Nastepujacy program:

1) Czyta dane, bedace okreslonym stanem planszy przed wykonaniem jakichkolwiek przeksztalcen
2) Ujednolica dlugosci wierszy, wstawiajac spacje. Jest to wykonywawane z dwoch powodow: po pierwsze, dzieki temu mozemy po prostu odejmowac lub dodawac do danego 
indeksu wartosc (dlugosc_wiersza+1) by dowiedziec sie co jest pod lub nad nim. Po drugie, gdybysmy tego nie robili to w niektorych wypadkach nie bylibysmy w stanie 
sprawdzic czy pola nad lub pod danym elementem sa puste, bo nie byloby ich w tablicy.
3) Porzadkuje tablice. Najpierw opuszcza wszystkie znaki ktore maja pod soba wolne pola najnizej jak sie da. Nastepnie usuwa wszystkie wiersze w ktorych nie ma 
widocznego znaku (chociaz w specyfikacji nie jest napisane wprost ze trzeba to zrobic, to analiza przykladowych danych jasno pokazuje ze trzeba). Na sam koniec 
usuwa wszystkie kolumny bez widocznych znakow.
4) Wymazuje grupe widocznych znakow, zaczynajaca sie w lewym dolnym rogu planszy, w sposob podobny do tego jak dzieje sie to w grze SameGame. Dowolny znak nalezy do 
grupy znakow, jesli jest taki sam jak znak startowy w lewym dolnym rogu planszy, i da sie przejsc do tego znaku od znaku startowego wykonujac tylko kroki do 
bezposrednich sasiadow w wierszu lub kolumnie rowniez bedacych takim samym znakiem jak znak startowy.
5) Ponownie porzadkuje plansze
6) Usuwa spacje na koncach wierszy, ktore nie sa juz nam potrzebne i ktorych nie chcemy wypisywac. Warto zaznaczyc, ze od tego momentu wiersze przestaja miec rowna 
dlugosc i zmienna dlugosc_wiersza staje sie bezsensowna
7) Wypisujemy wynik

Funkcja main powinna zwracac 0, a kod kompilujemy poleceniem: gcc -std=c11 -pedantic -Wall -Wextra -Werror -fstack-protector-strong -g SG.c -o SG 
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void ujednolicenie(char **plansza, int *rozmiar, int dlugosc_wiersza) /* ujednolica dlugosci wierszy dodajac (chwilowo) spacje, co ulatwi operowanie danymi */
{
	int i = 0;
	int p = 0; /* dlugosc aktualnego wiersza */
	for (; i < (*rozmiar); i++)
	{
		if ((*plansza)[i] == '\n')
		{
			if (p < dlugosc_wiersza) /* sprawdzamy czy wiersz ma odpowiednia dlugosc, jak nie to go wydluzamy */
			{
				int rozmiar_przed_powiekszeniem = *rozmiar;
				*rozmiar = (*rozmiar)+(dlugosc_wiersza-p);
				*plansza = realloc(*plansza, sizeof(**plansza)*(*rozmiar));
				
				while(p < dlugosc_wiersza)
				{
					int y = rozmiar_przed_powiekszeniem-1; /* indeks ostatniego elementu ktory przesuwamy */
					for(; y >= i; y--)
					{
						(*plansza)[y+1] = (*plansza)[y]; /* robimy miejsce na spacje */
					}
					(*plansza)[i] = ' ';
					p++;
					i++;
					rozmiar_przed_powiekszeniem++;
				} 
			}
			p = -1; /* bo zaraz wykona sie p++, a chcemy wejsc w nowy wiersz z p = 0 */
		}
		p++;
	}	
}

int czytaj_dane(char **plansza, int * dlugosc_wiersza) /* czyta dane, zwraca dlugosc tablicy (n) oraz ustawia dlugosc wiersza */
{
	int n = 0;
	int znak = 'a';
	int maxwiersz = 0;
	int p = 0;
	while (znak != EOF)
	{
		znak = getchar();
		
		if (znak != EOF)
		{
			n++;
			*plansza = realloc(*plansza, sizeof(**plansza)*n);
			(*plansza)[n-1] = znak;
			if (znak == '\n')
			{
				if (p > maxwiersz)
				{
					maxwiersz = p;
				}
				p = 0;
			}
			else 
			{
				p++;
			}
		}
	}
	
	*dlugosc_wiersza = maxwiersz;
	
	return n; 
}

void opusc(char ***plansza, int rozmiar, int dlugosc_wiersza) /* przesuwa znaki w dol */
{
	int i = rozmiar-1-1; /* ostatni element to na pewno \n wiec mozna pominac */
	for (; i >= (0+dlugosc_wiersza+1); i--) /*  nie wykonujemy w linii na samej gorze bo wyjdziemy za tablice */
	{
		if((**plansza)[i] == ' ')
		{
			if((**plansza)[i-(dlugosc_wiersza+1)] != ' ')
			{
				(**plansza)[i] = (**plansza)[i-(dlugosc_wiersza+1)];
				(**plansza)[i-(dlugosc_wiersza+1)] = ' ';
				int x = i+(dlugosc_wiersza+1);
				bool koniec = false;
				while ((x < rozmiar-1) && (!koniec)) /* sprawdzamy czy mozemy pojsc jeszcze nizej */
				{
					if ((**plansza)[x] == ' ')
					{
						(**plansza)[x] = (**plansza)[x-(dlugosc_wiersza+1)];
						(**plansza)[x-(dlugosc_wiersza+1)] = ' ';
						x = x + dlugosc_wiersza+1;
					}
					else /* jesli trafilismy na jakis znak to nizej nie ma sensu schodzic */
					{ 
						koniec = true;
					}
				}	
			}
		}
	}
}

int usun_kolumny_wyznacz(int indeks, int dlugosc_wiersza) /* funkcja wyznacza indeks najwyzszego elementu z usuwanej kolumny */
{
	while ((indeks-(dlugosc_wiersza+1)) >= 0)
	{
		indeks = indeks-(dlugosc_wiersza+1);
	} 
	
	return indeks;
}

void przesun_w_lewo(char ****plansza, int x, int z, int y) /* przesuwamy elementy w tablicy by skrocic tablice, od elementu o indeksie x do z, o y pozycji w lewo */
{
	for (; x <= z; x++) /* dla x > z funkcja nic nie zrobi */
	{
		(***plansza)[x-y] = (***plansza)[x];
	}
}

void usun_kolumny(char ***plansza, int **rozmiar, int **dlugosc_wiersza) /* usuwa puste kolumny */
{
	int i = (**rozmiar)-1-1; /* ostatnia kolumna nigdy nie jest pusta bo mamy '\n' */
	
	for (; i >= ((**rozmiar)-1)-(**dlugosc_wiersza); i--)
	{
		if ((**plansza)[i] == ' ') /* jestesmy na samym dole, nic tu nie spadlo czyli nic wyzej nie ma */
		{
			int a = usun_kolumny_wyznacz(i, **dlugosc_wiersza);
			int b = a+(**dlugosc_wiersza);
			int c = 0;
			while (a <= i)
			{
				if (b > (**rozmiar)-1) /* nie chcemy wyjsc za tablice */
				{
					b = (**rozmiar)-1;
				}
				c++;
				przesun_w_lewo(&plansza, a+1, b, c);
				a = a+(**dlugosc_wiersza)+1;
				b = a+(**dlugosc_wiersza);
			}
			**rozmiar = (**rozmiar)-c;
			**plansza = realloc(**plansza, sizeof(***plansza)*(**rozmiar));		
			**dlugosc_wiersza = (**dlugosc_wiersza)-1; /* nie zmieniamy i, bo wzielismy kolumne z prawej, ktora byla juz sprawdzona */
			i = (**rozmiar)-1-1;
		}
	}
}

void usun_wiersze(char ***plansza, int **rozmiar, int dlugosc_wiersza) /* usuwa puste wiersze */
{
	int i = 0;
	int p = 0; /* ilosc spacji w aktualnym wierszu */
	if (dlugosc_wiersza == 0) /* plansza o dlugosci wiersza 0 to plansza samych \n, czyli usuwamy wszystkie wiersze i tablica jest teraz pusta */
	{
		**rozmiar = sizeof(***plansza)*0;
		**plansza = realloc(**plansza, sizeof(***plansza)*(**rozmiar));
	}
	for (; i < (**rozmiar); i++)
	{
		if ((**plansza)[i] == ' ')
		{
			p++;
		}
		else
		{
			p = 0;
		}
		if (p == dlugosc_wiersza) 
		{
			/* jedyny przypadek w ktorym to bedzie prawda, to wiersz samych ' '. Poniewaz kazdy wiersz konczy sie znakiem '\n', wiemy 
			ze w nowy wiersz wchodzimy zawsze z p==0, a nawet jesli w jednym wierszu najpierw zwiekszymy p, potem wyzerujemy, potem znowu zwiekszymy, to 
			poniewaz dokonalismy po drodze wyzerowania to p bedzie < dlugosc_wiersza */
			
			przesun_w_lewo(&plansza, i+1+1, (**rozmiar)-1, dlugosc_wiersza+1); /* i to przedostatni indeks wiersza do usuniecia, dlatego i+1+1 */
			**rozmiar = (**rozmiar)-(dlugosc_wiersza+1); /* jesli wlasnie bylismy w ostatnim wierszu i byl pusty, to rozmiar = 0  */
			**plansza = realloc(**plansza, sizeof(***plansza)*(**rozmiar));	
			p = 0;
			i = i-dlugosc_wiersza; /*  musimy sprawdzic teraz znowu ten sam wiersz, bo moglismy przeniesc pusty wiersz na pusty wiersz */
		}
	}
}

void uporzadkuj(char **plansza, int *rozmiar, int *dlugosc_wiersza) /* przesuwa znaki w dol i usuwa kolumny */
{
	opusc(&plansza, *rozmiar, *dlugosc_wiersza); 
	usun_wiersze(&plansza, &rozmiar, *dlugosc_wiersza);
	if (*rozmiar > 0) /* jesli usuwajac wiersze wyczyscilismy cala tablice, to nie ma juz czego sprawdzac */
	{
		usun_kolumny(&plansza, &rozmiar, &dlugosc_wiersza);
	}
}

void grupa(char ***plansza, int rozmiar, int dlugosc_wiersza, int start, char znak) /* sprawdzamy siebie i sasiadow */
{
	if (((**plansza)[start] == znak) && ((**plansza)[start] != ' ') && ((**plansza)[start] != '\n'))
	{
		(**plansza)[start] = ' ';
		
		if (start-1 >= 0) /* lewo */
		{
			grupa((plansza), rozmiar, dlugosc_wiersza, start-1, znak);
		}
		if (start+1 <= rozmiar-1) /* prawo */
		{
			grupa((plansza), rozmiar, dlugosc_wiersza, start+1, znak);
		}
		if (start+(dlugosc_wiersza+1) <= rozmiar-1) /* dol */
		{
			grupa((plansza), rozmiar, dlugosc_wiersza, start+(dlugosc_wiersza+1), znak);
		}
		if (start-(dlugosc_wiersza+1) >= 0) /* gora */
		{
			grupa((plansza), rozmiar, dlugosc_wiersza, start-(dlugosc_wiersza+1), znak);
		}
	}
}

void wymaz_grupe(char **plansza, int rozmiar, int dlugosc_wiersza) /* wymazuje grupe znakow */
{
	if (rozmiar > 0) /* nie robimy nic w pustej tablicy */
	{
		int start = rozmiar-1-dlugosc_wiersza; /* znajdujemy znak w lewym dolnym rogu */
		char znak = (*plansza)[start];
		grupa (&plansza, rozmiar, dlugosc_wiersza, start, znak);
	}
}

void wypis(char **plansza, int rozmiar) /* wypisuje plansze */
{
	int i = 0;
	for(; i < rozmiar; i++)
	{
		putchar((*plansza)[i]);
	}
}

void usun_spacje(char **plansza, int *rozmiar) /* usuwamy zbedne spacje na koncach wierszy, po wywolaniu tej funkcji zmienna dlugosc_wiersza staje sie bezsensowna */
{
	int i = 0;
	bool czy_usuwamy = false;

	for(; i < *rozmiar; i++)
	{
		if ((*plansza)[i] == ' ')
		{
			czy_usuwamy = true;
		}
		else if ((*plansza)[i] == '\n')
		{
			if (czy_usuwamy)
			{
				int y = 0; /* ile spacji usuwamy */
				i--;
				while ((*plansza)[i] == ' ')
				{
					y++;
					i--;
				}
				char ***plansza_wskaznik = &plansza; /* funkcja przesun potrzebuje int ****, wiec musimy zrobic dwie referencje */
				przesun_w_lewo(&plansza_wskaznik, i+y+1, (*rozmiar)-1, y);
				*rozmiar = (*rozmiar)-y;
				*plansza = realloc(*plansza, sizeof(**plansza)*(*rozmiar));
				czy_usuwamy = false;
			}
		}
		else
		{
			czy_usuwamy = false;
		}
	}
}

int main()
{
	char *plansza = malloc(sizeof(char)*1); /* ta tablica to nasza plansza */
	int rozmiar; /* ilosc elementow w planszy */
	int dlugosc_wiersza; /* dlugosc nie wlicza '\n', po dodaniu (d_w+1) do indeksu otrzymujemy indeks pod nim, odejmujac mamy indeks nad nim */

	rozmiar = czytaj_dane(&plansza, &dlugosc_wiersza); /* wpisujemy dane do tablicy */
	ujednolicenie(&plansza, &rozmiar, dlugosc_wiersza); /* ujednolicamy dlugosci wierszy, dzieki czemu mozemy latwo skakac z wiersza do wiersza */
	uporzadkuj(&plansza, &rozmiar, &dlugosc_wiersza); /* porzadkujemy zgodnie ze specyfikacja */
	wymaz_grupe(&plansza, rozmiar, dlugosc_wiersza); /* wymazujemy grupe znakow */
	uporzadkuj(&plansza, &rozmiar, &dlugosc_wiersza); /* znowu porzadkujemy */
	usun_spacje(&plansza, &rozmiar); /* usuwamy zbedne spacje na koncach wierszy */
	wypis(&plansza, rozmiar); /* rysujemy plansze po przeksztalceniach */
	
	free(plansza);
	
	return 0;
}
