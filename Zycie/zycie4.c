/*

Autor: Filip Bienkowski
Numer Indeksu: 407686
Grupa A. Z.

Niniejszy program stanowi konsolowa implementacje gry w zycie.
Komorki zywe reprezentowane przez znaki '0', komorki martwe przez '.'
Na poczatku ustawiamy pierwsza generacje komorek, za pomoca znakow 0, konczac kazdy wiersz '\n'. Na koncu pierwszej generacji dajemy pusty wiersz z '\n', aby
zatwierdzic dane. Przykladowa pierwsza generacja:
...0.\n
..0..\n
..0..\n
.0...\n
\n

Nastepnie, wprowadzac mozemy cztery rodzaje polecen (albo EOF by zakonczyc prace z programem):
	1. Polecenie puste (\n), liczymy jedna nastepna generacje
	2. Liczba calkowita x (np. 7), liczy x nastepnych generacji
	3. Dwie liczby calkowite x y (np. 7 14), przestawiaja lewy gorny rog okienka z zachowaniem odleglosci
	4. Cztery liczby calkowite x y z q (np. 7 14 17 28), przestawiaja lewy gorny i prawy dolny rog ekranu

Po wypisaniu planszy program podaje informacje o stanie gry w postaci A {B} (C:D) (E:F) [G:H] [I:J], gdzie:
	A to numer generacji, dla generacji początkowej rowny 0
	B to liczba zywych komorek,
	C to numer pierwszego wiersza na planszy, w ktorym jest zywa komorka,
	D to numer pierwszej kolumny na planszy, w ktorej jest zywa komorka,
	E to numer ostatniego wiersza na planszy, w ktorym jest zywa komorka,
	F to numer ostatniej kolumny na planszy, w ktorej jest zywa komorka,
	G H I J to aktualne współrzędne okna. I oraz J powinno byc zawsze wieksze od odpowiednio G i H, w przeciwnym wypadku planszy zwyczajnie nie ma
	Zaklada sie poprawnosc wprowadzanych danych

Wiersze numerujemy od gory do dolu, a kolumny od lewej do prawej.

W programie zastosowana jest tablica przechowujaca "bramki" czyli atrapy dla list zawierajacych komorki. Zastosowana jest funkcja, ktora dla danych w (wiersz) i
k(kolumna)zwraca zawsze taka sama liczbe, dzieki czemu mozna, znajac w i k poszukiwanej komorki, trafic do odpowiedniej listy i tam jej szukac. Jesli ja znajdziemy, to
oznacza ze istnieje zywa komorka o takich w oraz k. W przeciwnym wypadku wiemy ze nie ma takiej zywej komorki, czyli komorka o zadanym w i k jest martwa.

Program kompilujemy poleceniem gcc -std=c11 -pedantic -Wall -Wextra -Werror -fstack-protector-strong -g zycie4.c -o zycie

Podanie znaku konca danych EOF konczy prace z programem

*/

/* pliki naglowkowe */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* ilosc bramek w tablicy z bramkami prowadzacymi do list komorek, wyznaczona eksperymentalnie */
#ifndef ILEBRAM
#	define ILEBRAM 100000
#endif

/* uzywane struktury */

struct wspolrzedna
{
	int k; /* numer kolumny */
	int w; /* numer wiersza */
};
typedef struct wspolrzedna wspolrzedna;

struct komorka
{
	int w; /* wiersz */
	int k; /* kolumna */
	struct komorka* nast; /* wskaznik na nastepna komorke */
};
typedef struct komorka komorka;

struct bramka /* bramka prowadzi nas od hashu do odpowiedniej listy z komorkami, pelni role atrapy */
{
	struct komorka* nast; /* wskaznik na pierwsza komorke przypisana do danego indeksu*/
};
typedef struct bramka bramka;

