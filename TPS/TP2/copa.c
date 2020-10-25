#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "copa.h"
#include "laberinto.h"
/*---------------------------------------*/
//CONSTANTES
#define ARRIBA 'w'
#define ABAJO 's'
#define DERECHA 'd'
#define IZQUIERDA 'a'
const int VIDA_MOVIMIENTO = 3;
#define  GANADO 1
#define  EN_CURSO 0
#define  PERDIDO -1

#define TOTAL_POSICIONES 23//TOTAL_AYUDAS+TOTAL_OBSTACULOS+JUGADOR+RIVAL+COPA

#define COPA 'C'
#define JUGADOR 'J'
const int VIDA_MAXIMA = 50;
#define RIVAL 'G'
const int CANT_PASOS = 4;

#define ESCREGUTO 'E'
const int CANTIDAD_ESCREGUTO = 1;
const int DANIO_ESCREGUTO = 20;
#define ACROMANTULA 'A'
const int CANTIDAD_ACROMANTULA = 1;
const int DANIO_ACROMANTULA = 10;
#define BOGGART 'B'
const int CANTIDAD_BOGGART = 1;
const int DANIO_BOGGART = 15;

#define IMPEDIMENTA 'I'
const int CANTIDAD_IMPEDIMENTA = 1;
#define RIDDIKULUS 'R'
const int CANTIDAD_RIDDIKULUS = 1;
#define POCION 'P'
const int CANTIDAD_POCION = 3;
const int POCION_VIDA = 15;
#define ESFINGE 'F'
const int CANTIDAD_ESFINGE = 1;
/*---------------------------------------*/



