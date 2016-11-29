#include "time_handler.h"
#include "cpu.h"
#include "debug.h"
#include "segment.h"
/* #include "stdbool.h" */

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

//compteur de "ticks" d'horloge
uint8_t cpt = 0;
//heure
uint8_t s = 0;
uint8_t m = 0;
uint8_t h = 0;

extern void console_settime(char* chaine){
	//placement du curseur pour affichage de l'heure tout en haut a droite
	console_putbytes("\t\t\t\t\t\t\t\t\t\b", 10);
	//ecriture de l'heure
	console_putbytes(chaine, 10);	
}

void tic_PIT(void){
	outb(0x20, 0x20);
	++cpt;
	if(cpt == 50){ // prerequis : regler la fq d'horloge a 50Hz
		if(s == 59){
			s = 0;
			// on passe a une nouvelle minute
			if (m == 59){ 
				// on passe a une nouvelle heure
				++h;
				m = 0;
				// on ne prend arbitrairement aucune contrainte sur l'heure
			}else{
				++m;				
			}			
		}else{
			++s;
		}		
	}	
	//passage en chaine de caractere
	char chaine[10]; //OK pour \0
	sprintf(chaine, "%u:%u:%u", h, m, s);
	console_settime(chaine);
}

void init_traitant_IT(int32_t num_IT, void (*traitant)(void)){
	//seul appel : init_traitant_IT(32, traitant_IT_32) via .S
	//adresse a intialiser est n mots de 8 octets plus loin : "1000 + 8*n"
	// chargement 1er mot en little endian
	uint32_t* ptr = (uint32_t*)(0x1000 + num_IT*8);
	*ptr = (KERNEL_CS << 16) | (0x00FF & (uint32_t)(traitant));
	// chargement du 2e mot
	// maj de l'adresse
	ptr = (uint32_t*)((uint32_t)(ptr) + 8);
	*ptr = ((0xFF00 & (uint32_t)traitant) << 16) | (0x8E00);	
}

void set_clock(void){
	outb(0x34, 0x43);
	outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
	// le modulo 2^8 tronque les bits de poids forts
	outb((QUARTZ / CLOCKFREQ) >> 8, 0x40);
	// on fait un décalage bits à bits pour envoyer les poids forts
}

void masque_IRQ(uint32_t num_IRQ, bool masque){
	uint8_t m = inb(0x21);
	bool tab_bool[8]; // 8 numeros d'IRQ
	// un peu sale, il doit y avoir plus joli
	// remplissage du tableau de booleens
	for (int i=0; i<8; ++i){
		tab_bool[i] = (bool)((m >> i) & 0x1); // i.e on ne garde que le ieme bit
	}
	//chgt du masque pour l'IRQ num_IRQ
	tab_bool[num_IRQ] = masque;
	
	m = tab_bool[7];
	//reformation de m pour envoi sur 0x21
	for (int i=6; i>=0; --i){
		m = (m << 1) | tab_bool[i];
	}
	outb(m, 0x21);
}

