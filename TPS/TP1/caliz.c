#include <stdio.h>
#include <stdbool.h>

//CONSTANTES
/*---------------------------------------*/
const int EDAD_MINIMA = 17;
const int INTELIGENCIA_MINIMA = 0;
const int INTELIGENCIA_MAXIMA = 10;
const int FUERZA_MINIMA = 0;
const int FUERZA_MAXIMA = 10;
const char CAPACIDAD_BUENA = 'B';
const char CAPACIDAD_REGULAR = 'R';
const char CAPACIDAD_MALA = 'M';

const char NO = 'N';
/*---------------------------------------*/

//FUNCIONES
/*---------------------------------------*/
bool no_en_rango(int valor,int minimo,int maximo){
	//Precondicion:Recibe tres nuneros enteros(no otro tipo de dato) donde "maximo" > "minimo".
	//Postcondicion:Devuelve verdadero en caso de que "minimo" < "valor" o "valor" > "maximo",devolvera false en caso contrario.

	return( (valor > maximo) || (valor < minimo) );
}
bool caracter_incorrecto(char valor){
	//Precondicion:Recibe un caracter(no otro tipo de dato) y debe de estar en mayuscula pudiendo ser B,R o M.
	//Postcondicion:Devuleve verdadero en caso de que "valor" no sea B,R y M, o falso en caso de que sea alguna de estas.

	return ((valor != CAPACIDAD_BUENA) && (valor != CAPACIDAD_REGULAR) && (valor != CAPACIDAD_MALA));
}
void pedir_edad(int *edad){
	//Precondicion:Recibe un numero entero(no otro tipo de dato).
	//Postcondicion:La funcion modificara el valor de "edad" la cual sera un numero entero(no otro tipo de dato) y no podra ser negativa.

	printf("\033[0m");
	printf("Ingrese la edad del alumno (%i años o mayor): ",EDAD_MINIMA);
	scanf("%i",edad);
} 
void pedir_inteligencia(int *inteligencia){
	//Precondicion:Recibe un numero entero(no otro tipo de dato).
	//Postcondicion:La funcion modificara el valor de "inteligencia" la cual sera un numero entero(y no otro tipo de dato) y debera estar en el rango estipulado.
	
	do{
		printf("\033[0m");
		printf("Ingrese la inteligencia del alumno entre %i y %i (inclusives): ",INTELIGENCIA_MINIMA,INTELIGENCIA_MAXIMA);
		scanf("%i",inteligencia);
		if(no_en_rango(*inteligencia,INTELIGENCIA_MINIMA,INTELIGENCIA_MAXIMA)){
			printf("\033[1;31m"); 
			printf("La inteligencia no se encuentra en el rango estipulado.Intente nuevamente.\n");
		}
	}while(no_en_rango(*inteligencia,INTELIGENCIA_MINIMA,INTELIGENCIA_MAXIMA));
}
void pedir_fuerza(int *fuerza){
	//Precondicion:Recibe un numero entero(no otro tipo de dato).
	//Postcondicion:La funcion modificara el valor de "fuerza" la cual sera un numero entero(y no otro tipo de dato) y debera estar en el rango estipulado.

	do{
		printf("\033[0m");
		printf("Ingrese la fuerza del alumno entre %i y %i (inclusives): ",FUERZA_MINIMA,FUERZA_MAXIMA);
		scanf("%i",fuerza);
		if(no_en_rango(*fuerza,FUERZA_MINIMA,FUERZA_MAXIMA)){
			printf("\033[1;31m"); 
			printf("La fuerza no se encuentra en el rango estipulado.Intente nuevamente.\n");
		}
	}while(no_en_rango(*fuerza,FUERZA_MINIMA,FUERZA_MAXIMA));
}
void pedir_capacidad_magica(char *capacidad_magica){
	//Precondicion:Recibe un caracter(no otro tipo de dato).
	//Postcondicion:La funcion modificara el valor de "capacidad_magica" la cual sera un caracter(y no otro tipo de dato) y debera estar en el rango estipulado.

	do{
		printf("\033[0m");
		printf("Ingrese la capacidad magica del alumno ((%c) buena,(%c) regular,(%c) mala): ",CAPACIDAD_BUENA,CAPACIDAD_REGULAR,CAPACIDAD_MALA);
		scanf(" %c",capacidad_magica);
		if(caracter_incorrecto(*capacidad_magica)){
			printf("\033[1;31m"); 
			printf("La letra es incorrecta.Intente nuevamente.\n");
		}
	}while(caracter_incorrecto(*capacidad_magica));
}
bool nuevo_campeon(int inteligencia,int fuerza,char capacidad_magica,int inteligencia_campeon,int fuerza_campeon,char capacidad_magica_campeon,int numero_campeon){
	/*Precondicion:Recibe  
				-inteligencia,fuerza,inteligencia_campeon,fuerza_campeon,numero_campeon:Numeros enteros(no otro tipo de dato) y ya debieron estar verificados anteriormente.
				-capacidad_magica,capacidad_magica_campeon:Caracteres(no otro tipo de dato).
	*/
	//Postcondicion:Devolvera verdadero en caso de que haya un nuevo campeon y falso en caso contrario sin modificar los parametros de entrada en ambos casos.

	bool condicion = false;

	if(numero_campeon == 0) //es el primero que se esta ingresando correctamente
		condicion = true;

	else if(inteligencia > inteligencia_campeon)
		condicion = true;

	else if(inteligencia == inteligencia_campeon){
		if(fuerza > fuerza_campeon)
			condicion = true;

		else if(fuerza == fuerza_campeon){
			if(capacidad_magica != capacidad_magica_campeon){
				if(capacidad_magica == CAPACIDAD_BUENA)
					condicion = true;

				else if((capacidad_magica == CAPACIDAD_REGULAR) && (capacidad_magica_campeon == CAPACIDAD_MALA))
					condicion = true;
			}
		}
	}
    return condicion;  
}
void actualizar_campeon(int inteligencia,int fuerza,char capacidad_magica,int *inteligencia_campeon,int *fuerza_campeon,char *capacidad_magica_campeon,int *numero_campeon,int contador){
	/*Precondicion:Recibe  
				-inteligencia,fuerza,inteligencia_campeon,fuerza_campeon,numero_campeon,contador:Numeros enteros(no otro tipo de dato).
				-capacidad_magica,capacidad_magica_campeon:Caracteres(no otro tipo de dato).
	*/
	//Postcondicion:Modificara los datos del campeon por los datos del usario .
	*inteligencia_campeon = inteligencia;
	*fuerza_campeon = fuerza;
	*capacidad_magica_campeon = capacidad_magica;
	*numero_campeon = contador;
}
void mostrar_campeon(int numero){
	//Precondicion:Recibe un numero entero(no otro tipo de dato), el cual debe ser mayor o igual a 0.

	if(numero == 0)
		printf("No hay campeón :(\n");
	else
		printf("El campeón es el número %i :)\n",numero);
}
/*---------------------------------------*/

