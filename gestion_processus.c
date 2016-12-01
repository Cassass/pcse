#include "gestion_processus.h"
#include "cpu.h"
#include "debug.h"
#include "segment.h"

// variable globale
processus* actif;

// initialisation des structures de processus
void init_processus(void){

	// initialisation de idle
	pidle.pid = 0;
	pidle.name = "idle";
	pidle.state = ELU;
	
	// initialisation de proc1
	//ptr de fonction pour la pile
	void (*func_ptr)(void);
	func_ptr = proc1;

	proce1.pid = 1;
	proce1.name = "proc1";
	proce1.state = ACTIVABLE;
	proce1.stack[511] =  (uint32_t)(func_ptr);
	proce1.proc_reg[1] = (uint32_t)&proce1.stack[511];

	// init variable globale
	actif = &pidle;
}

// retourne le nom d'un processus
char* mon_nom(void){
	return actif->name;
}

// retourne le pid d'un processus
int32_t mon_pid(void){
	return actif->pid;
}

// fonction d'ordonnancement
void ordonnance(void){
	if (pidle.state == 0){ // 0 == ELU dans l'enum
		pidle.state = 1; // 1 == ACTIVABLE
		proce1.state = 0;
		actif = &proce1;
		ctx_sw(pidle.proc_reg, proce1.proc_reg);
	} else {
		pidle.state = 1;
		proce1.state = 0;
		actif = &pidle;
		ctx_sw(proce1.proc_reg, pidle.proc_reg);
	}
}

//////////////////////// TESTS PARTIE COMMUNE ///////////////////////////////
/*
void proc1(void){
	for(int i = 0; i < 3; i++){
	printf("[proc1] idle m’a donne la main\n");
	printf("[proc1] je tente de la lui redonner\n");
	ctx_sw(proce1.proc_reg, pidle.proc_reg);
	}
	hlt();
}

void idle(){
	for (int i = 0; i < 3; ++i){
		printf("[idle] je tente de passer la main a proc1...\n");
		ctx_sw(pidle.proc_reg, proce1.proc_reg);
		printf("[idle] proc1 m’a redonne la main\n");
	}
	printf("[idle] je bloque le systeme\n");
}
*/

void idle(void){
	for (int i = 0; i < 3; i++){
		printf("[%s], pid = %i", mon_nom(), mon_pid());
		ordonnance();
	}
}

void proc1(){
	for (int i = 0; i < 3; i++){
		printf("[%s], pid = %i", mon_nom(), mon_pid());
		ordonnance();
	}
}