struct stan_gry /* rozmaite informacje o stanie gry */
{
	wspolrzedna min_zywa;
	wspolrzedna max_zywa;
	bramka *obecna;
	bramka *nastepna;
	bool zmiana;
};
typedef struct stan_gry stan_gry;

/* prototypy funkcji*/

void zamien_bramki(stan_gry*);
void kolejna_generacja(int*, stan_gry*);
int hash(int, int);
bool czy_komorka_zywa(int w, int k, bramka*);
void wypisz_plansze(wspolrzedna, wspolrzedna, bramka*);
void czytaj_dane(wspolrzedna*, wspolrzedna*, bool*, int*, int*, stan_gry*);
void policz_generacje(int, int*, int*, stan_gry*);
void przestaw(int, int, wspolrzedna*, wspolrzedna*);
void przestaw_oba(int, int, int, int, wspolrzedna*, wspolrzedna*);
void ustawienie_komorek(wspolrzedna*, bramka*, int*, stan_gry*);
void ozyj_komorke(int, int, bramka*);
void umrzyj_komorke(int, int, bramka*);
void status(wspolrzedna, wspolrzedna, int, int, stan_gry);
int max(int, int);
bool co_teraz(int, int, int*, int*, stan_gry*);
void inicjalizacja(bramka*);
komorka *pobierz(komorka*, int, int*);
void posprzataj(bramka*);

/* program */

int main()
{
	int numer_generacji = 0;
	int ile_komorek = 0;
	bramka pierwsza[ILEBRAM];
	inicjalizacja(pierwsza);
	bramka druga[ILEBRAM];
	inicjalizacja(druga);
	stan_gry stan;
	stan.obecna = pierwsza;
	stan.nastepna = druga;
	stan.min_zywa.w = 0;
	stan.min_zywa.k = 0;
	stan.max_zywa.w = 0;
	stan.max_zywa.k = 0;
	wspolrzedna rog; /* lewy gorny rog */
	wspolrzedna rog_dolny; /* w i k rogu dolnego powinno byc zawsze wieksze od w i k rogu gornego, jesli jest mniejsze to nie ma okna */
	(rog.k) = 1;
	(rog.w) = 1;
	(rog_dolny.k) = 0;
	(rog_dolny.w) = 0;
	bool koniec = false;

	ustawienie_komorek(&rog_dolny, stan.obecna, &ile_komorek, &stan);
	wypisz_plansze(rog, rog_dolny, stan.obecna);
	status(rog, rog_dolny, numer_generacji, ile_komorek, stan);
	while(!koniec)
	{
		czytaj_dane(&rog, &rog_dolny, &koniec, &numer_generacji, &ile_komorek, &stan);
		if(!koniec) wypisz_plansze(rog, rog_dolny, stan.obecna);
		if(!koniec) status(rog, rog_dolny, numer_generacji, ile_komorek, stan);
	}
	posprzataj(pierwsza);
	posprzataj(druga);

	return 0;
}

void inicjalizacja(bramka* tablica)
{
	int y = 0;

	for(; y < ILEBRAM; y++)
	{
		tablica[y].nast = NULL;
	}
}

int hash(int w, int k) /* liczymy hash ze wspolrzednych, czyli do jakiej bramki idziemy szukac komorki */
{
	int h = 23;
	h = 31 * h + w;
	h = 2137 * h + k;
	return h < 0 ? -h : h; /* zabezpieczenie przed ujemnymi hashami */
}

