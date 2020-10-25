#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "copa.h"
#define  GANADO 1
#define  EN_CURSO 0
#define  PERDIDO -1

/*
	Postcondion: La funcion mostrara por pantalla los datos del juego en tiempo real y las instrucciones del mismo.
*/
void mostrar_estado_juego_y_instrucciones(juego_t juego){
	printf("\033[0;0m");
	printf("               w (ARRIBA)             P(POCION) I(IMPEDIMENTA) R(RIDDIKULUS) F(ESFINGE)\n");
	printf("                                      'AYUDAS\n");
	printf("      a        s         d                                                  OBSTACULOS'\n");
	printf(" (IZQUIERDA) (ABAJO) (DERECHA)                   E(ESCREGUTO) A(ACROMANTULA) B(BOGGART)\n");

	printf("\033[1;36m");//color celeste
	printf("   *JUGADOR*\n");
	printf("\033[0;0m");
	printf("Vida: %i\n",juego.jugador.vida);
	printf("Ayudas jugador:  ");
	for(int i = 0;i < juego.jugador.tope_ayudas;i++){
		printf("%c ",juego.jugador.ayudas[i].codigo);
	}
	printf("\n");
	printf("Estado del juego: ");
	switch(estado_juego(juego)){
		case GANADO:
			printf("\033[1;32m");//color verde
			printf("Has ganado,Felicitaciones!\n");
			break;
		case PERDIDO:
			printf("\033[1;31m");//color rojo
			printf("Has perdido :(\n");
			break;
		case EN_CURSO:
			printf("En curso.\n");	
		default:
			break;
	}	
}

int main(){

	juego_t juego;
	char tecla;
	char laberinto_mostrado[TAMANIO][TAMANIO];
	srand((unsigned int)time(NULL));

	inicializar_laberinto(&juego);
	actualizar_laberinto(juego,laberinto_mostrado);
	mostrar_estado_juego_y_instrucciones(juego);
	mostrar_laberinto(laberinto_mostrado);
	do{
		scanf(" %c",&tecla);
		while(!es_movimiento_valido(&juego,tecla)){
			printf("\033[1;31m");//color rojo
			printf("La tecla ingresada es incorrecta,intente de nuevo: ");
			scanf(" %c",&tecla);
		}
		mover_jugador(&(juego.jugador),tecla);
		mover_rival(&juego);
		actualizar_juego(&juego);
		actualizar_laberinto(juego,laberinto_mostrado);
		mostrar_estado_juego_y_instrucciones(juego);
		mostrar_laberinto(laberinto_mostrado);
	}while(estado_juego(juego) == EN_CURSO);

	return 0;
}