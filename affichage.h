#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

#include <inttypes.h>
#include <stddef.h>

uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c);
void efface_ecran(void);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c);
void defilement(void);
extern void console_putbytes(const char* chaine, int32_t taille);

#endif
