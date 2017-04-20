 #include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "typedefs.h"
#include "backend.h"

/*-----------------------Constantes-----------------------*/

    /* Fichas */
    #define CHARJ1 'A'
    #define CHARJ2 'Z'
    #define VACIO '0'

/*-----------------------Prototipos de Funciones Static-----------------------------*/

/*Devuelve la cantidad de adyacentes en el nombre y la posicion de los adyacentes en ady[]*/
static int adyacentes (Tjuego* juego, Tdupla* dupla, Tdupla ady[]);

/*Inicializa el tablero todo vacio con las fichas en las esquinas*/
static void inicializarTablero(char** tablero, int fils, int cols);

/* Calcula el movimiento segun la estrategia indicada en estrategiaCPU */
static int mejorMovimiento(Tjuego* juego, Tmovimiento* movimiento);

/* Devuelve una Tdupla con los valores de fil y col */
static Tdupla dupla(int fil, int col);

/* Calcula y asigna la distancia de un movimiento, los valores posibles son 1, 2 y 0 en caso de error */
static void calcDistMov(Tmovimiento* mov);
 
 /* Valor absoluto para numeros enteros */
static int iabs(int i);

/* Genera numeros enteros aleatorios entre i y j */
static int randint(int i, int j);

/* Decide si un movimiento es mejor que otro basado en la estrategia indicada */
static int estrategiaCPU(Tmovimiento* actual, Tmovimiento* nuevo);

int crearJuego(Tjuego* juego)
{
	srand(time(NULL)); /* Cambio la seed del rand */
    juego->tablero=crearTablero(juego->fils, juego->cols);
    juego->cant1=juego->cant2=2; /* Inicialmente cada jugador tiene 2 fichas */
    juego->turno=randint(J1,J2); /* Empieza jugando un jugador al azar (J1 Y J2 son dos constantes int consecutivas, donde J1<J2)*/
	if(juego->tablero==NULL)
	{
		return ERROR;
	}
		return OK;
}

char** crearTablero(int fils, int cols)
{
    char** tablero;
    int i=0;
    /*Reservo espacio para los punteros a las columnas*/
    if((tablero=malloc(sizeof(*tablero)*fils))==NULL)
    	return NULL;
    for(i=0; i<fils; i++)
    {
        /*Reservo espacio para cada columna*/
        tablero[i]=malloc(cols*sizeof(**tablero));
    }
    inicializarTablero(tablero, fils, cols);
    return tablero;
}

static void inicializarTablero(char** tablero, int fils, int cols)
{
	int i,j;
	/* Vacio todo el tablero */
	for(i=0; i<fils; i++)
		for(j=0; j<cols; j++)
			tablero[i][j]=VACIO;

	/*Pongo las fichas en las esquinas*/
	tablero[0][0] = tablero[fils-1][0] = CHARJ1; /*Jugador 1*/
    tablero[0][cols-1] = tablero[fils-1][cols-1] = CHARJ2; /*Jugador 2*/
}

static int adyacentes (Tjuego* juego, Tdupla* dupla, Tdupla ady[])
{
    int i,j,cant = 0, movAr=1, movAb=1, movIzq=1, movDer=1;
    char contrario = (juego->turno==J1)?CHARJ2:CHARJ1;

    /*Restringo el area que recorro dependiendo de la posicion*/
    movAr = !(dupla->fil == 0);
    movAb = !(dupla->fil == juego->fils-1);
    movIzq = !(dupla->col == 0);
    movDer = !(dupla->col == juego->cols-1);

    for (i = (dupla->fil)-movAr; i <= (dupla->fil)+movAb; i++)
    {
        for (j = (dupla->col)-movIzq; j <= (dupla->col)+movDer; j++)
        {
            if (juego->tablero[i][j] == contrario)
            {
                if(ady!=NULL)
                {
                    ady[cant].fil = i;
                    ady[cant].col = j;
                }
                cant++;
            }
        }
    }
    return cant;
}

