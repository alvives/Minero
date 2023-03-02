#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Puntuaciones.h"
using namespace std;

//CABECERA PRIVADOS:
void inicializar_Marcador(tPuntuaciones& marcador);
void aumentar_Capacidad(tPuntuaciones& marcador);
//inventados por nosotros:
void crear_Jugador(tPuntuacionJugador& jugador);
int buscar_Posicion(const string& nombre, tPuntuaciones& marcador);


//FUNCIONES PÚBLICAS:
bool cargar_Marcador(tPuntuaciones& marcador) {
	bool carga = false;
	ifstream input;
	//Datos persona:
	tPuntuacionJugador jugador;
	//Auxiliares:
	int aux;


	inicializar_Marcador(marcador);

	input.open("Puntuaciones.txt");

	if (input.is_open()) {
		carga = true;

		input >> jugador.nombre;
		while (jugador.nombre != "000") {
			input >> jugador.punt_total;
			input >> jugador.minasRecorridas;
			crear_Jugador(jugador);	//Pongo a 0 todo 
			for (int i = 0; i < jugador.minasRecorridas; i++) {
				input >> aux;
				jugador.vMinasRecorridas[aux - 1].IdMina = aux;
				input >> jugador.vMinasRecorridas[aux - 1].numMovimientos;
				input >> jugador.vMinasRecorridas[aux - 1].numGemas;
				input >> jugador.vMinasRecorridas[aux - 1].numDinamitas;
				input >> jugador.vMinasRecorridas[aux - 1].puntosMina;
			}

			//buscamos el hueco:
			aux = buscar_Posicion(jugador.nombre, marcador);
			insertar(marcador, jugador.nombre, aux);
			marcador.array_clasificacion[aux] = jugador;

			input >> jugador.nombre;
		} 
	}

	return carga;
}
void insertar(tPuntuaciones& marcador, const string& nombre, int pos) {
	//Abrimos hueco:
	for (int i = marcador.num_jugadores; i > pos; i--) {
		marcador.array_clasificacion[i] = marcador.array_clasificacion[i - 1];
	}
	//Insertamos: 
	marcador.array_clasificacion[pos].nombre = nombre;
	marcador.num_jugadores++;
	// Si hace falta, redimensionamos:
	if (marcador.num_jugadores == marcador.capacidad) {
		aumentar_Capacidad(marcador);
	}
}
void mostrar_Datos_Usuarios(const tPuntuaciones& marcador) {
	cout << setw(43) << "----    JUGADORES    ----" << endl << endl;
	for (int i = 0; i < marcador.num_jugadores; i++) {		
		mostrar_Minas_Usuario(marcador, i);
	}
}
void mostrar_Minas_Usuario(const tPuntuaciones& marcador, int pos) {
	int aux = 0;

	cout << setw(10) << marcador.array_clasificacion[pos].nombre;
	cout << setw(15) << "Movimientos" << setw(10) << "Gemas";
	cout << setw(10) << "Dinamitas" << setw(10) << "Puntos";
	cout << setw(10) << "Puntos en total" << endl;

	for (int j = 0; j < NUM_TOTAL_MINAS; j++) {
		if (marcador.array_clasificacion[pos].vMinasRecorridas[j].numMovimientos != 0) {
			cout << "  Mina  " << j + 1;
			cout << setw(10) << marcador.array_clasificacion[pos].vMinasRecorridas[j].numMovimientos;
			cout << setw(15) << marcador.array_clasificacion[pos].vMinasRecorridas[j].numGemas;
			cout << setw(10) << marcador.array_clasificacion[pos].vMinasRecorridas[j].numDinamitas;
			if (aux == 0) {
				cout << setw(20) << marcador.array_clasificacion[pos].punt_total;
				aux = 1;
			}
			cout << endl;
		}
	}
	cout << endl;

}
void destruir(tPuntuaciones& marcador) {
	delete[] marcador.array_clasificacion;
	marcador.num_jugadores = 0;
	marcador.capacidad = 0;
	marcador.array_clasificacion = NULL;
}
bool buscar(const string& nombre, const tPuntuaciones& marcador, int& pos) {
	int  ini, fin;
	bool encontrado;

	encontrado = false;
	ini = 0;
	fin = marcador.num_jugadores - 1;
	while (!encontrado && ini <= fin) {
		pos = (ini + fin) / 2;
		if (nombre == marcador.array_clasificacion[pos].nombre) {
			encontrado = true;
		}
		else if (nombre < marcador.array_clasificacion[pos].nombre) {
			fin = pos - 1;
		}
		else {
			ini = pos + 1;
		}

	}

	return encontrado;
}
void mostrar_Alfabetico(const tPuntuaciones& marcador) {
	cout << "--------------" << setw(30) << "LISTA DE JUGADORES" << setw(30) << "--------------" << endl << endl;
	
	for (int i = 0; i < marcador.num_jugadores; i++) {
		cout << "              " << setw(20) << marcador.array_clasificacion[i].nombre <<" " << marcador.array_clasificacion[i].punt_total << endl;
	}
}
bool guardar_Marcador(const tPuntuaciones& marcador) {
	bool guardado = false;
	ofstream output;

	output.open("Puntuaciones2.txt");

	if (output.is_open()) {
		guardado = true;
	
		for (int i = 0; i < marcador.num_jugadores; i++) {
			output << marcador.array_clasificacion[i].nombre << endl;
			output << marcador.array_clasificacion[i].punt_total << endl;
			for (int j = 0; j < NUM_TOTAL_MINAS; j++) {
				if (marcador.array_clasificacion[i].vMinasRecorridas[j].numMovimientos != 0) {
					output << marcador.array_clasificacion[i].vMinasRecorridas[j].IdMina << " ";
					output << marcador.array_clasificacion[i].vMinasRecorridas[j].numMovimientos << " ";
					output << marcador.array_clasificacion[i].vMinasRecorridas[j].numGemas << " ";
					output << marcador.array_clasificacion[i].vMinasRecorridas[j].numDinamitas << " ";
					output << marcador.array_clasificacion[i].vMinasRecorridas[j].puntosMina << endl;
				}
			}

		}
		output << "000";
	}

	return guardado;
}
//inventada por nosotros (crea jugador y tambien pone los puntos a 0):
void inicializar_Jugador(tPuntuacionJugador& jugador) {
	crear_Jugador(jugador);	
	jugador.punt_total = 0;
}


