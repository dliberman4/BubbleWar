#include <stdio.h>
#include "backend.h"
#include "archivos.h"
#include "constantes.h"

/*Codigos de retorno general*/
#define OK 1
#define ERROR 0

int cargarArchivo(Tjuego* juego, char* nombreArchivo)
{
	/* Abro  el archivo en modo lectura */
	FILE *archivo=fopen(nombreArchivo, "r");
	int i;

	if(archivo==NULL)
	{
		/* Error al abrir el archivo*/
		return ERR_ABRIR_ARCH;
	}
	if(fread(juego, sizeof(int), 6, archivo)!=6) /* Intento recuperar todos los datos excepto la matriz */
	{
		/* Error al leer el archivo */
		fclose(archivo);
		return ERR_LEER_ARCH;
	}
	/* Como ya tengo las dimensiones del tablero, genero la matriz. */
	juego->tablero = crearTablero(juego->fils, juego->cols);
	for(i=0; i<juego->fils; i++)
	{
		/* Voy rellenando los elementos de cada fila, si no pude rellenar una fila, devuelvo error */
		if(fread((juego->tablero)[i], sizeof(char), juego->cols, archivo)!=juego->cols)
		{
			/* Error al leer el archivo */
			fclose(archivo);
			return ERR_LEER_ARCH;
		}
	}
	fclose(archivo);
	return OK;
}

int guardarArchivo(Tjuego* juego, char* nombreArchivo)
{
	/* Abro  el archivo en modo escritura */
	FILE *archivo=fopen(nombreArchivo, "w");
	int i;

	if(archivo==NULL)
	{
		/* Error al abrir el archivo */
		return ERR_ABRIR_ARCH;
	}
	if(fwrite(juego, sizeof(int), 6, archivo)!=6) /* Intento guardar todos los datos excepto la matriz */
	{
		/* Error al escribir el archivo */
		fclose(archivo);
		return ERR_ESC_ARCH;
	}
	/* Guardo los datos de la matriz */
	for(i=0; i<juego->fils; i++)
	{
		/* Voy rellenando los elementos de cada fila, si no pude rellenar una fila, devuelvo error */
		if(fwrite((juego->tablero)[i], sizeof(char), juego->cols, archivo)!=juego->cols)
		{
			/* Error al escribir el archivo */
			fclose(archivo);
			return ERR_ESC_ARCH;
		}
	}
	fclose(archivo);
	return OK;
}