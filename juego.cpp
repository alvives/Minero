#include "juego.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

//CABECERAS SUBPROGRAMAS PRIVADOS:
void hacerMovimiento(tJuego& juego, tTecla t);
void dibujar(const tJuego& juego);
tTecla leerTecla();
void lanzarDinamita(tJuego& juego);
void explotar(tJuego& juego, int px, int py);
void caidaLibre(tJuego& juego, int px, int py, int& nx, int& ny);
void caida_piedrasYgemas(tJuego& juego, int f, int c);
//inventados por mí:
bool posicion_destino_valida(const tMina& mina, int fila, int columna, tTecla t);
tCasilla objeto_casilla(const tMina& mina, int fila, int columna, tTecla t);		
bool puedo_mover_piedra(const tMina& mina, int fila, int columna, tTecla t);
void moverse_posicion_destino(tMina& mina, tTecla t);		
int fila_piedra(int fila_minero, tTecla t);
int col_piedra(int col_minero, tTecla t);
tTecla transformar_char_tecla(char& c);



//SUBPROGRAMAS PÚBLICOS:
void inicializar_Juego(tJuego& juego, const int nivel) {
	///inicializo los contadores de gemas, movimientos y dinamitas a 0:
	juego.total_gemas_recogidas = 0;
	juego.num_movimientos = 0;
	juego.num_dinamitas_usadas = 0;
	//selecciono el nivel indicado:
	juego.nivel = nivel;
	//inicializo estado del juego:
	juego.estado = JUGANDO;
}
bool cargar_Juego(tJuego& juego){
	ifstream archivo;
	string nivel;
	bool carga = false;
	int num;           //Para pruebas

	nivel = juego.nivel + '0';
	nivel = nivel + '.' + 't' + 'x' + 't';

	/*Para poder hacer pruebas (BORRAR ANTES DE ENTREGAR):
	do {
		cout << "Deseas cargar una prueba?" << endl;
		cout << "    1. Si       2. NO    " << endl;
		cin >> num;

		if (num == 1) {
			cout << "Introduce nombre del archivo: ";
			cin >> nivel;		// Voy a suponer que introduzco el nombre bien
		}
		else if (num != 2) {
			cout << "Dato no valido" << endl;
		}
	} while (num != 1 && num != 2);
	*/

	archivo.open(nivel);

	if (archivo.is_open()) {
		carga = true;
		cargar_Mina(archivo, juego.mina);
	}

	archivo.close();

	return carga;
}
void jugar_desde_teclado(tJuego& juego) {
	tTecla t;

	juego.estado = JUGANDO;
	do {
		t = leerTecla();
		if (t == Arriba || t == Abajo || t == Derecha || t == Izquierda) {
			juego.num_movimientos++;
			hacerMovimiento(juego, t);
		}
		if (t == TNT) {
			juego.num_movimientos++;
			juego.num_dinamitas_usadas++;
			hacerMovimiento(juego, t);
		}
		if (t == Salir) {
			juego.estado = PERDIDO;
		}

		system("cls");
		dibujar(juego);

	} while (juego.estado == JUGANDO);


}
void jugar_desde_fichero(tJuego& juego) {
	ifstream archivo;
	string nombre;
	char c;
	tTecla t;

	cout << "Introduzca el nombre del fichero: ";
	cin >> nombre;

	archivo.open(nombre + ".txt");

	if (!archivo.is_open()) {
		juego.estado = ABANDONO;
		cout << endl << "EL fichero no se ha encontrado" << endl;
	}

	else {
		juego.estado = JUGANDO;
		archivo >> c;
		while (juego.estado == JUGANDO && c != '.') {
			t = transformar_char_tecla(c);
			if (t == Arriba || t == Abajo || t == Izquierda || t == Derecha) {
				juego.num_movimientos++;
				hacerMovimiento(juego, t);
			}
			if (t == TNT) {
				juego.num_dinamitas_usadas++;
				juego.num_movimientos++;
				hacerMovimiento(juego, t);
			}
			if (t == Salir) {
				juego.estado = ABANDONO;
			}
			dibujar(juego);
			Sleep(1000);
			archivo >> c;
		}
	}
}



