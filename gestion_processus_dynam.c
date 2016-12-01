#include "gestion_processus_dynam.h"
#include "cpu.h"
#include "debug.h"
#include "segment.h"
#include "stdio.h"
#include <malloc.c.h>
// #define ENOMEM 12
// ca partait d'une bonne intention mais pas de errno.h ou de assert.h
/*
void memory_error(void){
	errno = ENOMEM;
	perror(0);
	exit(1);
}

void *xmalloc(size_t size){
	void * p = malloc(size);
	if (!p)
		memory_error();
	return p;
}

void *xcalloc(size_t nbmem, size_t size){
	void * p = calloc(nbmem, size);
	if (!p)
		memory_error();
	return p;
}

void *xrealloc(void *ptr, size_t size){
	void * p = realloc(ptr, size);
	if (!p)
		memory_error();
	return p;
}
*/

// creation d'un processus, en distinguant idle et les autres 
int32_t cree_processus(void (*code)(void), char* nom){
	if (pid_courant_creat < NB_PROC){
		ptable[pid_courant_creat] = malloc(sizeof(processus));
		ptable[pid_courant_creat]->pid = pid_courant_creat;
		ptable[pid_courant_creat]->name = nom; //strcpy
		if (pid_courant_creat == 0){ // idle
			ptable[pid_courant_creat]->state = 0;
			actif = &ptable[pid_courant_creat];
			// pas d'init de la pile ou des registres
		} else { //proc "classique"
			ptable[pid_courant_creat]->state = 1; // ACTIVABLE
			ptable[pid_courant_creat]->stack[511] = (uint32_t)(code);
			ptable[pid_courant_creat]->proc_reg[1] = 
				(uint32_t)&(ptable[pid_courant_creat]->stack[511]);
		}
		pid_courant_creat++;
		return pid_courant_creat-1;			
	} else {
		return -1;
	}
}

void inserer_activables(processus* p){
	if (activables->queue == NULL){ // aucun processus n'est activable
		activables->head = p;
		activables->queue = p;
	}else{
		activables->queue->next = p;
		activables->queue = p;
	}
}

void ordonnance(void){
	int32_t pid_actif = actif->pid; //pas forcement necessaire, a tester
	inserer_activables(actif);
	ctx_sw(actif, passer_elu(pid_actif+1));
}

