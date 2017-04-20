#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "backend.h"
#include "archivos.h"
#include "constantes.h"
#include "typedefs.h"
#include "getnum.h"

/* Prototipos */

void pedirTablero(int* fils, int* cols);
void mostrarTabla(Tjuego* juego);
int menu(void);
void leerEntrada(char comando[], char* mensaje);
void mensajeError(char comando[]);
int aJugar(Tjuego* juego);
int deseaGuardar(void);
int parsearComandoMov(char* comando, Tmovimiento* mov);
/* Verifica que el comando ingresado sea valido y se lee con formato */
int verifComando(char*comando,Tmovimiento* mov, char* filename);

int main(void)
{
	Tjuego juego;
	int opcion, volver, mostrarMenu;
	char nombreArchivo[MAXCOMANDO];
	mostrarMenu=1;
	
	printf("Bienvenido al juego Blob Wars\n---------------------------------------------------------\n");
	printf("Trabajo especial realizado por Daniella Liberman, Giuliano Scaglioni\n");
	printf("y Lorant Mikolas.\n---------------------------------------------------------\n");
	
	while(mostrarMenu!=QUIT)
	{
		do
		{
			volver=0;
			opcion = menu();
			switch(opcion)
			{
				case MOD_2JUG:
				case MOD_1JUG:
					juego.modo=opcion;
					pedirTablero(&(juego.fils), &(juego.cols));
					crearJuego(&juego);
					break;
				case MOD_RECUPERAR:
					leerEntrada(nombreArchivo, "Ingrese el nombre del archivo: ");
					if(cargarArchivo(&juego, nombreArchivo) != OK)
					{
						volver=1;
						printf("Error al cargar el archivo!\n");
					}
					break;
				case QUIT: return 0; /* Salgo del juego */
			}
		} while(volver);
		mostrarMenu=aJugar(&juego);
	}

	return 0;
}

int aJugar(Tjuego* juego)
{
	Tmovimiento mov;
	char comando[MAXCOMANDO];
	int finJuego=1;
	int tipocom=1;
	int volver;
	int perdedor;
	char filename[MAXCOMANDO];

	while(finJuego)
	{
		mostrarTabla(juego);
		printf("Fichas jugador 1: %d\nFichas jugador 2: %d\n", juego->cant1, juego->cant2);
        if(juego->modo==MOD_2JUG || juego->turno==1)
        {
			printf("Es el turno del jugador %d\n---------------------------------------------------------\n", juego->turno);
            tipocom=SINASIGNAR;
            do
            {   volver=0;
				if(tipocom==SINASIGNAR)
				{
					leerEntrada(comando, "Escriba el movimiento, salve partida o salga (finalice con enter)");
					tipocom=verifComando(comando,&mov, filename);
				}
				else if(tipocom==GUARDARYSALIR)
					leerEntrada(filename, "Ingrese el nombre del archivo: ");
                switch (tipocom)
                {
                    case MOVIMIENTO:
                        if (verificarMovimiento(juego, &mov))
                        {
                            realizarMovimiento(juego, &mov);
                        }
                        else
                        {
                            volver=OK;
							tipocom=SINASIGNAR;
                            printf("Movimiento no valido!\n");
                        }
						break;
                    case QUIT:
                        if (deseaGuardar())
                        {
                            tipocom=GUARDARYSALIR;
                            volver=OK;
                        }
                        else
                            return QUIT;
						break;
					case GUARDARYSALIR:
                    case GUARDAR:
                        if (guardarArchivo(juego, filename) != OK)
                        {
                            printf("ERROR al guardar el archivo %s\n", filename);
                        }
						else if(tipocom==GUARDARYSALIR)
							return QUIT;
						tipocom=SINASIGNAR;
                        volver=OK;
						break;
					default:
						mensajeError(comando);
						volver=1;
						tipocom=SINASIGNAR;
						break;
                }
            } while (volver);
        }
        else
        {
			printf("Esta por jugar la CPU\n");
            juegaComputadora(juego);
			printf("Juego la CPU\n");
        }
		if(!juego->cant1)
		{
			mostrarTabla(juego);
			printf("Gano el jugador 2 con %d fichas\nFelicitaciones!!!\n", juego->cant2);
			return OK;
		}
		else if(!juego->cant2)
		{
			mostrarTabla(juego);
			printf("Gano el jugador 1 con %d fichas\nFelicitaciones!!!\n", juego->cant1);
			return OK;
		}
		else
		{
			perdedor = verificaPerdida(juego);
			if (perdedor)
			{
				rellenarTablero(juego, perdedor);
				printf("Gano el jugador %d con %d fichas\nFelicitaciones!!!\n", (perdedor==1)?J2:J1, (perdedor==1)?juego->cant2:juego->cant1);
				return OK;
			}
		}
        juego->turno = (juego->turno == J1)?J2:J1;
	}
	return OK;
}