//SUBPROGRAMAS PRIVADOS:
void hacerMovimiento(tJuego& juego, tTecla t) {
	bool se_ha_movido = false;
	int fil = juego.mina.fila, col = juego.mina.columna;
	//Me parece que es privada porque solo la uso para implementar el jugar_desde_teclado
	if (t == TNT) {
		lanzarDinamita(juego);
	}
	if (t == Arriba || t == Abajo || t == Derecha || t == Izquierda) {
		//Si la posicion destino es valida, se mueve:		
		if (posicion_destino_valida(juego.mina, juego.mina.fila, juego.mina.columna, t)) {
			switch (objeto_casilla(juego.mina, juego.mina.fila, juego.mina.columna, t)) {
			case MURO:
				break;
			case GEMA:
				moverse_posicion_destino(juego.mina, t);
				se_ha_movido = true;
				juego.total_gemas_recogidas++;
				break;
			case TIERRA:
			case LIBRE:
				moverse_posicion_destino(juego.mina, t);
				se_ha_movido = true;
				break;
			case SALIDA:
				moverse_posicion_destino(juego.mina, t);
				se_ha_movido = true;
				juego.estado = GANADO;
				break;
			case PIEDRA:
				if (puedo_mover_piedra(juego.mina, fila_piedra(juego.mina.fila, t), col_piedra(juego.mina.columna, t), t)) {
					moverse_posicion_destino(juego.mina, t);
					se_ha_movido = true;
					juego.mina.plano_mina.cuadricula[fila_piedra(juego.mina.fila, t)][col_piedra(juego.mina.columna, t)] = PIEDRA;
					//si atrás hay otra piedra tampoco me deja moverla					
					caida_piedrasYgemas(juego, fila_piedra(juego.mina.fila, t), col_piedra(juego.mina.columna, t));
				}
				//El se_ha_movido ya lo he inicializado a false
				break;
			}
			

			if (se_ha_movido && (t != Arriba) && (juego.mina.plano_mina.cuadricula[fil - 1][col] == PIEDRA || juego.mina.plano_mina.cuadricula[fil - 1][col] == GEMA)) {
				caida_piedrasYgemas(juego, fil - 1, col);
			}
			
		}
	}
}
void dibujar(const tJuego& juego) {
	cout << endl << endl;
	if (juego.escala == 1) {
		dibujar1_1(juego.mina.plano_mina);
	}
	else {
		dibujar1_3(juego.mina.plano_mina);
	}

	cout << endl << endl;
	cout << "Gemas totales recogidas: " << juego.total_gemas_recogidas << endl;
	cout << "Numero de movimientos: " << juego.num_movimientos << endl;
	cout << "Dinamitas usadas: " << juego.num_dinamitas_usadas << endl;
}
tTecla leerTecla() {
	tTecla t;

	cin.sync();
	int dir = _getch();
	if (dir == 0xe0) {
		dir = _getch();
		switch (dir) {
		case 72: t = Arriba; break;
		case 80: t = Abajo; break;
		case 77: t = Derecha; break;
		case 75: t = Izquierda; break;
		}
	}
	else if (dir == 27) { t = Salir; }
	else if (dir == 68 || dir == 100) { t = TNT; }
	else { t = Nada; }

	return t;
}
void lanzarDinamita(tJuego& juego) {
	int px, py, nx, ny;

	px = juego.mina.fila;
	py = juego.mina.columna;

	juego.mina.plano_mina.cuadricula[juego.mina.fila][juego.mina.columna] = DINAMITA;
	caidaLibre(juego, px, py, nx, ny);			//Da igual que el minero desaparezca mientras cae la dinamita
	juego.mina.plano_mina.cuadricula[juego.mina.fila][juego.mina.columna] = MINERO;
	explotar(juego, nx, ny);					//Da igual que la salida tb desaparezca
}
void explotar(tJuego& juego, int px, int py) {
	int nx, ny;

	for (int f = px - 1; f < px + 2; f++) {
		for (int c = py - 1; c < py + 2; c++) {
			if ((f >= 0 && f <= juego.mina.plano_mina.nfilas) && (c >= 0 && c <= juego.mina.plano_mina.ncolumnas)) {
				if (juego.mina.plano_mina.cuadricula[f][c] == MINERO) {
					juego.estado = PERDIDO;
				}
				juego.mina.plano_mina.cuadricula[f][c] = LIBRE;
			}
		}
	}

	dibujar(juego);

	for (int c = py - 1; c < py + 2; c++) {
		caida_piedrasYgemas(juego, px - 2, c);
	}
	
}
void caidaLibre(tJuego& juego, int px, int py, int& nx, int& ny) {
	//px-py es la posición del objeto que va a caer
	while (posicion_destino_valida(juego.mina, px, py, Abajo) && juego.mina.plano_mina.cuadricula[px + 1][py] == LIBRE) {
		juego.mina.plano_mina.cuadricula[px + 1][py] = juego.mina.plano_mina.cuadricula[px][py];
		juego.mina.plano_mina.cuadricula[px][py] = LIBRE;
		px++;
		dibujar(juego);
	}
	nx = px;
	ny = py;

}
void caida_piedrasYgemas(tJuego& juego, int f, int c) {
	int nx, ny;
	
	while (juego.mina.plano_mina.cuadricula[f][c] == PIEDRA || juego.mina.plano_mina.cuadricula[f][c] == GEMA) {
		//duda: ¿hay alguna forma de quitar nx y ny?
		caidaLibre(juego, f, c, nx, ny);
		f--;
	}
}

