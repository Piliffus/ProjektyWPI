/*
Autor programu: Filip Bienkowski
Numer indeksu: 407686

Zaprezentowany ponizej program sluzy do gry w palindromy.
Gracze wykonuja ruchy naprzemiennie, wprowadzajac litere odpowiadajaca kolumnie,
w ktorej chca umiescic swoj pion (domyslnie z zakresu od a do h, ilosc lub indeksowanie kolumn
mozna zmienic manipulujac liczba po instrukcji define).
Alternatywnie, wprowadzenie znaku "=" tudziez stalej EOF umozliwia zakonczenie gry w dowolnym momencie
Dzialanie poszczegolnych instrukcji opisane zostalo w kodzie.

Wygrywa gracz ktory swoim ruchem ulozy palindrom o dlugosci 5 znakow 
*/

#include <stdio.h>
#include <stdbool.h>

#define WIERSZE 8
#define KOLUMNY 8
#define MINIMUM 97 /* dla char = 97 (NIE CHAR = '97') mamy litere a, minimum sluzy wyznaczeniu litery od ktorej zacynamy nazywac kolumny */
#define BAZA 48 /*  49 - 1 , 50 - 2, do tej liczby dodajemy numer gracza by dostac jego pion */
#define DLUGOSC_PALINDROMU 5

void ustaw_plansze(char plansza[][KOLUMNY]) /* zeruje plansze */
{
	int i=0;
	int y=0;

	for (;i<WIERSZE;++i)
	{
		for (;y<KOLUMNY;++y)
		{
			plansza[i][y] = '-';
		}               	
		y=0;
	}
}

bool sprawdzam(char plansza[][KOLUMNY], int znak) /* prawda dla odpowiednich danych falsz dla zlych */
{
	bool zwracam = true;

	if (((znak)<(MINIMUM))||((znak)>=(MINIMUM+KOLUMNY))) /* cos takiego oznacza ze znak nie jest z zakresu ktory nas interesuje */
	{
		zwracam = false;
        }
	else /* wiemy ze znak jest ok, teraz sprawdzamy czy mozna go zmiescic */
	{
		if ((plansza[0][znak-MINIMUM])!=('-'))
			{
			zwracam = false;
			}
	}
	return zwracam;
}


void rysuj(char plansza[][KOLUMNY], int kto_teraz, bool zwyciestwo) /* podaje aktualny stan planszy, oraz kto teraz pisze */
{
	int i=0;
	int y=0;
	char kolumna;
	
	for (;i<WIERSZE;++i)
	{
		for (;y<KOLUMNY;++y)
		{
			putchar(' ');
			putchar(plansza[i][y]);
		}
		
		y=0;
		putchar('\n');
	}

	int h = MINIMUM;

	for (;h<(MINIMUM+KOLUMNY);++h) /* minimum standardowo = 97; 97 - a, 98 - b ......... 122 - z */
	{
		kolumna = h;
		putchar(' ');
		putchar(kolumna);
	}	
	
	if (!zwyciestwo)
	{
		printf("\n%d:\n", kto_teraz);
	}
	else 
	{
		printf("\n%d.\n", kto_teraz);
	}
}

void wstaw_znak(char plansza[][KOLUMNY], int znak, int kto_teraz, int * jaki_wiersz, int * jaka_kolumna) /* aktualizuje tablice */
{
	bool sukces = false;
	int i = 1;

	while (!sukces)
	{
		if ((plansza[WIERSZE-i][znak-MINIMUM]) == '-')
		{
			plansza[WIERSZE-i][znak-MINIMUM] = (BAZA+kto_teraz);
			*jaki_wiersz = WIERSZE-i;
			*jaka_kolumna = znak-MINIMUM;
			sukces = true;
		}
		else i++;
	}
}

/* mozemy dodac te funkcje jesli chcemy sprawdzac czy w planszy sa jeszcze wolne pola
void czy_zapchana(char plansza[][KOLUMNY], bool * * koniecgry)  
{
	int i=0;
	bool wolne = false; 
	while ((i<KOLUMNY)&&(!wolne)) 
	{
		if (plansza[0][i]=='-') 
	{
			wolne = true;
		}
		
		i++;
	}

	if ((i==KOLUMNY)&&(!wolne)) 
	{
		**koniecgry=true;
	}
}
*/

