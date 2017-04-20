#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef struct{
    int modo;
    int turno;
    int fils;
    int cols;
    int cant1;
    int cant2;
    char**tablero;
}Tjuego;

typedef struct{
    int fil;
    int col;
}Tdupla;

typedef struct{
    Tdupla origen;
    Tdupla destino;
    int capturados;
    int distancia;
}Tmovimiento;

#endif
