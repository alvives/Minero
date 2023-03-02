#include "juego.h"
#include "Puntuaciones.h"
#include "checkML.h"


void mostrarMenu();
void mostrarMenuMovimientos();
int leerOpcion(int limInf, int limSup);
void mostrarMenuNivel();
bool jugar_un_nivel(tJuego& juego, int nivel);
//extras:
void calcular_Puntos(tJuego& juego, int& puntos);
void actualizar_Marcador(tJuego& juego, tPuntuacionJugador& jugador, const int puntos);


int main(){
	tPuntuaciones marcador;
	tJuego juego;
	int opc, pos, puntos;
	string nombre;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	system("chcp 1252");  // Para no tener problemas con acentos, ñ,...

	cargar_Marcador(marcador);
	mostrar_Datos_Usuarios(marcador);
	cout << "Introduce el nombre del usuario: ";
	cin >> nombre;
	if (buscar(nombre, marcador, pos)) {
		cout << "Ya estas registrado." << endl << "Mira las minas que has recorrido por nivel: " << endl << endl;
		mostrar_Minas_Usuario(marcador, pos);
	}
	else {
		insertar(marcador, nombre, pos);
		inicializar_Jugador(marcador.array_clasificacion[pos]);
		cout << "Eres un nuevo usuario: " << nombre << endl;
		cout << "Mira las puntuaciones de los otros jugadores (Tu de momento tienes 0):" << endl;
		mostrar_Alfabetico(marcador);
	}
	system("pause");


	do {
		mostrarMenuNivel();
		opc = leerOpcion(0, 5);

		if (opc == 0) {
			juego.estado = ABANDONO;
		}		 
		else {
			if (jugar_un_nivel(juego, opc)) {
				if (juego.estado == GANADO) {
					calcular_Puntos(juego, puntos);
					actualizar_Marcador(juego, marcador.array_clasificacion[pos], puntos);
					mostrar_Minas_Usuario(marcador, pos);
				}
			}
		}
		system("pause");
	} while (juego.estado != ABANDONO);

	if (!guardar_Marcador(marcador)) {
		cout << "Lo sentimos, la partidas no se han podido guardar..." << endl;
	}
	destruir(marcador);
}

// visualización del menú principal
void mostrarMenu(){
	system("cls");
	cout << "\n\t\t 1. Jugar partida a escala 1:1." << endl;
	cout << "\t\t 2. Jugar partida a escala 1:3." << endl;
	cout << "\t\t 0. Salir." << endl;
	cout << "\t\t Introduce una opcion: ";
}
// visualiación del menú introducir movimientos
void mostrarMenuMovimientos() {
	system("cls");
	cout << "\n\t\t 1. Introducir movimientos por teclado." << endl;
	cout << "\t\t 2. Introducir movimientos por fichero." << endl;
	cout << "\t\t 0. Salir." << endl;
	cout << "\t\t Introduce una opcion: ";
}
// lectura validada de un número en el intervalo [limInf, limSup]
int leerOpcion(int limInf, int limSup){
	int i;
	cin >> i;
	while (cin.fail() || i < limInf || i>limSup){
		cin.clear(); cin.sync();
		cout << "\n\tOpción incorrecta. Introduce una opción: ";
		cin >> i;
	};
	cin.sync(); 
	return i;
}
// visualización del menú de los niveles
void mostrarMenuNivel() {
	system("cls");
	cout << "\t\t 1. Jugar nivel 1" << endl;
	cout << "\t\t 2. Jugar nivel 2" << endl;
	cout << "\t\t 3. Jugar nivel 3" << endl;
	cout << "\t\t 4. Jugar nivel 4" << endl;
	cout << "\t\t 5. Jugar nivel 5" << endl;
	cout << "\t\t 0. Salir." << endl;
	cout << "\t\t Introduce una opcion: ";
}
// ejecuta el juego, y devuelve false si el jugador abandona
bool jugar_un_nivel(tJuego& juego, int nivel) {
	int opc;
	bool cargado, seguimos = true;

	inicializar_Juego(juego, nivel);

	mostrarMenu();
	opc = leerOpcion(0, 2);
	if (opc == 1 || opc == 2) {

		cargado = cargar_Juego(juego);
		if (cargado) {
			if (opc == 1) juego.escala = 1;
			if (opc == 2) juego.escala = 2;

			mostrarMenuMovimientos();
			opc = leerOpcion(0, 2);
			if (opc == 0) {
				juego.estado = ABANDONO;
				seguimos = false;
			}
			else {
				if (opc == 1) {
					jugar_desde_teclado(juego);
				}
				else if (opc == 2) {
					jugar_desde_fichero(juego);
				}
			}
		}
		else {
			cout << "\n\tNo se ha podido cargar el fichero o no existe el nivel solicitado\n";
			opc = 0;
		}

	}
	else {
		juego.estado = ABANDONO;
		seguimos = false;
	}

	return seguimos;
}

// extras:
void calcular_Puntos(tJuego& juego, int& puntos) {
	puntos = juego.mina.plano_mina.ncolumnas * juego.mina.plano_mina.nfilas;
	puntos = puntos + A * juego.total_gemas_recogidas;
	puntos = puntos - juego.num_movimientos;
	puntos = puntos - B * juego.num_dinamitas_usadas;
}
void actualizar_Marcador(tJuego& juego, tPuntuacionJugador& jugador, const int puntos) {
	if (jugador.vMinasRecorridas[juego.nivel - 1].numMovimientos == 0) {
		jugador.minasRecorridas++;
	}

	jugador.punt_total = jugador.punt_total - jugador.vMinasRecorridas[juego.nivel - 1].puntosMina;

	jugador.vMinasRecorridas[juego.nivel - 1].numDinamitas = juego.num_dinamitas_usadas;
	jugador.vMinasRecorridas[juego.nivel - 1].numGemas = juego.total_gemas_recogidas;
	jugador.vMinasRecorridas[juego.nivel - 1].numMovimientos = juego.num_movimientos;
	jugador.vMinasRecorridas[juego.nivel - 1].puntosMina = puntos;

	jugador.punt_total = jugador.punt_total + puntos;
}