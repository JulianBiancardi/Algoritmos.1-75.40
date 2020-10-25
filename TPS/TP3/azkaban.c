#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prision.h"

int main(int argc,char *argv[]){

	if(argc > 1){
		if((strcmp(argv[1],"perdonables") == 0) && (argc == 3)){
			if(generar_presos_perdonables(argv[2]) == ERROR){
				printf("\033[1;31m");//color rojo
				printf("-Error al intentar generar los presos perdonables.\n");
			}
			else{
				printf("\033[1;32m");//color verde
				printf("-Los presos perdonables se han generado con exito.\n");
			}
		}
		else if((strcmp(argv[1],"liberar") == 0) && (argc == 5)){
			if(generar_presos_liberables(argv[2],argv[3],*argv[4]) == ERROR){
				printf("\033[1;31m");//color rojo
				printf("-Error al intentar generar los presos liberables.\n");
			}
			else{
				printf("\033[1;32m");//color verde
				printf("-Los presos liberables se han generado con exito.\n");
			}
		}
		else if((strcmp(argv[1],"actualizar") == 0) && (argc == 3)){
			if(actualizar(argv[2]) == ERROR){
				printf("\033[1;31m");//color rojo
				printf("-Error al intentar actualizar los archivos.\n");
			}
			else{
				printf("\033[1;32m");//color verde
				printf("-Los archivos de los presos han sido actualizado con exito.\n");
			}
		}
		else if((strcmp(argv[1],"mostrar_liberados") == 0) && (argc == 3)){
			mostrar_liberados(argv[2]); 
		}
		else{
			mostrar_ayudas();
		}
	}
	else{
		mostrar_ayudas();
	}

	printf("\033[0;0m");//color neutro
	
	return 0;
}