int main(void){

	//Datos del usuario
	int edad,inteligencia,fuerza;
	char capacidad_magica;
	int contador_alumnos = 0;
	
	//Datos del campeon
	int inteligencia_campeon = INTELIGENCIA_MINIMA,fuerza_campeon = FUERZA_MINIMA;
	char capacidad_magica_campeon = CAPACIDAD_MALA;
	int numero_campeon = 0;

	char continuar_ingresos;
	printf("Desea ingresar un alumno? (N para ​ NO​ , cualquier otra tecla para sí): ");
	scanf(" %c",&continuar_ingresos);
	while(continuar_ingresos != NO){
		contador_alumnos++;
		pedir_edad(&edad);
		if(edad >= EDAD_MINIMA){
			pedir_inteligencia(&inteligencia);
    		pedir_fuerza(&fuerza);
    		pedir_capacidad_magica(&capacidad_magica);	
    		printf("\033[1;32m"); 
    		printf("Datos del alumno ingresados correctamente.\n");
    		if(nuevo_campeon(inteligencia,fuerza,capacidad_magica,inteligencia_campeon,fuerza_campeon,capacidad_magica_campeon,numero_campeon))
    			actualizar_campeon(inteligencia,fuerza,capacidad_magica,&inteligencia_campeon,&fuerza_campeon,&capacidad_magica_campeon,&numero_campeon,contador_alumnos);
		}
		else{
			printf("\033[1;31m"); 
			printf("El alumno tiene menos de %i años por ende no puede participar.\n",EDAD_MINIMA);
		}
		printf("\033[0m");
    	printf("Desea ingresar otro alumno? (N para ​ NO​ , cualquier otra tecla para sí): ");
    	scanf(" %c",&continuar_ingresos);
	}
	mostrar_campeon(numero_campeon);

	return 0;
}