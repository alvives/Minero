#include "mina.h"
#include <iostream>
#include <Windows.h>
#include <iomanip>      // std::setw
using namespace std;

//CABECERAS PRIVADOS:
void caracter_color(tCasilla casilla, char& caracter, int& numColor);
void colorFondo(int color);
void genera3x3(tCasilla casilla, tPlanoCaracteres plano_minaCaracteres, tPlanoColores plano_minaColores, int i, int j);
tCasilla caracterACasilla(char c);



//SUBPROGRAMAS PÚBLICOS:
void cargar_Mina(ifstream& fichero, tMina& mina){
	char carac;

	fichero >> mina.plano_mina.nfilas >> mina.plano_mina.ncolumnas;
	fichero.get(carac);
	for (int f = 0; f <mina.plano_mina.nfilas; f++) {
		for (int c = 0; c < mina.plano_mina.ncolumnas; c++) {
			fichero.get(carac);
			mina.plano_mina.cuadricula[f][c] = caracterACasilla(carac);
			//Para saber donde empieza el minero:
			if (carac == 'J') {
				mina.fila = f;
				mina.columna = c;
			}
		}
		fichero.get(carac);
	}
};
void dibujar1_1(const tPlano& plano_mina) {
	char carac;
	int color;

	for (int f = 0; f < plano_mina.nfilas; f++) {
		for (int c = 0; c < plano_mina.ncolumnas; c++) {
			caracter_color(plano_mina.cuadricula[f][c], carac, color);
			colorFondo(color);
			cout << carac;
			colorFondo(0);
		}
		cout << endl;
	}
}
void dibujar1_3(const tPlano& plano_mina) {
	tPlanoCaracteres caracteres;
	tPlanoColores colores;

	//generar las matrices auxiliares caracteres y colores
	for (int f = 0; f < plano_mina.nfilas; f++) {
		for (int c = 0; c < plano_mina.ncolumnas; c++) {
			genera3x3(plano_mina.cuadricula[f][c], caracteres, colores, f, c);
		}
	}

	//Pintamos
	for (int i = 0; i < plano_mina.nfilas * 3; i++) {
		for (int j = 0; j < plano_mina.ncolumnas * 3; j++) {
			colorFondo(colores[i][j]);
			cout << " " << (caracteres[i][j]);
			colorFondo(0);
		}
		cout << endl;
	}
}



//SUBPROGRAMAS PRIVADOS:
void caracter_color(tCasilla casilla, char& caracter, int& numColor) {
	switch (casilla) {
	case LIBRE:  caracter = ' '; numColor = 4; break;
	case PIEDRA: caracter = '@'; numColor = 4; break;
	case TIERRA:  caracter = '.'; numColor = 4; break;
	case MURO: caracter = 'X'; numColor = 4; break;
	case MINERO: caracter = 'M'; numColor = 7; break;
	case DINAMITA: caracter = 'D'; numColor = 9; break;
	case GEMA: caracter = 'G'; numColor = 2; break;
	case SALIDA: caracter = 'S'; numColor = 1; break;
	}
}
void colorFondo(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}
void genera3x3(tCasilla casilla, tPlanoCaracteres plano_minaCaracteres, tPlanoColores plano_minaColores, int i, int j){
	char carac;
	int color;

	
	for (int f = i * 3; f < i * 3 + 3; f++) {
		for (int c = j * 3; c < j * 3 + 3; c++) {
			caracter_color(casilla, carac, color);
			plano_minaCaracteres[f][c] = carac;
			plano_minaColores[f][c] = color;
		}
	}
}
tCasilla caracterACasilla(char c) {
	tCasilla casilla;

	switch (c) {
	case ' ': casilla = LIBRE; break;
	case 'T': casilla = TIERRA; break;
	case 'G': casilla = GEMA; break;
	case 'P': casilla = PIEDRA; break;
	case 'M': casilla = MURO; break;
	case 'S': casilla = SALIDA; break;
	case 'J': casilla = MINERO; break;
	}
	return casilla;
}

//1) Devuelve el carácter y el color con los que pintar la casilla dada
//2) Pone el color de fondo al indicado
//3) Convierte una casilla en una 3x3
//4) Devuelve el valor del tipo tCasilla correspondiente al carácter c 