/*
	Postcondion: Devolvera una coordenada de rango (0,0) a (TAMANIO,TAMANIO) aleatoriamente.
*/
coordenada_t posicion_aleatoria(void){

	coordenada_t posicion;

	posicion.fil = rand()%TAMANIO;
	posicion.col = rand()%TAMANIO;

	return(posicion);
}
/*
	Postcondion: Devolvera la distancia mangattan entre las dos posiciones que sera un numero entero mayor o igual a 0.
*/
int distancia_manhattan(coordenada_t posicion1, coordenada_t posicion2){

	return ( abs(posicion1.fil - posicion2.fil) + abs(posicion1.col - posicion2.col) );
}
/*
	Precondicion: Recibe una posicion la cual ya debio de estar verificada anteriormente (posicion de (0,0) a (TAMANIO,TAMANIO)).
	Postcondion: Devolvera el caracter del laberinto que se encuentra en esa posicion.
*/
char elemento_posicion(char laberinto_original[TAMANIO][TAMANIO],coordenada_t posicion){

	return (laberinto_original[posicion.fil][posicion.col]);
}
/*
	Postcondion: Devolvera verdadero en caso de que las posiciones sean las mismas o falso en caso de que no lo sean.
*/
bool misma_posicion(coordenada_t posicion1,coordenada_t posicion2){

	return((posicion1.fil == posicion2.fil) && (posicion1.col == posicion2.col));
}
/*
	Postcondion: Devolvera verdadero en caso de que esa posicion se encuentre en el vector de posiciones ocupadas o la posicion sea una pared,devolvera falso en caso contrario.
*/
bool posicion_ocupada(juego_t juego,coordenada_t posicion,coordenada_t posiciones_ocupadas[TOTAL_POSICIONES],int tope_posiciones_ocupadas){

	bool esta_ocupada = false;
	int i = 0;

	if(elemento_posicion(juego.laberinto_original,posicion) != VACIO)
		esta_ocupada = true;
	else{
		while( (i < tope_posiciones_ocupadas) && !esta_ocupada){
		if(misma_posicion(posiciones_ocupadas[i],posicion))
			esta_ocupada = true;
			i++;
		}
	}
	return(esta_ocupada);
}
/*
	Postcondion: La funcion agregara la posicion al vector de posicoines ocupadas y quedara como una posicion ocupada y reveservada para ese elemento,es decir, 
				cuando se quiera una posicion aleatoria esta posicion no sera un posible valor entre el conjunto de valores.
*/
void agregar_posicion_ocupada(coordenada_t posicion,coordenada_t posiciones_ocupadas[TOTAL_POSICIONES],int *tope_posiciones_ocupadas){
	posiciones_ocupadas[(*tope_posiciones_ocupadas)] = posicion;
	(*tope_posiciones_ocupadas)++;
}
/*
	Precondicion: Recibe un vector de posiciones ocupadas, el cual no debe estar lleno para que la copa pueda ser posicionada
	Postcondion: La funcion modificara los datos de la copa, ubicara la copa en el laberinto(esta posicion ya es valida es decir no es pared y no se superpone con otro objeto)
				,cambia su codigo para luego identificarla.
*/
void ubicar_copa(juego_t* juego,coordenada_t posiciones_ocupadas[TOTAL_POSICIONES],int *tope_posiciones_ocupadas,char codigo){

	(*juego).copa.posicion  = posicion_aleatoria();
	while(posicion_ocupada((*juego),(*juego).copa.posicion,posiciones_ocupadas,(*tope_posiciones_ocupadas))){
		(*juego).copa.posicion  = posicion_aleatoria();
	}
	agregar_posicion_ocupada((*juego).copa.posicion,posiciones_ocupadas,tope_posiciones_ocupadas);
	(*juego).copa.codigo = codigo;
}
/*
	Precondicion: Recibe un vector de posiciones ocupadas, el cual no debe estar lleno para que los obstaculos puedan ser posicionados.
	Postcondion: La funcion modificara los datos del obstaculo, ubicara el obstaculo en el laberinto(esta posicion ya es valida es decir no es pared y no se superpone con otro objeto)
				,cambia su codigo para luego identificarlo,cambia su danio y lo agrega al vector de obstaculo.Todo esto lo hara n veces que sera la cantidad que se ingresa.
*/
void ubicar_obstaculo(juego_t* juego,coordenada_t posiciones_ocupadas[TOTAL_POSICIONES],int *tope_posiciones_ocupadas,char codigo,int danio,int cantidad){
	int disponible = TOTAL_OBSTACULOS - (*juego).tope_obstaculos;

	if((*juego).tope_obstaculos < TOTAL_OBSTACULOS){ 
		if(cantidad > disponible)
			cantidad = disponible;

		for(int i = 0;i < cantidad;i++){
			(*juego).obstaculos[(*juego).tope_obstaculos].posicion  = posicion_aleatoria();
			while(posicion_ocupada((*juego),(*juego).obstaculos[(*juego).tope_obstaculos].posicion,posiciones_ocupadas,*tope_posiciones_ocupadas)){
				(*juego).obstaculos[(*juego).tope_obstaculos].posicion  = posicion_aleatoria();
			}
			agregar_posicion_ocupada((*juego).obstaculos[(*juego).tope_obstaculos].posicion,posiciones_ocupadas,tope_posiciones_ocupadas);
			(*juego).obstaculos[(*juego).tope_obstaculos].codigo = codigo;
			(*juego).obstaculos[(*juego).tope_obstaculos].danio = danio;
			(*juego).tope_obstaculos ++;
		}
	}
}
/*
	Precondicion: Recibe un vector de posiciones ocupadas, el cual no debe estar lleno para que las ayudas puedan ser posicionadas.
	Postcondion: La funcion modificara los datos de la ayuda, ubicara la ayuda en el laberinto(esta posicion ya es valida es decir no es pared y no se superpone con otro objeto)
				,cambia su codigo para luego indentificarla,cambia su vida a recuperar y lo agrega al vector de ayudas.Todo esto lo hara n veces que sera la cantidad que se ingresa.
*/
void ubicar_ayuda(juego_t* juego,coordenada_t posiciones_ocupadas[TOTAL_POSICIONES],int *tope_posiciones_ocupadas,char codigo,int vida_a_recuperar,int cantidad){
	int disponible = TOTAL_AYUDAS - (*juego).tope_ayudas;

	if((*juego).tope_ayudas < TOTAL_AYUDAS){
		if(cantidad > disponible)
			cantidad = disponible;

		for(int i = 0;i < cantidad;i++){
			(*juego).ayudas[(*juego).tope_ayudas].posicion  = posicion_aleatoria();
			while( posicion_ocupada((*juego),(*juego).ayudas[(*juego).tope_ayudas].posicion,posiciones_ocupadas,*tope_posiciones_ocupadas)){
				(*juego).ayudas[(*juego).tope_ayudas].posicion  = posicion_aleatoria();
			}
			agregar_posicion_ocupada((*juego).ayudas[(*juego).tope_ayudas].posicion,posiciones_ocupadas,tope_posiciones_ocupadas);
			(*juego).ayudas[(*juego).tope_ayudas].codigo = codigo;
			(*juego).ayudas[(*juego).tope_ayudas].vida_a_recuperar = vida_a_recuperar;
			(*juego).tope_ayudas ++;
		}
	}
}
/*
	Precondicion: Recibe un vector de posiciones ocupadas, el cual no debe estar lleno para que el rival pueda ser posicionado
	Postcondion: La funcion modificara los datos del rival, ubicara al rival en el laberinto(esta posicion ya es valida es decir no es pared y no se superpone con otro objeto)
				,cambia su codigo para luego indentificarlo,cambia su direccion y cambia su cantidad de pasos.
*/
void ubicar_rival(juego_t* juego,coordenada_t posiciones_ocupadas[TOTAL_POSICIONES],int *tope_posiciones_ocupadas,char codigo){
	do{
		(*juego).rival.posicion  = posicion_aleatoria();
		while(posicion_ocupada((*juego),(*juego).rival.posicion,posiciones_ocupadas,*tope_posiciones_ocupadas)){
			(*juego).rival.posicion  = posicion_aleatoria();
		}
	}while(distancia_manhattan((*juego).rival.posicion , (*juego).copa.posicion ) < 10);
	agregar_posicion_ocupada((*juego).rival.posicion,posiciones_ocupadas,tope_posiciones_ocupadas);
	(*juego).rival.codigo = codigo;
	(*juego).rival.cantidad_pasos = 0;
	(*juego).rival.direccion = DERECHA;
}
/*
	Precondicion: Recibe un vector de posiciones ocupadas, el cual no debe estar lleno para que el jugador pueda ser posicionado
	Postcondion: La funcion modificara los datos del jugador, ubicara al jugador en el laberinto(esta posicion ya es valida es decir no es pared y no se superpone con otro objeto)
				,cambia su codigo para luego indentificarlo y cambia su vida.
*/
void ubicar_jugador(juego_t* juego,coordenada_t posiciones_ocupadas[TOTAL_POSICIONES],int *tope_posiciones_ocupadas,char codigo,int vida){	
	do{
		(*juego).jugador.posicion  = posicion_aleatoria();
		while(posicion_ocupada((*juego),(*juego).jugador.posicion,posiciones_ocupadas,*tope_posiciones_ocupadas)){
			(*juego).jugador.posicion  = posicion_aleatoria();
		}
	}while(distancia_manhattan((*juego).jugador.posicion , (*juego).copa.posicion ) < 10);
	agregar_posicion_ocupada((*juego).jugador.posicion,posiciones_ocupadas,tope_posiciones_ocupadas);
	(*juego).jugador.codigo = codigo;
	(*juego).jugador.vida = vida;
	(*juego).jugador.tope_ayudas = 0;
}
/*
	Postcondion: La funcion inicializara todas las estructuras de todos los elementos del juego(copa,jugador,rival.obstaculos y ayudas) ubicandolos correctamente aleatoreamente.
				Ademas creara el laberinto original, es decir, las paredes y pasillos aleatoreamente.
*/
void inicializar_laberinto(juego_t* juego){

	coordenada_t posiciones_ocupadas [TOTAL_POSICIONES];
	int tope_posiciones_ocupadas = 0;
	(*juego).tope_obstaculos = 0;
	(*juego).tope_ayudas = 0; 

    inicializar_paredes_laberinto((*juego).laberinto_original);


	ubicar_copa(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,COPA);
	ubicar_obstaculo(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,ESCREGUTO,DANIO_ESCREGUTO,CANTIDAD_ESCREGUTO);
	ubicar_obstaculo(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,ACROMANTULA,DANIO_ACROMANTULA,CANTIDAD_ACROMANTULA);
	ubicar_obstaculo(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,BOGGART,DANIO_BOGGART,CANTIDAD_BOGGART);
	ubicar_ayuda(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,IMPEDIMENTA,0,CANTIDAD_IMPEDIMENTA);
	ubicar_ayuda(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,RIDDIKULUS,0,CANTIDAD_RIDDIKULUS);	
	ubicar_ayuda(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,POCION,POCION_VIDA,CANTIDAD_POCION);
	ubicar_ayuda(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,ESFINGE,0,CANTIDAD_ESFINGE);
	ubicar_rival(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,RIVAL);
	ubicar_jugador(juego,posiciones_ocupadas,&tope_posiciones_ocupadas,JUGADOR,VIDA_MAXIMA);
}



