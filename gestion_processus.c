#include "gestion_processus.h"
#include "cpu.h"
#include "debug.h"
#include "segment.h"

void init_processus(void){
	// initialisation des structures de processus
	// initialisation de idle
	/* processus pidle; */
	pidle.pid = 0;
	pidle.name = "idle";
	pidle.state = ELU;
	
	// initialisation de proc1
	//ptr de fonction pour la pile
	void (*func_ptr)(void);
	func_ptr = proc1;

	/* processus proce1; */
	proce1.pid = 1;
	proce1.name = "proc1";
	proce1.state = ACTIVABLE;
	proce1.stack[511] =  (uint32_t)(func_ptr);
	proce1.proc_reg[1] = (uint32_t)&proce1.stack[511];
}

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
