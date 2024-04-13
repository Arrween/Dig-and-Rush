#ifndef ENTITE_PERSO_H
#define ENTITE_PERSO_H

#include "entite.h"
#include "texte.h"

/**
 * @brief Structure représentant un personnage joueur.
 */
 
struct s_perso {
    t_entite * parent; /**< entité caractérisée comme entité de personnage joueur par cette structure */
    int est_mort; /**< Indique si le joueur est mort (1) ou non (0). */
    SDL_FRect hitbox_attaque; /**< utilisée pour vérifier si une entité est touchée par l’attaque du joueur */
    int doit_afficher_hitbox_attaque; /**& booléen régissant l’affichage de `hitbox_attaque` */
    int vie;
};

t_entite * creer_entite_perso(char*, float, float, float, float, int);
void detruire_perso(t_perso**);

void perso_porter_coup(t_entite*, int*, t_texte*);
void perso_prendre_coup(t_entite*);
void perso_mourir(t_entite*);

#endif
