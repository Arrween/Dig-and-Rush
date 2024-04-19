/**
 * @file constantes.h
 * @brief Fichier d'en-tête pour les constantes du jeu.
 */

#define TITRE_FENETRE "Dig & Rush"
#define TAILLE_L 1280
#define TAILLE_H 720
#define FPS 30

/** @brief Fréquence audio en Hz. */
#define FREQ_AUDIO 44100
#define TAILLE_TAMPON_AUDIO 1024
/** @brief Nombre de canaux audio. */
#define N_CANAUX 10
/** @brief Volume initial de la musique du jeu (en pourcentage de MIX_MAX_VOLUME). */
#define VOLUME_MUSIQUE_INI (MIX_MAX_VOLUME * 0.2)
/** @brief Volume initial des effets sonores du jeu (en pourcentage de MIX_MAX_VOLUME). */
#define VOLUME_SONS_INI (MIX_MAX_VOLUME * 0.2)
/** @brief Canal audio pour les sons du menu. */
#define CANAL_BTN_MENU 0
/** @brief Canal audio pour les phrases d'accroche. */
#define CANAL_ACCROCHE 3
/** @brief Canal audio pour le cri du loup. */
#define CANAL_LOUP 1
/** @brief Canal audio pour le cri du coq. */
#define CANAL_COQ 1
/** @brief Canal audio pour le son des attaques du personnage. */
#define CANAL_ATTAQUE_PERSO 2
/** @brief Canal audio pour le son des blessures reçu sur le personnage. */
#define CANAL_BLESSURE_PERSO 3
/** @brief Canal audio pour le son lorsque le personnage meurt. */
#define CANAL_MORT_PERSO 3
/** @brief Canal audio pour le son des attaques du PNJ. */
#define CANAL_ATTAQUE_PNJ 4
/** @brief Canal audio pour le son lorsque le PNJ meurt. */
#define CANAL_MORT_PNJ 4
/** @brief Canal audio lorsque le personnage récupère un bonus. */
#define CANAL_BONUS 5
/** @brief Canal audio pour le son lorsque le personnage creuse. */
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
