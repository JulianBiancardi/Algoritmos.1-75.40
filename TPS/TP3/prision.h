#ifndef __PRISION_H__
#define __PRISION_H__

#define OK  1
#define ERROR -1

#define MAX_NOMBRE 200
#define MAX_FECHA 7

typedef struct preso{
	char nombre[MAX_NOMBRE];
	unsigned int edad;
	char conducta; 
	unsigned int pabellon;
	unsigned int celda;
	char fecha[MAX_FECHA]; //formato aaaamm
	int maldicion_realizada; //1:crucio 2:imperius
}preso_t;



int generar_presos_perdonables(char nombre_archivo[]);

int generar_presos_liberables(char nombre_archivo[],char fecha_liberacion[MAX_FECHA],char conducta);

int actualizar(char fecha_liberacion[MAX_FECHA]);

void mostrar_liberados(char fecha_liberacion[MAX_FECHA]);

void mostrar_ayudas();


#endif /* __PRISION_H__ */