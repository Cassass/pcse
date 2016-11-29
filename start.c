#include <cpu.h>
#include <inttypes.h>
#include "time_handler.h"
#include "gestion_processus.h"
extern void traitant_IT_32(void);


void kernel_start(void)
{
	// initialisation des structures de processus
	init_processus();
	/* // initialisation de idle */
	/* processus pidle; */
	/* pidle.pid = 0; */
	/* pidle.name = "idle"; */
	/* pidle.state = ELU; */
	
	/* // initialisation de proc1 */
	/* //ptr de fonction pour la pile */
	/* void (*func_ptr)(void); */
	/* func_ptr = proc1; */

	/* processus proce1; */
	/* proce1.pid = 1; */
	/* proce1.name = "proc1"; */
	/* proce1.state = ACTIVABLE; */
	/* proce1.stack[511] =  (uint32_t)(func_ptr); */
	/* proce1.proc_reg[1] = (uint32_t)&proce1.stack[511]; */
	// lancement du test
	idle();
	/* init_traitant_IT(32, traitant_IT_32); */
	/* set_clock(); */
	/* masque_IRQ(0, false); */
	//demasquage des interruptions externes
	//sti();
	while (1) {
		// cette fonction arrete le processeur
		hlt();
	}
}