void szukaj_palindromow(char plansza[][KOLUMNY], bool * * koniecgry, int ktory_gracz, int znak, int * jaki_wiersz2, int * jaka_kolumna2) /* szuka palindromow, wypisuje gracza i konczygre */
{
	bool zwyciestwo = false;
	int jaki_wiersz= *jaki_wiersz2;
	int jaka_kolumna= *jaka_kolumna2;

	if ((jaki_wiersz)<=(WIERSZE-DLUGOSC_PALINDROMU)) /* szukamy pionowego palindromu */
	{
		int i = DLUGOSC_PALINDROMU-1;
		int y = 0;
		bool rozne = false;

		while ((i>DLUGOSC_PALINDROMU/2)&&(!rozne))
		{
			if ((plansza[jaki_wiersz+y][znak-MINIMUM])==(plansza[jaki_wiersz+i][znak-MINIMUM]))
			{
				++y;
				--i;
			}
			else
			{
				rozne=true;
			}
		}
		if (y==DLUGOSC_PALINDROMU/2)
		{
			zwyciestwo=true;
		}
	}
	
	/* szukamy poziomego palindromu */
	{
		int h = jaka_kolumna-(DLUGOSC_PALINDROMU-1);
		bool koniec = false;
		
		for (;((h<=(jaka_kolumna))&&(!koniec));++h)
		{
			if (h<0) /* po prostu zwiekszamy h jesli wyszlismy za tablice z lewej strony */
			{
				h = 0;
			}
			if (((plansza[jaki_wiersz][h])==('-'))||((plansza[jaki_wiersz][h+(DLUGOSC_PALINDROMU-1)])==('-')))  /* takie porownania maja na celu upewnienie sie ze nie porownamy pustego pola */
			{
				
			}
			else if ((plansza[jaki_wiersz][h])==(plansza[jaki_wiersz][h+(DLUGOSC_PALINDROMU-1)]))
			{
				if (((plansza[jaki_wiersz][h+1])==('-'))||((plansza[jaki_wiersz][h+(DLUGOSC_PALINDROMU-1-1)])==('-')))
				{
					
				}
				else if ((plansza[jaki_wiersz][h+1])==(plansza[jaki_wiersz][h+(DLUGOSC_PALINDROMU-1-1)]))
				{
					if((plansza[jaki_wiersz][h+2])!=('-'))
					{
					zwyciestwo=true;
					}
				}
			}
			if (h+1+(DLUGOSC_PALINDROMU-1)>(KOLUMNY-1)) /* jesli w nastepnym obiegu po prawej byloby za malo elementow, to dalej nie spradwzamy */
			{
				koniec = true;
			}
		}	
	}
	
	/* szukamy ukosnego palindromu z lewego dolnego rogu do prawego gornego */
	{
		int x = jaki_wiersz+(DLUGOSC_PALINDROMU-1);
		int q = jaka_kolumna-(DLUGOSC_PALINDROMU-1);
		bool zakoncz = false;
		
		while (((x)>=(jaki_wiersz))&&(!zakoncz))
		{
			if (x>(WIERSZE-1))
			{
				x = WIERSZE-1;
			} 
			if (q<0)
			{
				q = 0;
			}
			if (((plansza[x][q])==('-'))||((plansza[x-(DLUGOSC_PALINDROMU-1)][q+(DLUGOSC_PALINDROMU-1)])==('-'))) /* aby nie porownac - */
			{
				
			}
			else if ((plansza[x][q])==(plansza[x-(DLUGOSC_PALINDROMU-1)][q+(DLUGOSC_PALINDROMU-1)]))
			{
				if (((plansza[x-1][q+1])==('-'))||((plansza[x-(DLUGOSC_PALINDROMU-1-1)][q+(DLUGOSC_PALINDROMU-1-1)])==('-')))
				{
				
				}
				else if ((plansza[x-1][q+1])==(plansza[x-(DLUGOSC_PALINDROMU-1-1)][q+(DLUGOSC_PALINDROMU-1-1)]))
				{
					if ((plansza[x-2][q+2])!=('-'))
					{
					zwyciestwo=true;
					}
				}
			}	
			if ((x-1-(DLUGOSC_PALINDROMU-1))<0)
			{
				zakoncz = true;
			}
			if ((q+1+(DLUGOSC_PALINDROMU-1))>(KOLUMNY-1))
			{
				zakoncz = true;
			}
			x--;
			q++;
		}
	}
	
	/* szukamy ukosnego palindromu z lewego gornego rogu do prawego dolnego */
	{
		int x = jaki_wiersz-(DLUGOSC_PALINDROMU-1);
		int q = jaka_kolumna-(DLUGOSC_PALINDROMU-1);
		bool zakoncz = false;

		while (((x)<=(jaki_wiersz))&&(!zakoncz))
		{
			if (x<0)
			{
				x = 0;
			}
			if (q<0)
			{
				q = 0;
			}
			if (((plansza[x][q])==('-'))||((plansza[x+(DLUGOSC_PALINDROMU-1)][q+(DLUGOSC_PALINDROMU-1)])==('-'))) /* aby nie porownac - */
			{

			}
			else if ((plansza[x][q])==(plansza[x+(DLUGOSC_PALINDROMU-1)][q+(DLUGOSC_PALINDROMU-1)]))
			{
				if (((plansza[x+1][q+1])==('-'))||((plansza[x+(DLUGOSC_PALINDROMU-1-1)][q+(DLUGOSC_PALINDROMU-1-1)])==('-')))
				{

				}
				else if  ((plansza[x+1][q+1])==(plansza[x+(DLUGOSC_PALINDROMU-1-1)][q+(DLUGOSC_PALINDROMU-1-1)]))
				{
					if ((plansza[x+2][q+2])!=('-'))
					{
					zwyciestwo=true;
					}
				}
			}
			if ((x+1+(DLUGOSC_PALINDROMU-1))>(WIERSZE-1))
			{
				zakoncz = true;
			}
			if ((q+1+(DLUGOSC_PALINDROMU-1))>(KOLUMNY-1))
			{
				zakoncz = true;
			}
			x++;
			q++;
		}
	
	}
	if (zwyciestwo)
	{
		rysuj(plansza, ktory_gracz, zwyciestwo);
		**koniecgry=true;
	}
}

