#ifndef _GESTION_PROCESSUS_DYNAM_
#define _GESTION_PROCESSUS_DYNAM_

#include "affichage.h"
#include "time_handler.h"
#include "debug.h"

#define NB_PROC 8
#define STACK_SIZE 512

typedef enum Etat Etat;
enum Etat {ELU, ACTIVABLE, ENDORMI};

typedef struct processus{
	int8_t pid;
	char* name;
	Etat state;
	uint32_t proc_reg[5];
	uint32_t stack[STACK_SIZE]; //memalloc aussi possible
	processus* next;
}processus;

typedef struct p_activables{
	processus* head;
	processus* queue;
}p_activables;

p_activables activables;
// N = 8 processus
processus* ptable[NB_PROC];

processus* actif;
int32_t pid_courant_creat = 0;

int32_t cree_processus(void (*code)(void), char* nom);
void ordonnance(void);
#endif
