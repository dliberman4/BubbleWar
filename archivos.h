#ifndef ARCHIVOS_h
#define ARCHIVOS_H

#include "typedefs.h"

/* Codigos de retorno */
#define ERR_ABRIR_ARCH 2
#define ERR_ESC_ARCH 3
#define ERR_LEER_ARCH 3

/* Carga los datos de nombreArchivo en la estrructura del juego */
int cargarArchivo(Tjuego* juego, char* nombreArchivo);

/* Guarda los datos de la estructura del juego en nombreArchivo */
int guardarArchivo(Tjuego* juego, char* nombreArchivo);

#endif
