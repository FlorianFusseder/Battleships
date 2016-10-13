#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <winbase.h>

//HIER FELDGRÖSSE ÄNDERN!!!

#define feldgrx 10
#define feldgry 10
typedef char ARRAY[feldgry + 4][feldgrx + 4];
typedef char NAME[100];


//UNTERFUNKTION FÜR BEWEGUNG
int bewegung_schuss(ARRAY, ARRAY);
void printspielfeld(ARRAY);
int go(char, int*, int*, ARRAY, ARRAY, char);
void befuellen(ARRAY);
/////////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION SCHIFFE BELEGEN
int schiffe_positionieren(ARRAY, int, int);
int schiffsposi(char, int*, int*, ARRAY, char, int, int);
void printspielfeld_posi(ARRAY);
void befuellen_feldposi(ARRAY);
void löschen(ARRAY, int**, int**);
int mengeboot(ARRAY);
////////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION ABGLEICHEN
void ende(int, NAME, NAME);
int zählung(ARRAY);
int versenkt(int**, int**, ARRAY, ARRAY);
int positionsregelnanfang(ARRAY, int**, int**, int);
int positionsregelndazwischen(ARRAY, int**, int**, int);
////////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION SOUNDS
void sound_hit();
void sound_miss();
void sound_versenkt();
void sound_win();
///////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION SPIELER
int spieler1(ARRAY, ARRAY, NAME);
int spieler2(ARRAY, ARRAY, NAME);
int spiel(NAME, NAME, ARRAY, ARRAY, ARRAY, ARRAY);
//////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION OPTISCHES
void name_spielerzuweißung(NAME, NAME);
void start();
//////////////////////////////////////////////////////////////////////////////

int main() {

	int win = 0;
	NAME name1;
	NAME name2;
	ARRAY feld1;
	ARRAY feldpositionierung1;
	ARRAY feld2;
	ARRAY feldpositionierung2;

	start();
	ende(spiel(name1, name2, feld1, feldpositionierung1, feld2, feldpositionierung2), name1, name2);
	return 0;

}

