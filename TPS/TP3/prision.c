#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "prision.h"
/*---------------------------------------*/
//CONSTANTES
#define BUENA 'B'
#define REGULAR 'R'
#define MALA 'M'

#define ESCRITURA "w"
#define LECTURA "r"
#define PRISIONEROS_CRUCIO "crucio.dat"
#define PRISIONEROS_IMPERIUS "imperius.dat"
#define ARCHIVO_AUXILIAR "auxiliar_nuevo.dat"
#define ARCHIVO_AUXILIAR_2 "auxiliar_nuevo2.dat"
const int MAX_NOMBRE_ARCHIVO = 200;
#define NOMBRE_LIBERANLES_DEFECTO "liberados_"
#define EXTENSION_TEXTO ".txt"
#define FORMATO_LECTURA "%[^\n]\n"

#define COMANDO_PERDONABLES "perdonables <nombre del archivo> "
#define COMANDO_LIBERAR "liberar <nombre del archivo> fecha conducta "
#define COMANDO_ACTUALIZAR "actualizar fecha"
#define COMANDO_MOSTRAR "mostrar_liberados <nombre del archivo>"
#define COMANDO_AYUDA "ayuda"
/*---------------------------------------*/


/*
	Precondicion: Recibe un nombre de archivo(la cantidad de caracteres no puede exeder MAX_NOMBRE_ARCHIVO) 
					y una fecha de liberacion con formato aaaamm(donde 0000 < aaaa <= 9999 y 01 <= mm <= 12).
	Postcondion: La funcion modificara el nombre_archivo con el siguiente formato "liberados_fechaliberacion.txt".
*/
void generar_nombre_liberables(char nombre_archivo[MAX_NOMBRE_ARCHIVO],char fecha_liberacion[MAX_FECHA]){

	nombre_archivo[0] = '\0';
	strcat(nombre_archivo,NOMBRE_LIBERANLES_DEFECTO);
	strcat(nombre_archivo,fecha_liberacion);
	strcat(nombre_archivo,EXTENSION_TEXTO);
}

/*
	Postcondion: La funcion generara un archivo con el nombre ingresado, que contendra los presos que hayan realizado solo 1 maldicion
				mediante dos archivos binarios crucio y imperius.Devolvera OK si pudo generar correctamente el archivo perdonables,
				devolvera ERROR en caso contrario mostrando por pantalla un error informandole al usuario lo sucedido.
*/
int generar_presos_perdonables(char nombre_archivo[]){

	FILE* prisioneros_crucio = fopen(PRISIONEROS_CRUCIO,LECTURA);
	if(prisioneros_crucio == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",PRISIONEROS_CRUCIO);
		return ERROR;
	}
	FILE* prisioneros_imperius = fopen(PRISIONEROS_IMPERIUS,LECTURA);
	if(prisioneros_imperius == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",PRISIONEROS_IMPERIUS);
		fclose(prisioneros_crucio);
		return ERROR;
	}
	FILE* perdonables = fopen(nombre_archivo,ESCRITURA);
	if(perdonables == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",nombre_archivo);
		fclose(prisioneros_crucio);
		fclose(prisioneros_imperius);
		return ERROR;
	}

	preso_t preso_crucio;
	preso_t preso_imperius;
	bool pude_escribir = true;
	int estado_operacion = OK;

	size_t leido_crucio = fread(&preso_crucio,sizeof(preso_t),1,prisioneros_crucio);
	size_t leido_imperius = fread(&preso_imperius,sizeof(preso_t),1,prisioneros_imperius);


	while((leido_crucio > 0) && (leido_imperius > 0) && pude_escribir){
		if(strcmp(preso_crucio.nombre,preso_imperius.nombre) < 0){
			pude_escribir = fwrite(&preso_crucio,sizeof(preso_t),1,perdonables);
			if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
			leido_crucio = fread(&preso_crucio,sizeof(preso_t),1,prisioneros_crucio);
		}
		else if(strcmp(preso_crucio.nombre,preso_imperius.nombre) > 0){
			pude_escribir = fwrite(&preso_imperius,sizeof(preso_t),1,perdonables);
			if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
			leido_imperius = fread(&preso_imperius,sizeof(preso_t),1,prisioneros_imperius);
		}
		else{
			leido_crucio = fread(&preso_crucio,sizeof(preso_t),1,prisioneros_crucio);
			leido_imperius = fread(&preso_imperius,sizeof(preso_t),1,prisioneros_imperius);
		}
	}
	while((leido_crucio > 0) && pude_escribir){
		pude_escribir = fwrite(&preso_crucio,sizeof(preso_t),1,perdonables);
		if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
		leido_crucio = fread(&preso_crucio,sizeof(preso_t),1,prisioneros_crucio);
	}
	while((leido_imperius > 0) && pude_escribir){
		pude_escribir = fwrite(&preso_imperius,sizeof(preso_t),1,perdonables);
		if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
		leido_imperius = fread(&preso_imperius,sizeof(preso_t),1,prisioneros_imperius);
	}

	fclose(prisioneros_crucio);
	fclose(prisioneros_imperius);
	fclose(perdonables);

	return estado_operacion;
}

