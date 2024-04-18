/**
 * @file constantes.h
 * @brief Fichier d'en-tête pour les constantes du jeu.
 */

#define TITRE_FENETRE "Dig & Rush"
#define TAILLE_L 1280
#define TAILLE_H 720
#define FPS 30

#define FREQ_AUDIO 44100
#define TAILLE_TAMPON_AUDIO 1024
#define N_CANAUX 10
#define FACTEUR_VOLUME_MUSIQUE_INI 0.2
#define FACTEUR_VOLUME_SONS_INI 0.2

#define CANAL_BTN_MENU 0
#define CANAL_ACCROCHE 3
#define CANAL_LOUP 1
#define CANAL_COQ 1
#define CANAL_ATTAQUE_PERSO 2
#define CANAL_BLESSURE_PERSO 3
#define CANAL_MORT_PERSO 3
#define CANAL_ATTAQUE_PNJ 4
#define CANAL_BONUS 5
#define CANAL_CREUSAGE 6
/** @brief canal audio réservé pour la musique de jour */
#define CANAL_MUS_JOUR 8
/** @brief canal audio réservé pour la musique de nuit */
#define CANAL_MUS_NUIT 9


#define VITESSE_CHUTE 2./1 // float, en %age de la hauteur de jeu par frame


#define COULEUR_HITBOX_R 255
#define COULEUR_HITBOX_V 255
#define COULEUR_HITBOX_B 255
#define COULEUR_HITBOX_A 255

#define COULEUR_HITBOX_ATTQ_R 255
#define COULEUR_HITBOX_ATTQ_V 0
#define COULEUR_HITBOX_ATTQ_B 0
#define COULEUR_HITBOX_ATTQ_A 255

#define FAUX 0
#define VRAI 1