//UNTERFUNKTION FÜR BEWEGUNG
int bewegung_schuss(ARRAY feld, ARRAY feldpositionierung) {


	int i = 0;
	int y1 = 2;
	int x1 = 2;
	char original;
	char _move;


	while (i < 1) {
		system("cls");
		original = feld[y1][x1];
		feld[y1][x1] = 'x';
		printspielfeld(feld);
		_move = _getch();
		i = go(_move, &y1, &x1, feld, feldpositionierung, original);
	}

	if (i == 2) {
		return 0;
	}

}
void printspielfeld(ARRAY feld) {

	int y, x;

	printf("\n\n");
	for (y = 2; y < feldgry + 2; y++) {
		printf("\t\t    ");
		for (x = 2; x < feldgrx + 2; x++) {
			printf("%c", feld[y][x]);
		}
		printf("\n");
	}


}
int go(char ch, int* y, int* x, ARRAY feld, ARRAY feldpositionierung, char orginal) {

	switch (ch) {
	case 'a':
		*x = *x - 1;

		if (*x < 2) {
			*x = *x + feldgrx;
			feld[*y][*x - feldgrx + 1] = orginal;
			return 0;
		}

		feld[*y][*x + 1] = orginal;
		return 0;




	case 'd':
		*x = *x + 1;
		if (*x > feldgrx + 1) {
			*x = *x - feldgrx;
			feld[*y][*x + feldgrx - 1] = orginal;
			return 0;
		}

		feld[*y][*x - 1] = orginal;
		return 0;


	case 'w':
		*y = *y - 1;
		if (*y < 2) {
			*y = *y + feldgry;
			feld[*y - feldgry + 1][*x] = orginal;
			return 0;
		}

		feld[*y + 1][*x] = orginal;
		return 0;


	case 's':
		*y = *y + 1;
		if (*y > feldgry + 1) {
			*y = *y - feldgry;
			feld[*y + feldgry - 1][*x] = orginal;
			return 0;
		}

		feld[*y - 1][*x] = orginal;
		return 0;




	case 'c':
		return 0;

	case 'e':
		feld[*y][*x] = 's';
		if (feld[*y][*x] == 's'&&feldpositionierung[*y][*x] == 'o') {
			feld[*y][*x] = 't';
			sound_hit();
			if (versenkt(&y, &x, feld, feldpositionierung) == 1) {
				printf("\t\t    versenkt\n");
				sound_versenkt();
			}
			system("cls");
			printspielfeld(feld);
			Sleep(500);
			if (zählung(feld) == 30) {
				return 2;
			}
			return 0;
		}


		else {
			feld[*y][*x] = ' ';
			sound_miss();
			return 1;

		}

	default:
		system("cls");
		printf("Nicht belegte Taste\n");
		system("pause");
		feld[*y][*x] = orginal;
		return 0;

	}

}
void befuellen(ARRAY feld) {
	int y, x;

	for (y = 0; y < feldgry + 4; y++) {
		for (x = 0; x < feldgrx + 4; x++) {

			feld[y][x] = 219;
		}
	}

}
/////////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTIONEN SCHIFFE BELEGEN
int schiffe_positionieren(ARRAY feldpositionierung, int y, int x) {

	int lu_boot = 2;
	int iu_boot = mengeboot(feldpositionierung, lu_boot);
	int lzerstörer = 3;
	int izerstörer = mengeboot(feldpositionierung, lzerstörer);
	int lkreuzer = 4;
	int ikreuzer = mengeboot(feldpositionierung, lkreuzer);
	int lschlachtschiff = 5;
	int ischlachtschiff = mengeboot(feldpositionierung, lschlachtschiff);
	int y1 = y;
	int x1 = x;
	char original;
	char _moveschiff;

	iu_boot = mengeboot(feldpositionierung, lu_boot);
	while (iu_boot < 8) {
		system("cls");
		original = feldpositionierung[y1][x1];
		feldpositionierung[y1][x1] = 'x';
		printf("U-Boote setzen (%d/4) 2 Felder lang", iu_boot / 2);
		printspielfeld_posi(feldpositionierung);
		if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
			return 0;
		}
		_moveschiff = _getch();
		iu_boot += schiffsposi(_moveschiff, &y1, &x1, feldpositionierung, original, iu_boot, lu_boot);
		if (iu_boot%lu_boot == 0) {
			iu_boot = mengeboot(feldpositionierung, lu_boot);
		}
		if (iu_boot > 8) {
			iu_boot = 8;
			system("pause");
		}
	}

	if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
		return 0;
	}



	izerstörer = mengeboot(feldpositionierung, lzerstörer);
	while (izerstörer < 9) {
		system("cls");
		original = feldpositionierung[y1][x1];
		feldpositionierung[y1][x1] = 'x';
		printf("Zerstörer setzen (%d/3) 3 Felder lang", izerstörer / 3);
		printspielfeld_posi(feldpositionierung);
		if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
			return 0;
		}
		_moveschiff = _getch();
		izerstörer += schiffsposi(_moveschiff, &y1, &x1, feldpositionierung, original, izerstörer, lzerstörer);
		if (izerstörer%lzerstörer == 0) {
			izerstörer = mengeboot(feldpositionierung, lzerstörer);
		}
		if (izerstörer > 9) {
			izerstörer = 9;
			system("pause");
		}
	}

	if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
		return 0;
	}



	ikreuzer = mengeboot(feldpositionierung, lkreuzer);
	while (ikreuzer < 8) {
		system("cls");
		original = feldpositionierung[y1][x1];
		feldpositionierung[y1][x1] = 'x';
		printf("Kreuzer setzen (%d/2) 4 Felder lang", ikreuzer / 4);
		printspielfeld_posi(feldpositionierung);
		_moveschiff = _getch();
		ikreuzer += schiffsposi(_moveschiff, &y1, &x1, feldpositionierung, original, ikreuzer, lkreuzer);
		if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
			return 0;
		}
		if (ikreuzer%lkreuzer == 0) {
			ikreuzer = mengeboot(feldpositionierung, lkreuzer);
		}
		if (ikreuzer > 8) {
			ikreuzer = 8;
			system("pause");
		}
	}

	if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
		return 0;
	}



	ischlachtschiff = mengeboot(feldpositionierung, lschlachtschiff);
	while (ischlachtschiff < 5) {
		system("cls");
		original = feldpositionierung[y1][x1];
		feldpositionierung[y1][x1] = 'x';
		printf("U-Boote setzen (%d/1) 5 Felder lang", ischlachtschiff / 5);
		printspielfeld_posi(feldpositionierung);
		if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
			return 0;
		}
		_moveschiff = _getch();
		ischlachtschiff += schiffsposi(_moveschiff, &y1, &x1, feldpositionierung, original, ischlachtschiff, lschlachtschiff);
		if (ischlachtschiff%lschlachtschiff == 0) {
			ischlachtschiff = mengeboot(feldpositionierung, lschlachtschiff);
		}
		if (ischlachtschiff > 5) {
			ischlachtschiff = 5;
		}
	}

	if ((mengeboot(feldpositionierung, lu_boot) + mengeboot(feldpositionierung, lzerstörer) + mengeboot(feldpositionierung, lkreuzer) + mengeboot(feldpositionierung, lschlachtschiff)) == 30) {
		return 0;
	}


}
int schiffsposi(char ch, int* y, int* x, ARRAY feldpositionierung, char orginal, int aktuell, int schiffslänge) {

	int yy = 0;
	int xx = 0;
	int yyy;
	int xxx;


	switch (ch) {
	case 'a':
	case 'A':
		*x = *x - 1;

		if (*x < 2) {
			*x = *x + feldgrx;
			feldpositionierung[*y][*x - feldgrx + 1] = orginal;
			return 0;

		}

		feldpositionierung[*y][*x + 1] = orginal;
		return 0;




	case 'd':
	case 'D':
		*x = *x + 1;
		if (*x > feldgrx + 1) {
			*x = *x - feldgrx;
			feldpositionierung[*y][*x + feldgrx - 1] = orginal;
			return 0;

		}

		feldpositionierung[*y][*x - 1] = orginal;
		return 0;



	case 'w':
	case 'W':
		*y = *y - 1;
		if (*y < 2) {
			*y = *y + feldgry;
			feldpositionierung[*y - feldgry + 1][*x] = orginal;
			return 0;

		}

		feldpositionierung[*y + 1][*x] = orginal;
		return 0;



	case 's':
	case 'S':
		*y = *y + 1;
		if (*y > feldgry + 1) {
			*y = *y - feldgry;
			feldpositionierung[*y + feldgry - 1][*x] = orginal;
			return 0;

		}

		feldpositionierung[*y - 1][*x] = orginal;
		return 0;


	case 'e':
	case 'E':
	case '\r':
		if (orginal == 'o') {
			feldpositionierung[*y][*x] = 'o';
			return 0;
		}
		else {
			if (aktuell%schiffslänge == 0) {
				if (positionsregelnanfang(feldpositionierung, &y, &x, schiffslänge)) {
					feldpositionierung[*y][*x] = 'o';
					return 1;
				}

				else {
					printf("\t\t    Position nicht erlaubt\n\n");
					feldpositionierung[*y][*x] = 219;
					system("pause");
					return 0;
				}
			}
			else {
				if (positionsregelndazwischen(feldpositionierung, &y, &x, schiffslänge)) {
					feldpositionierung[*y][*x] = 'o';
					return 1;
				}
				else {
					printf("\t\t    Position nicht erlaubt\n\n");
					feldpositionierung[*y][*x] = 219;
					system("pause");
					return 0;
				}

			}

		}


	case 'q':
	case 'Q':
		if (aktuell%schiffslänge == 1) {
			feldpositionierung[*y][*x] = orginal;
			for (yyy = 2; yyy < feldgry + 2; yyy++) {
				for (xxx = 2; xxx < feldgrx + 2; xxx++) {
					if (feldpositionierung[yyy][xxx] == 'o' && (feldpositionierung[yyy - 1][xxx] == -37 && feldpositionierung[yyy + 1][xxx] == -37 && feldpositionierung[yyy][xxx - 1] == -37 && feldpositionierung[yyy][xxx + 1] == -37)) {
						feldpositionierung[yyy][xxx] = -37;
					}
				}
			}
			printf("\t\t    EINZELSTÜCK WIRD GELÖSCHT\n\n");
			system("pause");
			return -1;
		}
		if (orginal == 'o') {
			yy = *y;
			xx = *x;
			löschen(feldpositionierung, &y, &x);
			schiffe_positionieren(feldpositionierung, yy, xx);
			break;
		}
		else {
			feldpositionierung[*y][*x] = -37;
			return 0;
		}


	default:
		feldpositionierung[*y][*x] = 219;
		return 0;
	}

}
void printspielfeld_posi(ARRAY feldpositionierung) {

	int y, x;

	printf("\n\n");
	for (y = 2; y < feldgry + 2; y++) {
		printf("\t\t    ");
		for (x = 2; x < feldgrx + 2; x++) {

			printf("%c", feldpositionierung[y][x]);

		}
		printf("\n");
	}

}
void befuellen_feldposi(ARRAY feldpositionierung) {

	int y, x;

	for (y = 0; y < feldgry + 4; y++) {
		for (x = 0; x < feldgrx + 4; x++) {

			feldpositionierung[y][x] = -37;
		}
	}
}
void löschen(ARRAY feldpositionierung, int**y, int**x) {

	int i = 1;
	int yy = **y;
	int xx = **x;
	feldpositionierung[yy][xx] = -37;

	while (feldpositionierung[yy - i][xx] == 'o') {
		feldpositionierung[yy - i][xx] = -37;
		i++;
		if (feldpositionierung[yy - i][xx] != 'o') {
			i = 1;
		}
	}


	while (feldpositionierung[yy + i][xx] == 'o') {
		feldpositionierung[yy + i][xx] = -37;
		i++;
		if (feldpositionierung[yy + i][xx] != 'o') {
			i = 1;
		}
	}

	while (feldpositionierung[yy][xx - i] == 'o') {
		feldpositionierung[yy][xx - i] = -37;
		i++;
		if (feldpositionierung[yy][xx - i] != 'o') {
			i = 1;
		}
	}

	while (feldpositionierung[yy][xx + i] == 'o') {
		feldpositionierung[yy][xx + i] = -37;
		i++;
		if (feldpositionierung[yy][xx + i] != 'o') {
			i = 1;
		}
	}


}
int mengeboot(ARRAY feldpositionierung, int lboot) {

	int y, x;
	int i = 0;
	int yy = 1;
	int xx = 1;
	int boot = 0;

	ARRAY zwischenfeld;
	for (y = 0; y < feldgry + 4; y++) {
		for (x = 0; x < feldgrx + 4; x++) {
			zwischenfeld[y][x] = feldpositionierung[y][x];
		}
	}


	for (y = 0; y < feldgry + 4; y++) {
		for (x = 0; x < feldgrx + 4; x++) {
			if (zwischenfeld[y][x] == 'o') {
				zwischenfeld[y][x] = -37;
				i++;
				while (zwischenfeld[y + yy][x] == 'o') {
					zwischenfeld[y + yy][x] = -37;
					i++;
					yy++;

				}

				while (zwischenfeld[y][x + xx] == 'o') {
					zwischenfeld[y][x + xx] = -37;
					i++;
					xx++;
				}

				if (i == lboot) {
					boot++;
				}

				i = 0;
				yy = 1;
				xx = 1;
			}
		}
	}

	return boot*lboot;
}
/////////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION ABGLEICHEN
void ende(int win, NAME name1, NAME name2) {
	if (win == 1) {
		system("cls");
		printf("\t\t%s gewinnt das Spiel\n\n", name1);
		sound_win();
	}
	else {
		system("cls");
		printf("\t\t%s gewinnt das Spiel\n\n", name2);
		sound_win();
	}

	main();

}
int zählung(ARRAY feld) {
	int y, x, i;
	i = 0;

	for (y = 0; y < feldgry + 4; y++) {
		for (x = 0; x < feldgrx + 4; x++) {
			if (feld[y][x] == 't') {
				i++;
			}
		}
	}
	return i;
}
int versenkt(int** y, int** x, ARRAY feld, ARRAY feldpositionierung) {

	int xx = **x;
	int yy = **y;
	int zähler = 1;
	int y1, x1;

	ARRAY feldkombi;
	for (y1 = 0; y1 < feldgry + 4; y1++) {
		for (x1 = 0; x1 < feldgrx + 4; x1++) {
			feldkombi[y1][x1] = feldpositionierung[y1][x1];
		}
	}

	for (y1 = 0; y1 < feldgry + 4; y1++) {

		for (x1 = 0; x1 < feldgrx + 4; x1++) {
			if (feld[y1][x1] == 't' || feld[y1][x1] == ' ') {
				feldkombi[y1][x1] = feld[y1][x1];
			}
		}
	}


	if (feldkombi[yy - zähler][xx] == 't') {
		while (1) {
			if (feldkombi[yy][xx] == 't') {
				yy--;
			}
			else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
				yy++;
				if (feldkombi[yy + zähler][xx] == 't') {
					while (1) {
						if (feldkombi[yy][xx] == 't') {
							yy++;
						}

						else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
							return 1;
						}
						else if (feldkombi[yy][xx] == 'o') {
							return 0;
						}
					}
				}
			}

			else if (feldkombi[yy][xx] == 'o') {
				return 0;
			}

		}

	}

	if (feldkombi[yy + zähler][xx] == 't') {
		while (1) {
			if (feldkombi[yy][xx] == 't') {
				yy++;
			}

			else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
				yy--;
				if (feldkombi[yy - zähler][xx] == 't') {
					while (1) {
						if (feldkombi[yy][xx] == 't') {
							yy--;
						}

						else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
							return 1;
						}
						else if (feldkombi[yy][xx] == 'o') {
							return 0;
						}
					}
				}
			}

			else if (feldkombi[yy][xx] == 'o') {
				return 0;
			}

		}

	}

	if (feldkombi[yy][xx - zähler] == 't') {
		while (1) {
			if (feldkombi[yy][xx] == 't') {
				xx--;
			}
			else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
				xx++;
				if (feldkombi[yy][xx + zähler] == 't') {
					while (1) {
						if (feldkombi[yy][xx] == 't') {
							xx++;
						}
						else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
							return 1;
						}
						else if (feldkombi[yy][xx] == 'o') {
							return 0;
						}
					}
				}
			}
			else if (feldkombi[yy][xx] == 'o') {
				return 0;
			}

		}

	}

	if (feldkombi[yy][xx + zähler] == 't') {
		while (1) {
			if (feldkombi[yy][xx] == 't') {
				xx++;
			}
			else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
				xx--;
				if (feldkombi[yy][xx - zähler] == 't') {
					while (1) {
						if (feldkombi[yy][xx] == 't') {
							xx--;
						}
						else if (feldkombi[yy][xx] == -37 || feldkombi[yy][xx] == ' ') {
							return 1;
						}
						else if (feldkombi[yy][xx] == 'o') {
							return 0;
						}
					}
				}
			}
			else if (feldkombi[yy][xx] == 'o') {
				return 0;
			}

		}

	}
	else {
		return 0;
	}
}
int positionsregelnanfang(ARRAY feldpositionierung, int **y, int **x, int länge) {

	int yy = **y;
	int xx = **x;

	if (feldpositionierung[yy - 1][xx] == -37 && feldpositionierung[yy + 1][xx] == -37 && feldpositionierung[yy][xx - 1] == -37 && feldpositionierung[yy][xx + 1] == -37 &&
		feldpositionierung[yy + 1][xx + 1] == -37 && feldpositionierung[yy - 1][xx + 1] == -37 && feldpositionierung[yy + 1][xx - 1] == -37 && feldpositionierung[yy - 1][xx - 1] == -37) {
		return 1;
	}

	else {
		return 0;
	}
}
int positionsregelndazwischen(ARRAY feldpositionierung, int **y, int **x, int länge) {

	int yy = **y;
	int xx = **x;
	int i = 0;


	if (feldpositionierung[yy - 1][xx] == 'o')
		i++;

	if (feldpositionierung[yy - 1][xx - 1] == 'o')
		i += 2;

	if (feldpositionierung[yy + 1][xx] == 'o')
		i++;

	if (feldpositionierung[yy - 1][xx + 1] == 'o')
		i += 2;

	if (feldpositionierung[yy][xx - 1] == 'o')
		i++;

	if (feldpositionierung[yy + 1][xx + 1] == 'o')
		i += 2;

	if (feldpositionierung[yy][xx + 1] == 'o')
		i++;

	if (feldpositionierung[yy + 1][xx - 1] == 'o')
		i += 2;


	if (i != 1) {
		return 0;
	}

	else {
		return 1;
	}

}