//SUBPROGRAMAS PRIVADOS:
void inicializar_Marcador(tPuntuaciones& marcador) {
	marcador.num_jugadores = 0;
	marcador.capacidad = TM_INICIAL;
	marcador.array_clasificacion = new tPuntuacionJugador[marcador.capacidad];
}
void aumentar_Capacidad(tPuntuaciones& marcador) {
	tPuntuacionJugador* p = new tPuntuacionJugador[marcador.capacidad + TM_INICIAL];

	for (int i = 0; i < marcador.num_jugadores; i++) {
		p[i] = marcador.array_clasificacion[i];
	}
	delete[] marcador.array_clasificacion;
	marcador.array_clasificacion = p;
	marcador.capacidad = marcador.capacidad + TM_INICIAL;
}
//Inventados por nosotros:
void crear_Jugador(tPuntuacionJugador& jugador) {
	//Pongo a 0 las componentes no rellenas:
	for (int i = 0; i < NUM_TOTAL_MINAS; i++) {
		jugador.vMinasRecorridas[i].IdMina = i + 1;
		jugador.vMinasRecorridas[i].numDinamitas = 0;
		jugador.vMinasRecorridas[i].numGemas = 0;
		jugador.vMinasRecorridas[i].numMovimientos = 0;
		jugador.vMinasRecorridas[i].puntosMina = 0;
	}
}
int buscar_Posicion(const string& nombre, tPuntuaciones& marcador) {
	int pos = 0;
	bool va_despues = true;

	while (va_despues && pos < marcador.num_jugadores) {
		if (nombre < marcador.array_clasificacion[pos].nombre) {
			va_despues = false;
		}
		else {
			pos++;
		}
	}
	
	return pos;
}