/*
	Precondicion: Recibe una fecha de liberacion y una conducta las cuales ya debieron estar verificadas anteriormente.
	Postcondion: Devolvera verdadero en caso de que el prisionero cumpla con los requisitos para salir,devolvera falso en caso contrario.
*/
bool puede_ser_liberado(preso_t preso,char fecha_liberacion[MAX_FECHA],char conducta){

	bool condicion = false;

	if((strcmp(preso.fecha,fecha_liberacion) == 0) || (strcmp(preso.fecha,fecha_liberacion) < 0)){
		switch(preso.conducta){
			case BUENA:
				condicion = true;
				break;
			case REGULAR:
				if(conducta == REGULAR || conducta == MALA)
					condicion = true;
				break;
			case MALA:
				if(conducta == MALA)
					condicion = true;
				break;
		}
	}

	return condicion;
}
/*
	Postcondion: Devolvera verdadero en caso de que exista un archivo con el nombre ingresado,devolvera falso en caso contrario.
*/
bool archivo_existe(char nombre_archivo[]){
	bool existe = false;

	FILE* archivo = fopen(nombre_archivo,LECTURA);
	if(archivo != NULL){
		existe = true;
		fclose(archivo);
	}

	return existe;
}
/*
	Precondicion: Recibe una fecha de liberacion con formato aaaamm(donde 0000 < aaaa <= 9999 y 01 <= mm <= 12) 
					y una conducta (caracter y no otro tipo de dato) pudiendo ser B,R o M en mayuscula. 
	Postcondion: La funcion generara un archivo con el nombre ingresado,que contendra los presos que pueden ser liberados compliendo con las condicines
				 mediante dos archivos binarios crucio y imperius.Devolvera OK si pudo actualizar correctamente ambos archivos,
				 devolvera ERROR en caso contrario mostrando por pantalla un error informandole al usuario lo sucedido.
*/
int generar_presos_liberables(char nombre_archivo[],char fecha_liberacion[MAX_FECHA],char conducta){

	char nombre_archivo_liberables[MAX_NOMBRE_ARCHIVO];
	generar_nombre_liberables(nombre_archivo_liberables,fecha_liberacion);

	FILE* archivo_perdonables = fopen(nombre_archivo,LECTURA);
	if(archivo_perdonables == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",nombre_archivo_liberables);
		return ERROR;
	}
	if(archivo_existe(nombre_archivo_liberables)){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) ya existe.\n",nombre_archivo_liberables);
		fclose(archivo_perdonables);
		return ERROR;
	}
	FILE* archivo_liberables = fopen(nombre_archivo_liberables,ESCRITURA);
	if(archivo_liberables == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",nombre_archivo_liberables);
		fclose(archivo_perdonables);
		return ERROR;
	}

	preso_t preso;
	bool pude_escribir = true;
	int estado_operacion = OK;

	size_t leido = fread(&preso,sizeof(preso_t),1,archivo_perdonables);
	
	while((leido != 0) && pude_escribir){
		if(puede_ser_liberado(preso,fecha_liberacion,conducta)){
			pude_escribir = fprintf(archivo_liberables,"%s\n",preso.nombre);
			if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
		}
		leido = fread(&preso,sizeof(preso_t),1,archivo_perdonables);
	}

	fclose(archivo_perdonables);
	fclose(archivo_liberables);

	return estado_operacion;
}


