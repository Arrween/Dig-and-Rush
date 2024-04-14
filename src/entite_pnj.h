#ifndef ENTITE_PNJ_H
#define ENTITE_PNJ_H

#include "entite.h"
#include "texte.h"

/**
 * @brief Structure représentant un PNJ (Personnage Non-Joueur).
 */
 
struct s_pnj {
    t_entite * parent; /**< entité caractérisée comme entité de PNJ par cette structure */
    void (*comportement) (t_entite*, t_entite*); /**< Pointeur vers la fonction de comportement du PNJ. */
    float x_patrouille_g; /**< Position en x du point de patrouille gauche du PNJ. */
    float x_patrouille_d; /**< Position en x du point de patrouille droit du PNJ. */
    int est_mort; /**< Indique si le PNJ est mort (1) ou non (0). */
    int valeur_vaincu; /**< Valeur en points attribuée par le PNJ une fois vaincu. */
    int est_ecrasable; /**< Indique si le PNJ est écrasable (1) ou non (0). */
    SDL_FRect hitbox_attaque; /**< utilisée pour vérifier si une entité est touchée par l’attaque du pnj */
    int doit_afficher_hitbox_attaque; /**& booléen régissant l’affichage de `hitbox_attaque` */
    char id_son_attaque[TAILLE_MAX_ID];
};

t_entite * creer_entite_pnj_depuis_spritesheet(char*, float, float, float, float, int);
t_entite * creer_entite_pnj(char*, float, float, float, float, int);
void detruire_pnj(t_pnj**);

void pnj_mourir(t_entite * pnj, int * score, t_texte * texte_score);

#endif
