/** @file
 *  @brief implémentation du rôle de bonus ramassable d’une entité (entête)
 */

#ifndef _ENTITE_bonus_H
#define _ENTITE_bonus_H

#include "entite.h"
#include "constantes.h"

/**
 * @brief Structure renseignant le rôle de bonus ramassable d’un `t_entite`
 */
struct s_bonus {
    int valeur;                 /**< nombre de points de score à ajouter lors du ramassage */
    int soin;                   /**< nombre de points de vie à ajouter lors du ramassage */
    char id_son[TAILLE_MAX_ID]; /**< id de ressource du son à jouer lors du ramassage */
};

t_entite * creer_entite_bonus(char* id, float x, float y, float w, float h, int est_relatif);
void detruire_bonus(t_bonus**);

#endif