/* 
	Precondicion: Recibe 5 archivos,3 de ellos seran para la diferencia(2 a 1) y 2 de ellos seran los nuevos archivos que contendran el resultado.
				Todos estos archivos los cuales ya debieron estar verificados anteriormente.
	Postcondion: Devolvera OK si pudo generar un nuevo archivo crucio,que contendra los presos de crucio a ecepcion de los que pueden ser liberados,
				y un nuevo archivo imperius,que contendra los presos de imperius a ecepcion de los que pueden ser liberados.Devolvera ERROR en caso contrario
				mostrando por pantalla un error informandole al usuario lo ocurrido.
*/
int diferencia_tres_archivos(FILE* primer_archivo,FILE* segundo_archivo,FILE* archivo_liberables,FILE* nuevo_primer_archivo,FILE* nuevo_segndo_archivo){

	preso_t preso_crucio;
	preso_t preso_imperius;
	char nombre_liberable[MAX_NOMBRE];
	bool pude_escribir = true;
	int estado_operacion = OK;

	size_t leido_primer_presos = fread(&preso_crucio,sizeof(preso_t),1,primer_archivo);
	size_t leido_segundo_presos = fread(&preso_imperius,sizeof(preso_t),1,segundo_archivo);
	int leido_liberables = fscanf(archivo_liberables,FORMATO_LECTURA,nombre_liberable);

	while((leido_primer_presos > 0)  && (leido_segundo_presos > 0) && (leido_liberables > 0) && pude_escribir){
		if(strcmp(preso_crucio.nombre,nombre_liberable) == 0){
			leido_primer_presos = fread(&preso_crucio,sizeof(preso_t),1,primer_archivo);
			leido_liberables = fscanf(archivo_liberables,FORMATO_LECTURA,nombre_liberable);
		}
		else if(strcmp(preso_imperius.nombre,nombre_liberable) == 0){
			leido_segundo_presos = fread(&preso_imperius,sizeof(preso_t),1,segundo_archivo);
			leido_liberables = fscanf(archivo_liberables,FORMATO_LECTURA,nombre_liberable);
		}
		else if(strcmp(preso_crucio.nombre,nombre_liberable) < 0){
			pude_escribir = fwrite(&preso_crucio,sizeof(preso_t),1,nuevo_primer_archivo);
			if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
			leido_primer_presos = fread(&preso_crucio,sizeof(preso_t),1,primer_archivo);

			if(strcmp(preso_imperius.nombre,nombre_liberable) < 0){
				pude_escribir = fwrite(&preso_imperius,sizeof(preso_t),1,nuevo_segndo_archivo);
				if(!pude_escribir){
					printf("\033[1;31m");//color rojo
					printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
					estado_operacion = ERROR;
				}
				leido_segundo_presos = fread(&preso_imperius,sizeof(preso_t),1,segundo_archivo);
			}	
		}
		else{
			if(strcmp(preso_imperius.nombre,nombre_liberable) < 0){
				pude_escribir = fwrite(&preso_imperius,sizeof(preso_t),1,nuevo_segndo_archivo);
				if(!pude_escribir){
					printf("\033[1;31m");//color rojo
					printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
					estado_operacion = ERROR;
				}
				leido_segundo_presos = fread(&preso_imperius,sizeof(preso_t),1,segundo_archivo);
			}
		}
	}

	while((leido_primer_presos > 0) && (leido_liberables > 0) && pude_escribir){
		if(strcmp(preso_crucio.nombre,nombre_liberable) == 0){
			leido_primer_presos = fread(&preso_crucio,sizeof(preso_t),1,primer_archivo);
			leido_liberables = fscanf(archivo_liberables,FORMATO_LECTURA,nombre_liberable);
		}
		else if(strcmp(preso_crucio.nombre,nombre_liberable) < 0){
			pude_escribir = fwrite(&preso_crucio,sizeof(preso_t),1,nuevo_primer_archivo);
			if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
			leido_primer_presos = fread(&preso_crucio,sizeof(preso_t),1,primer_archivo);	
		}
		else{
			leido_liberables = fscanf(archivo_liberables,FORMATO_LECTURA,nombre_liberable);
		}
	}
	while((leido_primer_presos > 0) && pude_escribir){
		pude_escribir = fwrite(&preso_crucio,sizeof(preso_t),1,nuevo_primer_archivo);
		if(!pude_escribir){
			printf("\033[1;31m");//color rojo
			printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o es de acceso restringido.\n");
			estado_operacion = ERROR;
		}
		else
			leido_primer_presos = fread(&preso_crucio,sizeof(preso_t),1,primer_archivo);
	}

	while((leido_segundo_presos > 0) && (leido_liberables > 0) && pude_escribir){
		if(strcmp(preso_imperius.nombre,nombre_liberable) == 0){
			leido_segundo_presos = fread(&preso_imperius,sizeof(preso_t),1,segundo_archivo);
			leido_liberables = fscanf(archivo_liberables,FORMATO_LECTURA,nombre_liberable);
		}
		else if(strcmp(preso_imperius.nombre,nombre_liberable) < 0){
			pude_escribir = fwrite(&preso_imperius,sizeof(preso_t),1,nuevo_segndo_archivo);
			if(!pude_escribir){
				printf("\033[1;31m");//color rojo
				printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o el archivo es de acceso restringido.\n");
				estado_operacion = ERROR;
			}
			leido_segundo_presos = fread(&preso_imperius,sizeof(preso_t),1,segundo_archivo);
		}
		else{
			leido_liberables = fscanf(archivo_liberables,FORMATO_LECTURA,nombre_liberable);
		}
	}
	while((leido_segundo_presos > 0) && pude_escribir){
		pude_escribir = fwrite(&preso_imperius,sizeof(preso_t),1,nuevo_segndo_archivo);
		if(!pude_escribir){
			printf("\033[1;31m");//color rojo
			printf("Tuvimos problemas al intentar escribir.Puede que no tenga espacio o es de acceso restringido.\n");
			estado_operacion = ERROR;
		}
		else
			leido_segundo_presos = fread(&preso_imperius,sizeof(preso_t),1,segundo_archivo);
	}

	return estado_operacion;
}
/*
	Precondicion: Recibe una fecha de liberacion con formato aaaamm(donde 0000 < aaaa <= 9999 y 01 <= mm <= 12) 
	Postcondion: La funcion actualizara los archivos crucio y imperius mediante el archivo liberables con la fecha ingresada.
				Devolvera OK si pudo actualizar correctamente ambos archivos,devolvera ERROR en caso contrario mostrando por pantalla un error 
				informandole al usuario lo sucedido(tener en cuenta que los archivos que se estaban generando quedan como estaban cuando suceda el error).
*/
int actualizar(char fecha_liberacion[MAX_FECHA]){

	char nombre_archivo_liberables[MAX_NOMBRE_ARCHIVO];
	generar_nombre_liberables(nombre_archivo_liberables,fecha_liberacion);

	FILE* preso_crucio = fopen(PRISIONEROS_CRUCIO,LECTURA);
	if(preso_crucio == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",PRISIONEROS_CRUCIO);
		return ERROR;
	}
	FILE* preso_imperius = fopen(PRISIONEROS_IMPERIUS,LECTURA);
	if(preso_imperius == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",PRISIONEROS_IMPERIUS);
		fclose(preso_crucio);
		return ERROR;
	}
	FILE* liberables = fopen(nombre_archivo_liberables,LECTURA);
	if(liberables == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",nombre_archivo_liberables);
		fclose(preso_crucio);
		fclose(preso_imperius);
		return ERROR;
	}
	FILE* nuevo_primer_archivo = fopen(ARCHIVO_AUXILIAR,ESCRITURA);
	if(nuevo_primer_archivo == NULL){
		fclose(preso_crucio);
		fclose(preso_imperius);
		fclose(liberables);
		return ERROR;
	}
	FILE* nuevo__segundo_archivo = fopen(ARCHIVO_AUXILIAR_2,ESCRITURA);
	if(nuevo__segundo_archivo == NULL){
		fclose(preso_crucio);
		fclose(preso_imperius);
		fclose(liberables);
		fclose(nuevo_primer_archivo);
		return ERROR;
	}

	int estado_operacion = OK;
	estado_operacion = diferencia_tres_archivos(preso_crucio,preso_imperius,liberables,nuevo_primer_archivo,nuevo__segundo_archivo);

	fclose(preso_crucio);
	fclose(preso_imperius);
	fclose(liberables);
	fclose(nuevo_primer_archivo);
	fclose(nuevo__segundo_archivo);

	remove(PRISIONEROS_CRUCIO);
	rename(ARCHIVO_AUXILIAR,PRISIONEROS_CRUCIO);
	remove(PRISIONEROS_IMPERIUS);
	rename(ARCHIVO_AUXILIAR_2,PRISIONEROS_IMPERIUS);

	return estado_operacion;
}

