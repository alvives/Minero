#ifndef mina_h
#define mina_h
//////////////
#include <fstream>
using namespace std;

//CTE GLOBAL:
const int MAX = 20;

//TIPOS:
typedef enum tCasilla { LIBRE, TIERRA, GEMA, PIEDRA, MURO, SALIDA, MINERO, DINAMITA };
typedef tCasilla tCuadricula[MAX][MAX];
typedef struct {
	tCuadricula cuadricula;
	int nfilas;
	int ncolumnas;
} tPlano;
typedef struct {
	tPlano plano_mina;
	int fila;
	int columna;
} tMina;
typedef char tPlanoCaracteres[3 * MAX][3 * MAX];
typedef int tPlanoColores[3 * MAX][3 * MAX];


//FUNCIONES:
void cargar_Mina(ifstream& fichero, tMina& mina);
void dibujar1_1(const tPlano& plano_mina);
void dibujar1_3(const tPlano& plano_mina);

///////
#endif
