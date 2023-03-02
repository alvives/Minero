#ifndef juego_h
#define juego_h
///////////////
#include "mina.h"

//TIPOS:
typedef enum tTecla { Arriba, Abajo, Izquierda, Derecha, Salir, Nada, TNT };
typedef enum tEstado { GANADO, PERDIDO, ABANDONO, JUGANDO };
typedef struct{
	tMina mina;
	int total_gemas_recogidas, num_movimientos, num_dinamitas_usadas, escala, nivel;
	tEstado estado;
}tJuego;


//FUNCIONES:
void inicializar_Juego(tJuego& juego, const int nivel);
bool cargar_Juego(tJuego& juego);
void jugar_desde_teclado(tJuego& juego);
void jugar_desde_fichero(tJuego& juego);

///////
#endif