void kolejna_generacja(int *ile_komorek, struct stan_gry *stan) /* liczymy kolejna generacje */
{
	int w, k;
	int sasiedzi;
	int x = 0;
	int ktore_wywolanie = 1;
	bramka *obecna = stan->obecna;
	bramka *nastepna = stan->nastepna;
	bool pierwszy = true;

	*ile_komorek = 0;

	stan->min_zywa.w = 0;
	stan->min_zywa.k = 0;
	stan->max_zywa.w = 0;
	stan->max_zywa.k = 0;
	stan->zmiana = true;

	while (x < ILEBRAM)
	{
		/* jesli zwrocilo prawde, to mamy komorke do sprawdzenia, i uruchomimy funkcje potem jeszcze raz z tym samym x */
		if(co_teraz(x, ktore_wywolanie, &w, &k, stan))
		{
			if (pierwszy)
			{
				pierwszy = false;
				stan->min_zywa.w = w;
				stan->min_zywa.k = k;
			}
			ktore_wywolanie++;
			int pom_k = -1;
			int pom_w = -1;
			while (pom_w < 2)
			{
				w += pom_w;
				k += pom_k;
				int zyje = czy_komorka_zywa(w, k, obecna);
				sasiedzi =
					  czy_komorka_zywa(w - 1, k - 1, obecna)
					+ czy_komorka_zywa(w - 1, k    , obecna)
					+ czy_komorka_zywa(w - 1, k + 1, obecna)
					+ czy_komorka_zywa(w    , k - 1, obecna)
					+ czy_komorka_zywa(w    , k + 1, obecna)
					+ czy_komorka_zywa(w + 1, k - 1, obecna)
					+ czy_komorka_zywa(w + 1, k    , obecna)
					+ czy_komorka_zywa(w + 1, k + 1, obecna);

				if (sasiedzi == 3) /* martwa komorka z 3 zywymi sasiadami */
				{
					zyje = 1;
				}
				else if ((sasiedzi < 2 || sasiedzi > 3)) /* za duzo lub za malo sasiadow */
				{
					zyje = 0;
				}

				if (zyje)
				{
					ozyj_komorke(w, k, nastepna);
				}
				else
				{
					umrzyj_komorke(w, k, nastepna);
				}

				if ((zyje) || czy_komorka_zywa(w, k, nastepna)) /* patrzymy czy miesci sie na ekranie i jest min lub maks */
				{
					{
						if (w < stan->min_zywa.w) stan->min_zywa.w = w;
						if (k < stan->min_zywa.k) stan->min_zywa.k = k;
					}
					{
						if (w > stan->max_zywa.w) stan->max_zywa.w = w;
						if (k > stan->max_zywa.k) stan->max_zywa.k = k;
					}
				}
				w -= pom_w;
				k -= pom_k;
				pom_k++;
				if (pom_k > 1)
				{
					pom_k = -1;
					pom_w++;
				}
			}
		}
		else /* zwrocilo false, czyli wszystkie komorki dla danego x zostaly sprawdzone */
		{
			x++;
			ktore_wywolanie = 1;
		}
	}

	for (x = 0; x < ILEBRAM; x++)
	{
		komorka *kom = stan->zmiana ? stan->nastepna[x].nast : stan->obecna[x].nast;
		while (kom != NULL)
		{
			*ile_komorek += 1;
			kom = kom->nast;
		}
	}
}

bool czy_komorka_zywa(int w, int k, bramka *bramki) /* sprawdzamy czy komorka o danych wspolrzednych jest zywa */
{
	int x = hash(w, k) % ILEBRAM;
	struct komorka *akt = bramki[x].nast;
	while (akt != NULL)
	{
		if (akt->w == w && akt->k == k)
		{
			return true;
		}
		akt = akt->nast;
	}

	return false;
}

void wypisz_plansze(wspolrzedna rog, wspolrzedna rog_dolny, bramka *bramki) /* rysuje plansze */
{
	int k = rog.k; /* kolumny */
	int w = rog.w; /* wiersze */

	for(; w <= rog_dolny.w; w++)
	{
		for (; k <= rog_dolny.k; k++)
		{
			if (czy_komorka_zywa(w, k, bramki))
			{
				putchar('0');
			}
			else
			{
				putchar('.');
			}
		}
		k = rog.k; /* zresetowanie kolumny */
		putchar ('\n');
	}
}