/*
	Precondicion: Recibe una posicion y un caracter(no otro tipo de dato) y ya debieron estar verificados anteriormente.
	Postcondion: Devolvera la posicion futura (nueva posicion que adoptara) de acorde al caracter ingresado pero sin verificar que sean posiciones no validas(hay paredes).
*/
coordenada_t posicion_futura(coordenada_t posicion,char direccion){
	int movimiento_vertical = 0;
	int movimiento_horizontal = 0;
	coordenada_t posicion_futura;

	switch(direccion){
		case ARRIBA:
			movimiento_vertical = -1;
			break;
		case ABAJO:
			movimiento_vertical = 1;
			break;
		case DERECHA:
			movimiento_horizontal = 1;
			break;
		case IZQUIERDA:
			movimiento_horizontal = -1;
			break;
		default:
			break;
	}
	posicion_futura.fil = posicion.fil + movimiento_vertical;
	posicion_futura.col = posicion.col + movimiento_horizontal;
	return posicion_futura;
}
/*
	Precondicion: Recibe un caracter(no otro tipo de dato) pudiendo ser w,a s o d en minuscula.
	Postcondion: Devolvera falso en caso de que la tecla sea incorrecta o que el movimiento no se pueda realizar
				(hay una pared o el movimiento se encuentra fuera de la matriz),devolvera verdadero en caso contrario.
*/
bool es_movimiento_valido(juego_t* juego, char tecla){
	bool movimiento_valido = true;
	coordenada_t nueva_posicion;

	if( (tecla != ARRIBA) && (tecla != ABAJO) && (tecla != DERECHA) && (tecla != IZQUIERDA) )
		movimiento_valido = false;
	else{
		nueva_posicion = posicion_futura((*juego).jugador.posicion,tecla);
		if(nueva_posicion.fil >= TAMANIO || nueva_posicion.col >= TAMANIO || nueva_posicion.fil < 0 || nueva_posicion.col < 0)
			movimiento_valido = false;
		else if( elemento_posicion((*juego).laberinto_original,nueva_posicion) == PARED)
			movimiento_valido = false;
	}

	return movimiento_valido;
}
/*
	Precondicion: Recibe un caracter(no otro tipo de dato) y ya debio estar verificada anteriormente(w a s o d en minuscula).
	Postcondion: La funcion modificara la posicion del jugador cuya posicion es valida(no hay pared).
*/
void mover_jugador(jugador_t* jugador, char direccion){

	switch(direccion){
		case ARRIBA:
			(*jugador).posicion.fil = (*jugador).posicion.fil - 1;
			break;
		case ABAJO:
			(*jugador).posicion.fil = (*jugador).posicion.fil + 1;
			break;
		case DERECHA:
			(*jugador).posicion.col = (*jugador).posicion.col + 1;
			break;
		case IZQUIERDA:
			(*jugador).posicion.col = (*jugador).posicion.col - 1;
			break;	
	}
}
/*
	Precondicion: Recibe un caracter(no otro tipo de dato) y ya debio estar verificada anteriormente.
	Postcondion: La funcion modificara la direccion del rival segun la direccion ingresada.
*/
void cambiar_direccion_rival(char *direccion){

	switch((*direccion)){
		case ABAJO:
			(*direccion) = IZQUIERDA;
			break;
		case DERECHA:
			(*direccion) = ABAJO;
			break;
		case IZQUIERDA:
			(*direccion) = ARRIBA;
			break;	
		case ARRIBA:
		default:
			(*direccion) = DERECHA;
			break;	
	}
}
/*
	Postcondion: La funcion modificara la posicion del rival segun su direccion actual y la cantidad de pasos en esa direccion.
				Cambiara de direccion en caso de de quedarse sin pasos en esa direccion renovando la cantidad de pasos en la nueva direccion.
*/
void mover_rival(juego_t* juego){
	bool movimiento_valido;
	coordenada_t nueva_posicion;

	do{
		movimiento_valido = true;

		if((*juego).rival.cantidad_pasos == CANT_PASOS){
			cambiar_direccion_rival(&((*juego).rival.direccion));
			(*juego).rival.cantidad_pasos = 0;
		}

		nueva_posicion = posicion_futura((*juego).rival.posicion,(*juego).rival.direccion);
		if(nueva_posicion.fil >= TAMANIO || nueva_posicion.col >= TAMANIO || nueva_posicion.fil < 0 || nueva_posicion.col < 0)
			movimiento_valido = false;
		else if( elemento_posicion((*juego).laberinto_original,nueva_posicion) == PARED)
			movimiento_valido = false;

		(*juego).rival.cantidad_pasos++;	
	}while(!movimiento_valido);

	(*juego).rival.posicion = nueva_posicion;
}