//SUB. PRIVADOS INVENTADAS POR MÍ PARA FACILITARME LA TAREA:
bool posicion_destino_valida(const tMina& mina, int fila, int columna, tTecla t) {
	bool puede = true;

	if (columna == 0 && t == Izquierda) {
		puede = false;
	}
	if (columna == mina.plano_mina.ncolumnas - 1 && t == Derecha) {
		puede = false;
	}
	if (fila == 0 && t == Arriba) {
		puede = false;
	}
	if (fila == mina.plano_mina.nfilas - 1 && t == Abajo) {
		puede = false;
	}

	return puede;
}
tCasilla objeto_casilla(const tMina& mina, int fila, int columna, tTecla t) {
	tCasilla casilla;
	//fila y columna es la posicion inicial

	switch (t) {
	case (Arriba):
		casilla = mina.plano_mina.cuadricula[fila - 1][columna];
		break;
	case (Derecha):
		casilla = mina.plano_mina.cuadricula[fila][columna + 1];
		break;
	case (Abajo):
		casilla = mina.plano_mina.cuadricula[fila + 1][columna];
		break;
	case (Izquierda):
		casilla = mina.plano_mina.cuadricula[fila][columna - 1];
		break;
	}

	return casilla;
}
bool puedo_mover_piedra(const tMina& mina, int fila, int columna, tTecla t) {
	bool puedo = false;
	//fila y columna es la posicion de la piedra q estoy intentando mover

	if (posicion_destino_valida(mina, fila, columna, t) && (objeto_casilla(mina, fila, columna, t) == LIBRE)) {
		puedo = true;
	}

	
	return puedo;
}
void moverse_posicion_destino(tMina& mina, tTecla t) {
	if (t == Arriba) {
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = LIBRE;
		mina.fila--;
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = MINERO;
	}
	else if (t == Abajo) {
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = LIBRE;
		mina.fila++;
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = MINERO;
	}
	else if (t == Derecha) {
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = LIBRE;
		mina.columna++;
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = MINERO;
	}
	else {			//No creo que pueda llegarme aquí un dato erróneo
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = LIBRE;
		mina.columna--;		
		mina.plano_mina.cuadricula[mina.fila][mina.columna] = MINERO;
	}
}
int fila_piedra(int fila_minero, tTecla t) {
	int piedra = fila_minero;

	if (t == Arriba || t == Abajo) {
		if (t == Arriba) {
			piedra--;
		}
		else {
		piedra++;
		}
	}

	return piedra;
}
int col_piedra(int col_minero, tTecla t) {
	int piedra = col_minero;

	if (t == Izquierda || t == Derecha) {
		if (t == Izquierda) {
			piedra--;
		}
		else {
			piedra++;
		}
	}

	return piedra;
}
tTecla transformar_char_tecla(char& c) {
	tTecla t;

	switch (c) {
	case 'D':
		t = TNT;
		break;
	case 'M':
		t = Derecha;
		break;
	case 'N':
		t = Izquierda;
		break;
	case 'A':
		t = Arriba;
		break;
	case 'Z':
		t = Abajo;
		break;
	case 'X':
		t = Salir;
		break;
	}


	return t;
}


//Para no salirme del plano
//Para saber qué hay en la casilla destino
//Para moverme hacia donde quiero	
//Para saber la posicion de la piedra
//Para saber si dejo caer objeto y la posición del mismo