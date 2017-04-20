#ifndef BACKEND_H
#define BACKEND_H

#include "typedefs.h"

/* Constantes */
#define OK 1
#define ERROR 0
#define J1 1
#define J2 2
#define MOD_2JUG 0
#define MOD_1JUG 1

/* Crea el tablero e inicializa las variables del juego */
int crearJuego(Tjuego* juego);

/*Devuelve null si no se pudo crear el tablero, sino devuelve el puntero al tablero*/
char** crearTablero(int fils, int cols);

/* Verifica si algun jugador perdio por bloqueo o porque no tiene mas fichas */
int verificaPerdida (Tjuego *juego);

/* La computadora realiza su movimiento siguiendo su estrategia */
int juegaComputadora(Tjuego* juego);

/* Rellena el tablero con las fichas del ganador, recibe como argumento el perdedor */
void rellenarTablero(Tjuego* juego, int perdedor);

/* Realiza el movimiento mov, que debe ser validado con anterioridad */
void realizarMovimiento (Tjuego* juego, Tmovimiento* mov);

/* Indica si el movimiento mov es valido */
int verificarMovimiento(Tjuego* juego, Tmovimiento* mov);

#endif