/*
	Precondicion: Recibe un caracter(no otro tipo de dato) y ya debio estar verificada anteriormente(pudiendo ser I R P o F en mayuscula).
	Postcondion: Devolvera verdadero en caso de que el vector ayudas del jugador contega la ayuda o falso en caso contrario.
*/
bool jugador_tiene_ayuda(jugador_t jugador,char ayuda_necesaria){
	bool esta = false;
	int i = 0;

	while((i < jugador.tope_ayudas) && !esta){
		if(jugador.ayudas[i].codigo == ayuda_necesaria)
			esta = true;
		i++;
	}
	return esta;
}
/*
	Precondicion: Recibe un obstaculo(no otro tipo de dato) y ya debio estar verificada anteriormente(pudiendo ser E A o B en mayuscula).
	Postcondion: Devolvera verdadero en caso de que el vector ayudas del jugador contega la ayuda contra ese obstaculo en especifico o falso en caso contrario.
				Tener en cuenta que hay obstaculos que no contienen ayudas que lo contraresten (ACROMANTULA).
*/
bool tiene_ayuda_necesaria_obstaculo(jugador_t jugador,obstaculo_t obstaculo){
	bool tiene_ayuda = false;
    char ayuda_necesaria;

	switch(obstaculo.codigo){
		case ESCREGUTO:
			ayuda_necesaria = IMPEDIMENTA;
			break;
		case BOGGART:
			ayuda_necesaria = RIDDIKULUS;
			break;
		case ACROMANTULA:
		default:
			ayuda_necesaria = VACIO;
			break;
	}
	if(ayuda_necesaria != VACIO)
		tiene_ayuda = jugador_tiene_ayuda(jugador,ayuda_necesaria);

	return tiene_ayuda;
}
/*
	Precondicion: Recibe una ubicacion del vector la cual ya debio de esta verificada anteriormente(no puede ser negativa ni mayor a TOTAL_OBSTACULOS).
	Postcondion: La funcion eliminara el obstaculo ,de esa ubicacion del vector, del laberinto y no dejara ordenado el vector.
*/
void eliminar_obstaculo(obstaculo_t obstaculos[TOTAL_OBSTACULOS],int *tope_obstaculos,int ubicacion){
	//como no esta ordenado simplemente cambio el eliminado por el ultimo
	obstaculos[ubicacion] = obstaculos[(*tope_obstaculos) - 1];
	(*tope_obstaculos)--;
}
/*
	Precondicion: Recibe una ubicacion del vector la cual ya debio de esta verificada anteriormente(no puede ser negativa ni mayor a TOTAL_AYUDAS).
	Postcondion: La funcion eliminara la ayuda ,de esa ubicacion del vector, del laberinto y no dejara ordenado el vector.
*/
void eliminar_ayuda(ayuda_t ayudas[TOTAL_AYUDAS],int *tope_ayudas,int ubicacion){
	//como no esta ordenado simplemente cambio el eliminado por el ultimo
	ayudas[ubicacion] = ayudas[(*tope_ayudas) - 1];
	(*tope_ayudas)--;
}
/*
	Precondicion: Recibe una ayuda(no otro tipo de dato) y ya debio estar verificada anteriormente.
	Postcondion: La funcion modificara el vector de ayudas del jugador, añadiendo la ayuda si no es de accion inmediata al vector en caso de que haya espacio para esta.
*/
void agregar_ayuda_jugador(jugador_t *jugador,ayuda_t ayuda){

	if( (*jugador).tope_ayudas < TOTAL_AYUDAS && ayuda.codigo != POCION){
		(*jugador).ayudas[(*jugador).tope_ayudas] = ayuda;
		(*jugador).tope_ayudas++;
	}
}
/*
	Precondicion: Recibe una posicion la cual ya debio de esta verificada anteriormente.
	Postcondion: Devolvera la ubicacion del vector obstaculos con la misma posicion ingresada, es decir el obstaculo que tenga la misma posicion que la ingresada.
				En caso de no haber obstaculo con esa posicion devolvera -1 indicando que no se encontro.
*/
int hay_obstaculo_posicion(obstaculo_t obstaculos[TOTAL_OBSTACULOS],int tope_obstaculos,coordenada_t posicion){

	bool hay_obstaculo = false;
	int i = 0;
	int ubicacion = -1;

	while(i < tope_obstaculos && !hay_obstaculo){
		if(misma_posicion(obstaculos[i].posicion,posicion)){
			hay_obstaculo = true;
			ubicacion = i;
		}
		i++;
	}

	return ubicacion;
}
/*
	Precondicion: Recibe una posicion la cual ya debio de esta verificada anteriormente.
	Postcondion: Devolvera la ubicacion del vector ayudas con la misma posicion ingresada, es decir la ayuda que tenga la misma posicion que la ingresada.
				En caso de no haber ayuda con esa posicion devolvera -1 indicando que no se encontro.
*/
int hay_ayuda_posicion(ayuda_t ayudas[TOTAL_AYUDAS],int tope_ayudas,coordenada_t posicion){
	bool hay_ayuda = false;
	int i = 0;
	int ubicacion = -1;

	while(i < tope_ayudas && !hay_ayuda){
		if(misma_posicion(ayudas[i].posicion,posicion)){
			hay_ayuda = true;
			ubicacion = i;
		}
		i++;
	}

	return ubicacion;
}
/*
	Postcondion: La funcion eliminara la ayuda que este sobre el jugador(en caso de haber) realizando todos sus efectos.
				Hara lo mismo con un obstaculo(en caso de haber) y realizando todos sus efectos contando si tiene las ayudas necesarias para contrarestarlos.
				Actualizara todos los datos que se puedan producir durante un turno. 
*/
void actualizar_juego(juego_t* juego){

	int ubicacion;
	int danio = 0;

	(*juego).jugador.vida -= VIDA_MOVIMIENTO;

	if((*juego).jugador.vida > 0){

		ubicacion = hay_ayuda_posicion((*juego).ayudas,(*juego).tope_ayudas,(*juego).jugador.posicion);
		if(ubicacion >= 0){
			(*juego).jugador.vida += (*juego).ayudas[ubicacion].vida_a_recuperar;
			if((*juego).jugador.vida > VIDA_MAXIMA)
				(*juego).jugador.vida = VIDA_MAXIMA;
			agregar_ayuda_jugador(&((*juego).jugador),(*juego).ayudas[ubicacion]);
			eliminar_ayuda((*juego).ayudas,&((*juego).tope_ayudas),ubicacion);
		}
		else{
			ubicacion = hay_obstaculo_posicion((*juego).obstaculos,(*juego).tope_obstaculos,(*juego).jugador.posicion);
			if(ubicacion >= 0){
				danio = (*juego).obstaculos[ubicacion].danio;
				if(tiene_ayuda_necesaria_obstaculo((*juego).jugador,(*juego).obstaculos[ubicacion]))
					danio = 0;
				(*juego).jugador.vida -= danio;
				eliminar_obstaculo((*juego).obstaculos,&((*juego).tope_obstaculos),ubicacion);
			}
		}
	}
}
/*
	Postcondion: Devolvera un entero representando: 1(GANADO) cuando el jugador consiguio la copa, 0(EN CURSO) sigue el juego y -1(PERDIDO) cuando el rival consiguio la copa.
*/
int estado_juego(juego_t juego){
	int estado;

	if(juego.jugador.vida <= 0)
		estado = PERDIDO;
	else if(misma_posicion(juego.rival.posicion,juego.copa.posicion))
		estado = PERDIDO;
	else if(misma_posicion(juego.jugador.posicion,juego.copa.posicion))
		estado = GANADO;
	else
		estado = EN_CURSO;

	return estado;
}