void czytaj_dane(wspolrzedna* rog, wspolrzedna* rog_dolny, bool* koniec, int *numer_generacji, int *ile_komorek, stan_gry *stan) /* odczytuje znaki z wejscia */
{
	int x;
	int liczby[4];
	int y = -1;

	x = 'a';

	while ((x != EOF) && (x != '\n'))
	{
		x = getc(stdin);
		if ((x == '\n') || (x == ' ')) /* przy tych znakach nic nie robimy */
		{
		}
		else if (x == EOF) /* koniec danych */
		{
			*koniec = true;
		}
		else /* liczba calkowita */
		{
			y++;
			ungetc(x, stdin);
			scanf("%d", &(liczby[y]));
		}
	}
	if (x == EOF) /* koniec */
	{
	}
	else if (y == -1) /* zadnych liczb*/
	{
		policz_generacje(1, numer_generacji, ile_komorek, stan);
	}
	else if (y == 0) /* jedna liczba */
	{
		policz_generacje(liczby[0], numer_generacji, ile_komorek, stan);
	}
	else if (y == 1) /* dwie liczby */
	{
		przestaw(liczby[0], liczby[1], rog, rog_dolny);
	}
	else if (y == 3) /* cztery liczby */
	{
		przestaw_oba(liczby[0], liczby[1], liczby[2], liczby[3], rog, rog_dolny);
	}
}

void policz_generacje(int ile, int *numer_generacji, int *ile_komorek, struct stan_gry *stan) /* liczy "ile" nastepnych generacji */
{
	int x = 1;
	*numer_generacji = *numer_generacji + ile;

	for (; x <= ile; x++)
	{
		kolejna_generacja(ile_komorek, stan);
		if (stan->zmiana) zamien_bramki(stan);
	}
}

void przestaw(int w, int k, wspolrzedna* rog, wspolrzedna* rog_dolny) /* przestawia lewy gorny rog okna z zachowaniem odleglosci */
{
	(*rog_dolny).w = w + ((*rog_dolny).w - (*rog).w);
	(*rog_dolny).k = k + ((*rog_dolny).k - (*rog).k);
	(*rog).w = w;
	(*rog).k = k;
}

void przestaw_oba(int w, int k, int w_dolny, int k_dolny, wspolrzedna* rog, wspolrzedna* rog_dolny) /* przestawia oba rogi okna */
{
	(*rog).w = w;
	(*rog).k = k;
	(*rog_dolny).w = w_dolny;
	(*rog_dolny).k = k_dolny;
}

void umrzyj_komorke(int w, int k, bramka *tabramki) /* zywa komorka staje sie martwa */
{
	int x = hash(w, k) % ILEBRAM;
	struct komorka *akt = tabramki[x].nast, *pop = NULL;
	while (akt != NULL)
	{
		if (akt->w == w && akt->k == k)
		{
			if (pop == NULL)
			{
				tabramki[x].nast = akt->nast;
			}
			else
			{
				pop->nast = akt->nast;
			}
			free(akt);
			return;
		}

		pop = akt;
		akt = akt->nast;
	}
}

void ozyj_komorke(int w, int k, bramka *tabramki) /* martwa komorka staje sie zywa */
{
	int x = hash(w, k) % ILEBRAM;
	if (tabramki[x].nast == NULL)
	{
		tabramki[x].nast = malloc(sizeof(struct komorka) * 1);
		tabramki[x].nast->w = w;
		tabramki[x].nast->k = k;
		tabramki[x].nast->nast = NULL;
	}
	else
	{
		komorka* akt = tabramki[x].nast;
		bool koniec = false;
		while(!koniec)
		{
			if (akt->w == w && akt->k == k)
			{
				koniec = true;
			}
			else
			{
				if (akt->nast == NULL)
				{
					akt->nast = malloc(sizeof(struct komorka) * 1);
					akt->nast->w = w;
					akt->nast->k = k;
					akt->nast->nast = NULL;
					koniec = true;
				}
				else
				{
					akt = akt->nast;
				}
			}
		}
	}
}

