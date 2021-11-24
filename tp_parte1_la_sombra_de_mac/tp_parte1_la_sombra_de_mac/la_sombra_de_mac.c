#include "la_sombra_de_mac.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRIBA 'W'
#define IZQUIERDA 'A'
#define ABAJO 'S'
#define DERECHA 'D'
#define VIDA 3
#define MAC 'M'
#define BLOO 'B'
#define PUERTA 'D'
#define ESCALERAS 'E'
#define LLAVE 'L'
#define MONEDA 'C'
#define POZO 'W'
#define INTERRUPTOR 'O'
#define PORTAL 'P'
#define VELA 'V'

typedef struct vela{
	int fila_vela;
	int col_vela;
}vela_t;

typedef struct portal{
	int fila_portal;
	int col_portal;
}portal_t;

typedef struct interruptor{
	int fila_interruptor;
	int col_interruptor;
}interruptor_t;

typedef struct pozo{
	int fila_pozo;
	int col_pozo;
}pozo_t;

typedef struct moneda{
	int fila_moneda;
	int col_moneda;
}moneda_t;

typedef struct llave{
	int fila_llave;
	int col_llave;
}llave_t;

typedef struct escalera{
	int fila_escalera;
	int col_escalera;
}escalera_t;

typedef struct puerta{
	int fila_puerta;
	int col_puerta;
}puerta_t;

bool coordenadas_iguales(coordenada_t a, coordenada_t b){
	return (a.fila == b.fila) && (a.col == b.col);
}

void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){

	(*ref_personaje).posicion.fila = arranque_personaje.fila;
	(*ref_personaje).posicion.col = arranque_personaje.col;
	(*ref_personaje).vida = VIDA;
	(*ref_personaje).puntos = 0;
	(*ref_personaje).tiene_llave = false;
	(*ref_personaje).interruptor_apretado = false;
}

void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){

	(*ref_sombra).posicion.fila = arranque_sombra.fila;
	(*ref_sombra).posicion.col = arranque_sombra.col;
	(*ref_sombra).esta_viva = true;
}

bool es_movimiento_valido(char movimiento){
	return (movimiento == ARRIBA) || (movimiento == ABAJO) || (movimiento == IZQUIERDA) || (movimiento == DERECHA);
}

bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){
	return ((posicion.fila > -1) && (posicion.fila < max_alto) && (posicion.col > -1) && (posicion.col < max_ancho));
}

void mover_mac(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento, int max_ancho, int max_alto){
	if (movimiento == ABAJO){
		ref_personaje->posicion.fila++;
		if(!(esta_dentro_rango(ref_personaje->posicion, max_alto, max_ancho))){
			ref_personaje->posicion.fila--;
		}
	}else if (movimiento == ARRIBA){
		ref_personaje->posicion.fila--;
		if(!(esta_dentro_rango(ref_personaje->posicion, max_alto, max_ancho))){
			ref_personaje->posicion.fila++;
		}
	}else if (movimiento == IZQUIERDA){
		ref_personaje->posicion.col--;
		if(!(esta_dentro_rango(ref_personaje->posicion, max_alto, max_ancho))){
			ref_personaje->posicion.col++;
		}
	}else{
		ref_personaje->posicion.col++;
		if(!(esta_dentro_rango(ref_personaje->posicion, max_alto, max_ancho))){
			ref_personaje->posicion.col--;
		}
	}
}

void mover_bloo(sombra_t* ref_sombra, personaje_t* ref_personaje, char movimiento, int max_ancho, int max_alto){
		if (movimiento == ABAJO){
		ref_sombra->posicion.fila++;
	
		if(!(esta_dentro_rango(ref_sombra->posicion, max_alto, max_ancho))){
			ref_sombra->posicion.fila--;
		}
	}else if (movimiento == ARRIBA){
		ref_sombra->posicion.fila--;
		
		if(!(esta_dentro_rango(ref_sombra->posicion, max_alto, max_ancho))){
			ref_sombra->posicion.fila++;
		}
	}else if (movimiento == IZQUIERDA){
		ref_sombra->posicion.col++;
		
		if(!(esta_dentro_rango(ref_sombra->posicion, max_alto, max_ancho))){
			ref_sombra->posicion.col--;
		}
	}else{
		ref_sombra->posicion.col--;
		
		if(!(esta_dentro_rango(ref_sombra->posicion, max_alto, max_ancho))){
			ref_sombra->posicion.col++;
		}
	}
}


void mover_personaje(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){
	int max_ancho = MAX_FILAS;
	int max_alto = MAX_COLUMNAS;

	
	mover_mac(ref_personaje, ref_sombra, movimiento, max_ancho, max_alto);
	mover_bloo(ref_sombra, ref_personaje, movimiento, max_ancho, max_alto);

}

coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){
	
	coordenada_t coordenada;

	coordenada.fila = rand() % max_alto;
	coordenada.col = rand() % max_ancho;

	return coordenada;
}


int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){
	
	int i = 0;
	int posicion_vector;

	while(i < tope){
		if (coordenadas_iguales(elementos[i].posicion, posicion)){
			
			posicion_vector = i;
		}
		i++;
	}

	return posicion_vector;
}

void inicializar_vela(){
	int max_alto = MAX_FILAS;
	int max_ancho = MAX_COLUMNAS;

	vela_t vela;

	coordenada_t posicion = coordenada_aleatoria(max_alto, max_ancho);

	vela.fila_vela = posicion.fila;
	vela.col_vela = posicion.col;
}

void imprimir_terreno(juego_t juego){
	char terreno[MAX_FILAS][MAX_COLUMNAS];

	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMNAS; j++){

			terreno[i][j] = 'o';
		}
	}

	terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col] = MAC;
	terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col] = BLOO;

	for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMNAS; j++){
			printf(" %c ", terreno[i][j]);
		}
		printf("\n");
	}
}
