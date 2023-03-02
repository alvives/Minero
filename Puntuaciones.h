#ifndef puntuaciones_h
#define puntuaciones_h
//////////////////////
#include <iostream>
using namespace std;

//CTES GLOBALES:
const int A = 10;
const int B = 2;
const int NUM_TOTAL_MINAS = 5;
const int TM_INICIAL = 2;

//TIPOS:
typedef struct {
	int IdMina;
	int numMovimientos;
	int numGemas;
	int numDinamitas;
	int puntosMina;
} tDatosMina;

typedef struct {
	string nombre;
	int punt_total;
	int minasRecorridas;
	tDatosMina vMinasRecorridas[NUM_TOTAL_MINAS];
} tPuntuacionJugador;

typedef struct {
	int capacidad;
	int num_jugadores;
	tPuntuacionJugador *array_clasificacion;
}tPuntuaciones;

//FUNCIONES:
bool cargar_Marcador(tPuntuaciones& marcador);
void mostrar_Datos_Usuarios(const tPuntuaciones& marcador);
void mostrar_Minas_Usuario(const tPuntuaciones& marcador, int pos);
void destruir(tPuntuaciones& marcador);
bool buscar(const string& nombre, const tPuntuaciones& marcador, int& pos);
void insertar(tPuntuaciones& marcador, const string& nombre, int pos);
void mostrar_Alfabetico(const tPuntuaciones& marcador);
bool guardar_Marcador(const tPuntuaciones& marcador);
//inventada por nosotros:
void inicializar_Jugador(tPuntuacionJugador& jugador);


//////
#endif
