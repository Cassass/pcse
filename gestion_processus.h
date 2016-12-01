#ifndef _GESTION_PROCESSUS_
#define _GESTION_PROCESSUS_

#include "affichage.h"
#include "time_handler.h"
#include "debug.h"

#define NB_PROC 2
#define STACK_SIZE 512

typedef enum Etat Etat;
enum Etat {ELU, ACTIVABLE};

typedef struct processus{
	int8_t pid;
	char* name;
	Etat state;
	uint32_t proc_reg[5];
	uint32_t stack[STACK_SIZE]; //memalloc aussi possible
}processus;

processus ptable[NB_PROC];

processus pidle;
processus proce1;

void init_processus(void);
void idle(void);
void proc1(void);
extern void ctx_sw(uint32_t* ancien, uint32_t* nouveau);
#endif