static int mejorMovimiento(Tjuego* juego, Tmovimiento* mov)
{
    int movArr, movAb, movIzq, movDer, i, j, movPosible = 0;
    Tmovimiento movActual, movDefinitivo;

    movDefinitivo.origen = movActual.origen = mov->origen;

    /*Inicializo movDefinitivo con el peor caso de movimiento*/
    movDefinitivo.capturados = 0;
    movDefinitivo.distancia = 2;

    /*Restringo el area que recorro dependiendo de la posicion en la que estoy*/
    movArr = (mov->origen.fil==0) ? 0 : ((mov->origen.fil==1) ? 1 : 2);
    movIzq = (mov->origen.col==0) ? 0 : ((mov->origen.col==1) ? 1 : 2);
    movAb = (mov->origen.fil==((juego->fils)-1)) ? 0 : (mov->origen.fil==((juego->fils)-2) ? 1 : 2);
    movDer = (mov->origen.col==(juego->cols-1)) ? 0 : (mov->origen.col==(juego->cols-2) ? 1 : 2);

    for(i=mov->origen.fil-movArr; i<=mov->origen.fil+movAb; i++)
    {
        for(j=mov->origen.col-movIzq; j<=mov->origen.col+movDer; j++)
        {
            if((juego->tablero)[i][j]==VACIO)
            {
                movActual.destino = dupla(i,j);
                /*Paso null porque no me interesan cuales son los adyacentes*/
                movActual.capturados = adyacentes(juego, &(movActual.destino), NULL);
                calcDistMov(&movActual);

                /*Me quedo con el movimiento que cumpla la estrategia, o con el unico movieminto posible*/
                if (estrategiaCPU(&movDefinitivo, &movActual) || !movPosible)
                    movDefinitivo=movActual;
                movPosible=1;
            }
        }
    }
    if(movPosible)
        *mov=movDefinitivo;
    return movPosible;
}

int juegaComputadora(Tjuego* juego)
{
    int i, j, existeMovimiento=0;
    Tmovimiento movActual, mejorMov;

    for(i=0; i<juego->fils; i++)
    {
        for(j=0; j<juego->cols; j++)
        {
            if((juego->tablero)[i][j]==CHARJ2)
            {
                movActual.origen = dupla(i,j);
                if(mejorMovimiento(juego, &movActual))
                {
                    if (estrategiaCPU(&mejorMov, &movActual) || !existeMovimiento)
                        mejorMov=movActual;
					existeMovimiento=1;
                }
            }
        }
    }
    if(existeMovimiento)
        realizarMovimiento(juego, &mejorMov);

    return existeMovimiento;

}

static int estrategiaCPU(Tmovimiento* actual, Tmovimiento* nuevo)
{
    /*Reemplazo el movimiento definitivo si el movimiento nuevo captura mas fichas*/
    if (nuevo->capturados>actual->capturados)
    {
        return 1;
    }
    if(nuevo->capturados==actual->capturados)
    {
        /*Reemplazo si el actual captura la misma cantidad pero el movimiento es de menor distancia*/
        if (nuevo->distancia<actual->distancia)
            return 1;

        /*Si la distancia y la cantidad que capturan es la misma, elijo uno al azar*/
        if (nuevo->distancia==actual->distancia && randint(0,1))
            return 1;
    }
    return 0;
}