void ustawienie_komorek(wspolrzedna* rog_dolny, bramka *tabramki, int *ile_komorek, stan_gry *stan) /* podanie poczatkowego stanu planszy */
{
	int x = 'a';
	int w = 1; /* w jakim wierszu jestesmy */
	int k = 0; /* w jakiej kolumnie jestesmy */
	int k_max = 0;
	int w_max = 0;
	bool koniec = false;

	while(!koniec)
	{
		k++;
		x = getc(stdin);
		if (x == '0')
		{
			ozyj_komorke(w, k, tabramki);
			*ile_komorek += 1;
			if (*ile_komorek == 1)
			{
				stan->min_zywa.w = w;
				stan->min_zywa.k = k;
			}
			if (w < stan->min_zywa.w) stan->min_zywa.w = w;
			if (k < stan->min_zywa.k) stan->min_zywa.k = k;
			if (w > stan->max_zywa.w) stan->max_zywa.w = w;
			if (k > stan->max_zywa.k) stan->max_zywa.k = k;
		}
		else if (x == '\n')
		{
			if(k == 1)
			{
				w_max = w;
				koniec = true;
			}
			else
			{
				if (k > k_max)
				{
					k_max = k;
				}
				w++;
				k = 0;
			}
		}
	}
	(*rog_dolny).w = max(0, w_max - 1);
	(*rog_dolny).k = max(0, k_max - 1);
}

void status(wspolrzedna rog_gorny, wspolrzedna rog_dolny, int numer_generacji, int ile_komorek, struct stan_gry stan) /* drukuje status */
{
	printf("%d {%d} (%d:%d) (%d:%d) [%d:%d] [%d:%d]\n", numer_generacji, ile_komorek, stan.min_zywa.w, stan.min_zywa.k, stan.max_zywa.w, stan.max_zywa.k, rog_gorny.w, rog_gorny.k, rog_dolny.w, rog_dolny.k);
}

void zamien_bramki(stan_gry *stan) /* aktualizuje wskazniki po policzeniu generacji */
{
	bramka *pom = stan->obecna;
	stan->obecna = stan->nastepna;
	stan->nastepna = pom;
}

int max(int a, int b) /* zwraca wieksza z podanych dwoch liczb */
{
	return a > b ? a : b;
}

komorka *pobierz(komorka *glowa, int ktora, int *max) /* funkcja pomocnicza dla co_teraz*/
{
	int h;
	*max = 0;
	for (h = 1; h < ktora && glowa; h++)
	{
		glowa = glowa->nast;
		if (glowa == NULL)
		{
			return NULL;
		}
		*max += 1;
	}
	return glowa;
}

bool co_teraz(int x, int ktore_wywolanie, int *w, int *k, stan_gry *stan) /* funkcja sprawdza, dla jakiej komorki sprawdzamy teraz sasiadow przy liczeniu generacj*/
{
	komorka* pom = NULL;
	int max;

	if (x >= ILEBRAM) /* nie mamy takiej bramki */
	{
		return false;
	}
	else
	{
		pom = pobierz(stan->obecna[x].nast, ktore_wywolanie, &max);
		if (pom == NULL)
		{
			pom = pobierz(stan->nastepna[x].nast, ktore_wywolanie - max, &max);
		}
		if (pom == NULL) return 0;

		*w = pom->w;
		*k = pom->k;
	}

	return true;
}

void posprzataj(bramka *tablica) /* usuwa listy po zakonczeniu pracy z programem */
{
	komorka *pom = NULL;
	int y = 0;

	for(; y < ILEBRAM; y++)
	{
		pom = tablica[y].nast;
		while(pom != NULL)
		{
			komorka *pompom = pom;
			pom = pom->nast;
			free(pompom);
		}
	}
}
