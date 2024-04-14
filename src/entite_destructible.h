/** @file
 *  @brief implémentation du caractère destructible d’une `t_entite`
 */

#ifndef ENTITE_DESTRUCTIBLE_H
#define ENTITE_DESTRUCTIBLE_H

#include "entite.h"
#include "constantes.h"

/**
 * @brief Structure renseignant le caractère destructible d’une `t_entite`
 */
struct s_destructible {
    char id_son[TAILLE_MAX_ID]; /**< id de ressource du son à jouer lors de la destruction */
};

t_entite * creer_entite_destructible(char* id, float x, float y, float w, float h, int est_relatif);
void detruire_destructible(t_destructible**);

#endif
