#ifndef ENTITE_PNJ_H
#define ENTITE_PNJ_H

#include "entite.h"

/**
 * @brief Structure représentant un PNJ (Personnage Non-Joueur).
 */
 
struct s_pnj {
    void (*comportement) (t_entite*); /**< Pointeur vers la fonction de comportement du PNJ. */
    float x_patrouille_g; /**< Position en x du point de patrouille gauche du PNJ. */
    float x_patrouille_d; /**< Position en x du point de patrouille droit du PNJ. */
    int est_mort; /**< Indique si le PNJ est mort (1) ou non (0). */
    int valeur_vaincu; /**< Valeur en points attribuée au PNJ une fois vaincu. */
    int est_ecrasable; /**< Indique si le PNJ est écrasable (1) ou non (0). */
};

t_entite * creer_entite_pnj_depuis_spritesheet(char*, float, float, float, float, int);
t_entite * creer_entite_pnj(char*, float, float, float, float, int);

#endif