int verificarMovimiento (Tjuego* juego, Tmovimiento* mov)
{
	int flag = 0;
    calcDistMov(mov);
    if( (mov->destino.fil) < (juego->fils) && (mov->destino.col) < (juego->cols) )
    {
       if( (juego->tablero[mov->destino.fil][mov->destino.col]) == VACIO )
       {
            if (mov->distancia && ((mov->origen.col) != (mov->destino.col) || (mov->origen.fil) != (mov->destino.fil)) )
            {
                if (juego->turno == 1)
                {
                    if ((juego->tablero[mov->origen.fil][mov->origen.col]) == CHARJ1)
                    {
                        flag = 1;
                    }
                }
                else
                {
                    if ((juego->tablero[mov->origen.fil][mov->origen.col]) == CHARJ2)
                    {
                        flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}

void realizarMovimiento (Tjuego* juego, Tmovimiento* mov)
{
    int i, capturadas = 0;
    char jugadorActual = ((juego->turno == J1)? CHARJ1 : CHARJ2);
    Tdupla ady[8]; /* aqui se guardaran las ubicaciones de las manchas adyacentes del jugador contrario, como maximo 8 */
    juego->tablero[mov->destino.fil][mov->destino.col] = juego->tablero[mov->origen.fil][mov->origen.col]; /* ubico la en la nueva posicion */
    if (mov->distancia==2) /* si se movio una distancia 2 borro la mancha original */
    {
        juego->tablero[mov->origen.fil][mov->origen.col] = VACIO;
    }
    else
    {
        if (juego->turno == 1)
            juego->cant1++;
        else
            juego->cant2++;
    }
    capturadas = adyacentes(juego, &(mov->destino), ady);
    for (i = 0; i < capturadas; i++) /* cambio las manchas capturadas para que pertenezcan al jugador actual */
    {
        juego->tablero[ady[i].fil][ady[i].col] = jugadorActual;
    }
    if (juego->turno == 1)
    {
        juego->cant1 += capturadas;
        juego->cant2 -= capturadas;
    }
    else
    {
        juego->cant2 += capturadas;
        juego->cant1 -= capturadas;
    }
}

int hayMovimiento (Tjuego* juego, Tdupla* dupla, char jugador)
{
    int i,j, hayMov = 0, movAr=2, movAb=2, movIzq=2, movDer=2;
	
	/*Restringo el area que recorro dependiendo de la posicion*/
	
    if (dupla->fil == 0)
        movAr = 0;
    else if(dupla->fil == 1)
        movAr = 1;
    if (dupla->fil == juego->fils-1)
        movAb = 0;
    else if (dupla->fil == juego->fils-2)
        movAb = 1;
    if (dupla->col == 0)
        movIzq = 0;
    else if (dupla->col == 1)
        movIzq = 1;
    if (dupla->col == juego->cols-1)
        movDer = 0;
    else if (dupla->col == juego->cols-2)
        movDer = 1;
    for (i = (dupla->fil)-movAr; i <= (dupla->fil)+movAb && !hayMov; i++)
    {
        for (j = (dupla->col)-movIzq; j <= (dupla->col)+movDer && !hayMov; j++)
        {
            if (juego->tablero[i][j] == jugador)
            {
                hayMov = 1;
            }
        }
    }
    return hayMov;
}

static Tdupla dupla(int fil, int col)
{
    Tdupla resp;
	resp.fil = fil;
	resp.col = col;
    return resp;
}

static void calcDistMov(Tmovimiento* mov)
{
    if(iabs(mov->destino.col - mov->origen.col)==2 || iabs(mov->destino.fil - mov->origen.fil)==2)
        mov->distancia = 2;
    else if(iabs(mov->destino.col - mov->origen.col)==1 || iabs(mov->destino.fil - mov->origen.fil)==1)
        mov->distancia = 1;
    else
        mov->distancia = 0;
    return;
}

static int iabs(int i)
{
    return i<0 ? (i*-1) : i;
}

static int randint(int i, int j)
{
    return (int)((rand()/(RAND_MAX+1.0))*(i-j+1)+i);
}

int verificaPerdida (Tjuego *juego)
{
    Tdupla aux;
    int i, j, hayMov1 = 0, hayMov2 = 0, pierde = 0;
    for (i = 0; i < juego->fils && (!hayMov1 || !hayMov2); i++)
    {
        for (j = 0; j < juego->cols && (!hayMov1 || !hayMov2); j++)
        {
            if (juego->tablero[i][j] == VACIO)
            {
                aux.fil = i;
                aux.col = j;
                if (!hayMov1 && hayMovimiento(juego, &aux, CHARJ1))
                    hayMov1 = 1;
                if (!hayMov2 && hayMovimiento(juego, &aux, CHARJ2))
                    hayMov2 = 1;
            }
        }
    }
    if (!hayMov1 && hayMov2)
	{
        pierde = J1;
	}
    else if (!hayMov2 && hayMov1)
	{
        pierde = J2;
	}
    else if (!hayMov1 && !hayMov2)
	{
        if (juego->cant1 > juego->cant2)
		{
            pierde = J2;
		}
        else
		{
            pierde = J1;
		}
	}
    return pierde;
}

void rellenarTablero(Tjuego* juego, int perdedor)
{
    int i, j;
    for(i=0; i<juego->fils; i++)
    {
        for(j=0; j<juego->cols; j++)
        {
            if((juego->tablero)[i][j]==VACIO)
            {
                switch(perdedor)
                {
                    case J1: juego->cant2++; (juego->tablero)[i][j]=CHARJ2; break;
                    case J2: juego->cant2++; (juego->tablero)[i][j]=CHARJ1; break;
                }
            }
        }
    }
}
