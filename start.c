#include <cpu.h>
#include <inttypes.h>
#include "time_handler.h"

extern void traitant_IT_32(void);

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display


void kernel_start(void)
{
	// initialisation
	init_traitant_IT(32, traitant_IT_32);
	set_clock();
	masque_IRQ(0, false);
	//demasquage des interruptions externes
	sti();
	while (1) {
		// cette fonction arrete le processeur
		hlt();
	}
}