////////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION SOUNDS
void sound_hit() {
	Beep(700, 400);
	Beep(700, 400);
	Beep(1100, 600);
}
void sound_miss() {
	Beep(600, 400);
	Beep(500, 700);
}
void sound_versenkt() {
	Beep(1500, 200);
	Beep(1000, 400);
	Beep(2000, 350);
}
void sound_win() {
	Beep(2500, 600);
	Beep(2600, 700);
	Beep(2700, 800);
	Beep(2000, 1000);
}
///////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION SPIEL/ER
int spieler1(ARRAY feld, ARRAY feldpositionierung, NAME name1) {

	int i = 0;
	system("cls");
	printf("%s ist am Zug\n", name1);
	Sleep(1000);
	system("cls");
	i = bewegung_schuss(feld, feldpositionierung);
	system("cls");
	printspielfeld(feld);
	Sleep(500);
	system("cls");
	if (i == 0) {
		return 0;
	}
	return 1;

}
int spieler2(ARRAY feld, ARRAY feldpositionierung, NAME name2) {

	int i = 0;
	system("cls");
	printf("%s ist am Zug\n", name2);
	system("pause");
	system("cls");
	i = bewegung_schuss(feld, feldpositionierung);
	system("cls");
	printspielfeld(feld);
	Sleep(1000);
	system("cls");
	if (i == 0) {
		return 0;
	}
	return 1;
}
int spiel(NAME name1, NAME name2, ARRAY feld1, ARRAY feldpositionierung1, ARRAY feld2, ARRAY feldpositionierung2) {

	int win = 0;
	int x = 2;
	int y = 2;

	fflush(stdin);
	name_spielerzuweißung(name1, name2);
	befuellen(feld1);
	befuellen_feldposi(feldpositionierung1);
	befuellen(feld2);
	befuellen_feldposi(feldpositionierung2);


	printf("%s, Positioniere nun deine Schiffe\n", name1);
	system("pause");
	schiffe_positionieren(feldpositionierung1, y, x);
	system("cls");
	printf("%s, Positioniere nun deine Schiffe\n", name2);
	system("pause");
	schiffe_positionieren(feldpositionierung2, y, x);

	while (1) {

		if (spieler1(feld1, feldpositionierung2, name1) == 0)
			return  1;


		if (spieler2(feld2, feldpositionierung1, name2) == 0)
			return 2;

	}

}
//////////////////////////////////////////////////////////////////////////////

//UNTERFUNKTION OPTISCHES
void name_spielerzuweißung(NAME p1, NAME p2) {

	printf("Spieler 1 bitte Namen eingeben:\n");
	gets(p1);
	system("cls");

	printf("Spieler 2 bitte Namen eingeben:\n");
	gets(p2);
	system("cls");

}
void start() {

	int option = 0;

	printf("\n\t\tSCHIFFE VERSENKEN\n");
	printf("\n\t\t1.SPIELEN");
	printf("\n\t\t2.TASTENBELEGUNG");
	printf("\n\t\t3.EXIT\n\n");
	printf("\t\tAuswahl: ");
	scanf("%d", &option);

	switch (option) {

	case 1:
		system("cls");
		break;


	case 2:
		system("cls");
		printf("\n\n\t\tw = nach oben\n");
		printf("\t\ts = nach unten\n");
		printf("\t\ta = nach links\n");
		printf("\t\td = nach rechts\n");
		printf("\t\te = Schuss/Schiffteil positionieren\n");
		printf("\t\tq = Schiffteil loeschen\n\n\n");
		system("pause");
		system("cls");
		main();

	case 3:
		exit(0);

	default:
		system("cls");
		main();

	}

}
//////////////////////////////////////////////////////////////////////////////