void pedirTablero(int* fils, int* cols)
{
    int flag=1, opcion;
    do
    {
		if(!flag)
        {
            printf("\nNO INGRESO UN NUMERO ENTRE 5 Y 30\n");
        }
        opcion = getint("Ingrese la cantidad de filas (Entre 5 y 30): ");
		flag=0;
    } while(opcion>30 || opcion<5);
    *fils=opcion;
    flag=1;
    do
    {
    	if(!flag)
        {
            printf("\nNO INGRESO UN NUMERO ENTRE 5 Y 30\n");
        }
        opcion = getint("Ingrese la cantidad de columnas (Entre 5 y 30): ");
		flag=0;
    } while(opcion>30 || opcion<5);
    *cols=opcion;
}

int deseaGuardar()
{
    int opcion, flag=0;
    printf("\n---------------------------------------------------------\n");

     do
	{
		if(flag)
			printf("NO INGRESO UNA OPCION VALIDA(ingrese 1 o  2)\n");
        printf("Desea Guardar?\n");

		opcion=getint("Presione 1 si desea Guardar o 2 Salir.\n");
		flag=1;
	}
	while(opcion!=1 && opcion!=2);
    printf("\n---------------------------------------------------------\n");
    return (opcion==1)?1:0;
}

void mostrarTabla(Tjuego* juego)
{
	int i=0, j=0;
    printf("\n---------------------------------------------------------\n");
    printf("    ");
    for(j=0; j<juego->cols; j++)
        printf("%2d ", j);
    putchar('\n');
    for(i=0; i<(juego->fils); i++)
    {
        printf("%3d ", i);
        for(j=0; j<(juego->cols); j++)
        {
            putchar(' ');
            if(!((juego->tablero)[i][j]=='0'))
                putchar((juego->tablero)[i][j]);
            else
                putchar(' ');
            putchar(' ');
        }
        putchar('\n');
    }
    printf("\n---------------------------------------------------------\n");

    return;
}

/*Menu para el front end*/
int menu(void)
{
	int opcion;
	int flag=0;
	printf("Menu:\n");
	printf("1- Juego de dos jugadores.\n");
	printf("2- Juego contra computadora \n");
	printf("3- Recuperar un juego grabado \n");
	printf("4- Terminar\n");

	do
	{
		if(flag)
			printf("NO INGRESO UN NUMERO ENTRE 1 Y 4\n");
		opcion=getint("Presione 1,2,3 o 4 para elegir la opcion que desea.\n");
		flag=1;
	}
	while(opcion>4 || opcion<1); /* verifico si fue una entrada correcta */

	return opcion-1;
}

void mensajeError(char comando[])
{
	printf("\n---------------------------------------------------------\n");
	printf("%s", comando);
	printf("\nNO ES VALIDO.\n");
	printf("Intente nuevamente...\n");
	printf("---------------------------------------------------------\n");
	return;
}

int verifComando(char*comando, Tmovimiento* mov, char* filename)
{
	int estado;
	int tipocom=ERROR;

	if(comando[0]=='[')
		estado=MOVIMIENTO;
	else if(comando[0]=='s')
		estado=GUARDAR;
	else if(comando[0]=='q')
		estado=QUIT;


	switch(estado)
	{
		case MOVIMIENTO:
            if(parsearComandoMov(comando, mov))
                tipocom=MOVIMIENTO;
			break;

		case GUARDAR:
            if(strncmp(comando,COM_SAVE,FILE_POS)==0 && comando[FILE_POS]!=0)
            {
                strcpy(filename, comando+FILE_POS);
                tipocom=GUARDAR;
            }
            break;

		case QUIT: 
            if(strcmp(comando, COM_QUIT)==0)
			{
				tipocom=QUIT;
			}
	}
	return tipocom;

}

int parsearComandoMov(char* comando, Tmovimiento* mov)
{
    int estado=INICIO, i=0, j=0;
    int v[4] = {0};
    while(comando[i]!='\0' && estado!=ERROR && estado!=FIN)
    {
        switch(estado)
        {
            case INICIO:
                if(comando[i]=='[')
                    estado=NUEVO;
                else
                    estado=ERROR;
                break;
            case NUEVO:
                if(isdigit(comando[i]))
                {
                    v[j]=comando[i]-'0';
                    estado=NUMERO;
                }
                else
                {
                    estado=ERROR;
                }
                break;
            case NUMERO:
                if(isdigit(comando[i]))
                    v[j]=v[j]*10+comando[i]-'0';
                else if(comando[i]==',')
                {
                    estado=NUEVO;
                    j++;
                }
                else if(comando[i]==']')
                {
                    if(j<2)
                    {
                        j++;
                        estado=INICIO;
                    }
                    else
                    {
                        estado=FIN;
                    }
                }
                else
                {
                    estado=ERROR;
                }
                break;
        }
        i++;
    }
    if(estado==FIN && comando[i]=='\0')
    {
        mov->origen.fil = v[0];
        mov->origen.col = v[1];
        mov->destino.fil = v[2];
        mov->destino.col = v[3];
        return 1;
    }
    else
        return 0;
}

void leerEntrada(char comando[], char* mensaje)
{

	char c;
	int i=0;
	printf("%s\n", mensaje);
	while((c=getchar())!='\n')
	{
		if(i<MAXCOMANDO-1)
		{
			comando[i]=c;
			i++;
		}

	}
	comando[i]='\0';


	return;
}