void wpisz(char plansza[][KOLUMNY], int ktory_gracz, bool * koniecgry) /* przyjmuje polecenie od gracza, i je rozpatruje */
{
	int znak;
	bool koniec = false;
	int jaki_wiersz = 0;
	int jaka_kolumna = 0;
		
	while(!koniec)
	{  
		int i = 1;
		int znakpodany = ' '; /* od razu dajemy jakas wartosc by miec gwarancje ze wejdziemy do petli */
		int znak2;
		
		while ((znakpodany!='\n')&&(znakpodany!=EOF)) /*tu wprowadzamy dane*/
		{
			znakpodany = getchar();			
			if (i==1)
			{
				znak = znakpodany;
			}
			if (i==2)
			{
				znak2 = znakpodany;
			}
			if ((i==2)&&((znak2)!=('\n')))
			{
				znak = ' '; /* poniewaz wprowadzono wiecej niz jeden znak, ustawiamy znak ktory na pewno zostanie uznany pozniej za bledne dane*/ 
			}
			++i;
		}
		if ((znak!=('='))&&(znak!=EOF)) 
		{
			if(sprawdzam(plansza, znak)) /* komenda byla poprawna wiec wstawiamy znak do tablicy */
			{
				wstaw_znak(plansza, znak, ktory_gracz, &jaki_wiersz, &jaka_kolumna);
				szukaj_palindromow(plansza, &koniecgry, ktory_gracz, znak, &jaki_wiersz, &jaka_kolumna);
				koniec = true; 
			}
			else /* komenda byla bledna wiec rysujemy plansze i probujemy znow */
			{
				rysuj(plansza, ktory_gracz, false); /* dajemy false bo raczej nie powinno sie zdazyc by ktos zwyciezyl wprowadzajac bledne dane */ 
			}
		}
		else /* mamy = albo EOF wiec konczymy */
		{
			*koniecgry = true;
			koniec = true;
		}
	}
}

void zmiana_gracza(int * kto_teraz) /* gracz 1 -> 2 i vice versa */
{
	if (*kto_teraz==1)
	{
		*kto_teraz=2;
	}
	else 
	{
		*kto_teraz=1;
	}
}

int main(void)
{
	bool koniec = false;
	char plansza[WIERSZE][KOLUMNY];
	int kto_teraz = 1;
	
	ustaw_plansze(plansza);
	
	while (!koniec)
	{
		rysuj(plansza, kto_teraz, koniec);
		
		wpisz(plansza, kto_teraz, &koniec);
	
		zmiana_gracza(&kto_teraz);
	}
	
	return 0;
}