/* 
	Postcondion: La funcion mostrara por pantalla los presos que se encuentran en el archivo liberables con la fecha ingresada.
				En caso de no existir dicho archivo se mostrara por pantalla un error informandole al usuario lo sucedido.
*/
void mostrar_liberados(char fecha_liberacion[MAX_FECHA]){

	char nombre_archivo_liberables[MAX_NOMBRE_ARCHIVO];
	generar_nombre_liberables(nombre_archivo_liberables,fecha_liberacion);

	FILE* presos_liberables = fopen(nombre_archivo_liberables,LECTURA);
	if(presos_liberables == NULL){
		printf("\033[1;31m");//color rojo
		printf("-EL archivo ( %s ) no se ha encontrado o tuvo problemas para abrirse.\n",nombre_archivo_liberables);
	}
	else{
		printf("Los prisioneros liberables de la fecha  %s  son:\n",fecha_liberacion);

		char nombre[MAX_NOMBRE];
		int leido = fscanf(presos_liberables,FORMATO_LECTURA,nombre); 
		while(leido > 0){
			printf(" -%s\n", nombre);
			leido = fscanf(presos_liberables,FORMATO_LECTURA,nombre); 
		}

		fclose(presos_liberables);
	}
}

/* 
	Postcondion: La funcion mostrara por pantalla el comando seguido de su descripcion.
*/
void mostrar_comando(char comando[],char descripcion[]){
	printf("\033[1;36m");//color rojo
	printf("  •%s\n",comando);
	printf("\033[0;0m");//color neutro
	printf("       %s\n\n",descripcion);    	
}
/* 
	Postcondion: La funcion mostrara todos los comandos que el usuario tenga disponibles para usar y cómo se utiliza cada funcionalidad.
*/
void mostrar_ayudas(){
	system("clear");

	printf("                    ~~BIENVENIDO A LA PRISION DE AZKABAN~~\n");
	printf("Aqui se incluyen unos posibles comandos que usted puede ejecutar:\n");

	mostrar_comando(COMANDO_PERDONABLES,"Genera un arhivo con el nombre ingresado por el usuario, que contiene a los presos que pueden ser perdonados.Debe contener la extension.");
	mostrar_comando(COMANDO_LIBERAR,"Genera un archivo con el nombre ingresado por el usuario, que contiene a los presos que pueden ser liberados segun la fecha con formato (aaaamm donde 0000 < aaaa <= 9999 y 01 <= mm <= 12) y conducta (B,R o M en mayuscula).");
	mostrar_comando(COMANDO_ACTUALIZAR,"Actualiza ambos archivos de presos crucio y imperius eliminando los presos del archivo de liberables en la fecha ingresada con formato (aaaamm).");
	mostrar_comando(COMANDO_MOSTRAR,"Muestra por pantalla los presos que pueden ser liberados que se encuentran en el archivo liberables de la fecha ingresada con formato (aaaamm).");
	mostrar_comando(COMANDO_AYUDA,"Muestra los comandos disponible que el usuario puede utilizar.");
}

