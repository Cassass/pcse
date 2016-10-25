#include "cpu.h"
#include "string.h"
#include "affichage.h"

// variable globale pour la position du curseur
uint16_t ligne_curseur = 0;
uint16_t colonne_curseur = 0;

uint16_t *ptr_mem(uint32_t lig, uint32_t col){
	// on retourne ptr, a la postion de la case souhaitee
	uint16_t* ptr;
	ptr = (uint16_t *)(0xB8000 + 2*(80*lig + col));
	return ptr;
}

void ecrit_car(uint32_t lig, uint32_t col, char c){
	uint16_t* car_pos = ptr_mem(lig, col);
	// on recupere le code ascii de c
	*car_pos = (uint8_t)c;
	// force le dernier bit a 0 : pas de clignotement
	// force les 3 bits juste avant a 0 : fond noir
	// force les 4 lsb a 1 : texte en blanc
	// octet "format" souhaite : 0 000 1111
	*car_pos &= 0x0FFF; 
	*car_pos |= 0x0F00;
}

void efface_ecran(void){
	uint16_t* car_cour = ptr_mem(0,0);
	for (int ligne = 0; ligne < 25; ligne++){
		for (int colonne = 0; colonne < 80; colonne++){
			//chaque "case" recoit un espace blanc dans un fond noir
			*car_cour = 0x0F20;
		}
	}
}

void place_curseur(uint32_t lig, uint32_t col){
	// calcul de la position
	uint32_t pos_curseur = 80*lig + col;
	//actualisation variable globale
	ligne_curseur = lig;
	colonne_curseur = col;
	//gestion des 4 etapes
	// on indique au port de commande l'envoi de la partie basse
	outb(0x0F, 0x3D4);
	// on envoie la partie basse au port de donnees
	uint16_t moitie = pos_curseur & 0x00FF;
	outb(moitie,0x3D5);
	// idem pour la partie haute
	outb(0x0E, 0x3D4);
	moitie = pos_curseur & 0xFF00;
	outb(moitie, 0x3D5);
}

void traite_car(char c){
	// caracteres classiques : affichage simple
	if ((uint8_t)c >= 32 && (uint8_t)c <= 126){
		ecrit_car(c, ligne_curseur, colonne_curseur);
		if(colonne_curseur != 79){
			colonne_curseur++;
		}else if(ligne_curseur !=24){
			traite_car('\n');
		}
	}else if ((uint8_t)c == 8){
		// on recule le curseur
		if (colonne_curseur != 0){
			colonne_curseur--;
		}else if(ligne_curseur !=0){
			colonne_curseur = 79;
			ligne_curseur--;
		}
	}else if ((uint8_t)c == 9){
		uint16_t manque = 1;
		// tant qu'on ne trouve pas la fin de la ligne
		// ou la tabulation suivante...
		while ((colonne_curseur+manque != 79) || 
		       ((colonne_curseur+manque)%8 != 0)){
			manque++; // ... on incremente
		}
		colonne_curseur += manque;
	}else if ((uint8_t)c == 10){
		// retour a la ligne si on est pas sur la derniere
		if (ligne_curseur !=24){
			ligne_curseur++;
			colonne_curseur = 0;
		}else{
			defilement();
		}
	}else if ((uint8_t)c == 12){
		// on efface l'ecran
		efface_ecran();
		ligne_curseur = 0;
		colonne_curseur = 0;
	}else if ((uint8_t)c == 13){
		colonne_curseur = 0;
	}
	place_curseur(ligne_curseur, colonne_curseur);
}

void defilement(void){
	uint16_t* src = (uint16_t *)(0xB8000 + 2*80); // on deplace les lignes 1 à 24...
	uint16_t* dest = (uint16_t *)0XB8000; // ...dans 0 à 23
	memmove(dest, src, 2*80*25);
	//des blancs dans la derniere ligne
	uint16_t* car_cour = ptr_mem(24,0);
	for (int colonne = 0; colonne < 80; colonne++){
		//chaque "case" recoit un espace blanc dans un fond noir
		*car_cour = 0x0F20;
	}
	//on replace le curseur en début de ligne
	ligne_curseur = 24;
	colonne_curseur = 0;
	place_curseur(ligne_curseur, colonne_curseur);
}

extern void console_putbytes(const char* chaine, int32_t taille){
	for(int32_t i = 0; i < taille ; ++i){
		traite_car(chaine[i]);
		ecrit_car(ligne_curseur, colonne_curseur, chaine[i]);
	}
}
