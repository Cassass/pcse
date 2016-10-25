#ifndef _TIME_HANDLER_H_
#define _TIME_HANDLER_H_

#include "affichage.h"
#include "stdbool.h"

// affiche l'heure en haut à droite de la console
extern void console_settime(char* chaine);
// partie C du traitant d'IT. Acquitte l'IT et regle l'horloge
void tic_PIT(void);
void init_traitant_IT(int32_t num_IT, void (*traitant)(void));
void masque_IRQ(uint32_t num_IRQ, bool masque);
void set_clock(void);

#endif