/*
	Postcondion: La funcion modificara el color de escritura segun el caracter recibido, por ejemplo: la copa (C) se imprimira con un color amarillo , etc.
*/
void seleccionar_color(char elemento){
	switch(elemento){
		case COPA:
			printf("\033[1;33m");//color amarillos
			break;
		case JUGADOR:
			printf("\033[1;36m");//color cyan
			break;
		case POCION:
			printf("\033[1;32m");//color verde
			break;
		case BOGGART:
		case ESCREGUTO:
		case ACROMANTULA:
			printf("\033[1;31m");//color rojo
			break;
		case RIVAL:
		case IMPEDIMENTA:
		case RIDDIKULUS:
		case ESFINGE:
		default:
			printf("\033[0;0m");//color neutro
			break;

	}
}
/*
	Precondicion: La matriz ingresada ya debio de estar verificada anteriormente (debe tener el mismo tamaño que el laberinto original (TAMANIOxTAMANIO))
	Postcondion: La funcion modificara la matriz ingresada por los valores del laberinto original,quedando asi con los pasillos y paredes.
*/
void dibujar_laberinto(juego_t juego,char laberinto[TAMANIO][TAMANIO]){
	for(int i = 0;i < TAMANIO;i++){
		for(int k = 0;k < TAMANIO;k++){
			laberinto[i][k] = juego.laberinto_original[i][k];
		}
	}
}
/*
	Precondicion: La matriz ingresada ya debio de estar verificada anteriormente (debe tener el mismo tamaño que el laberinto original (TAMANIOxTAMANIO))
	Postcondion: La funcion modificara la matriz ingresada por los valores de las posiciones de los elementos presentes en el juego.
*/
void dibujar_elementos(juego_t juego,char laberinto[TAMANIO][TAMANIO]){
	int i = 0;
	if(jugador_tiene_ayuda(juego.jugador,ESFINGE) || juego.jugador.vida <= 15){
		laberinto[juego.copa.posicion.fil][juego.copa.posicion.col] = juego.copa.codigo;
	}
	for(i = 0; i < juego.tope_obstaculos;i++){
		laberinto[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].codigo;
	}
	for(i = 0; i < juego.tope_ayudas;i++){
		laberinto[juego.ayudas[i].posicion.fil][juego.ayudas[i].posicion.col] = juego.ayudas[i].codigo;
	}
	laberinto[juego.rival.posicion.fil][juego.rival.posicion.col] = juego.rival.codigo;
	laberinto[juego.jugador.posicion.fil][juego.jugador.posicion.col] = juego.jugador.codigo;
}
/*
	Precondicion: La matriz ingresada debe tener el mismo tamaño que el laberinto original (TAMANIOxTAMANIO)
	Postcondion: La funcion actualizara la matriz mostrada al usario.
*/
void actualizar_laberinto(juego_t juego, char laberinto[TAMANIO][TAMANIO]){ //laberinto esta pasado por referencia ojo
	system("clear");

	dibujar_laberinto(juego,laberinto);
	dibujar_elementos(juego,laberinto);
}
/*
	Precondicion: La matriz ingresada debe tener el mismo tamaño que el laberinto original (TAMANIOxTAMANIO)
	Postcondion: La funcion mostrara por pantalla la matriz ingresada.
*/
void mostrar_laberinto(char laberinto[TAMANIO][TAMANIO]){
	printf("\033[0;0m");
	for(int fila = 0;fila < TAMANIO;fila++){
		printf("                                ");
		for(int columna = 0;columna < TAMANIO;columna++){
			seleccionar_color(laberinto[fila][columna]);
			printf(" %c", laberinto[fila][columna]);
		}
		printf("\n");
	}	
}
