#include <string.h>

#include "entite_destructible.h"
#include "entite.h"
#include "constantes.h"

/**
 * @brief Crée une nouvelle structure d'entité destructible avec les attributs spécifiés.
 * 
 * @param id Identifiant de la ressource utilisée pour définir les valeurs par défaut.
 * @return Pointeur vers la nouvelle structure d'entité destructible créée, NULL en cas d'échec.
 */

t_destructible * creer_destructible(char * id) {
    t_destructible * nouv = malloc(sizeof(t_destructible));
    if (strcmp(id, "bloc_terre") == 0)
        strcpy(nouv->id_son, "destruction_bloc");
    else
        strcpy(nouv->id_son, "destruction_bloc");
    return nouv;
}

/**
 * @brief Crée une nouvelle structure d'entité destructible avec les attributs spécifiés.
 * 
 * @param id Identifiant de l'entité destructible.
 * @param x Position en x de l'entité destructible.
 * @param y Position en y de l'entité destructible.
 * @param w Largeur de l'entité destructible.
 * @param h Hauteur de l'entité destructible.
 * @param est_relatif 1 si les coordonnées sont relatives, 0 si elles sont absolues.
 * @return Pointeur vers la nouvelle structure d'entité créée, NULL en cas d'échec.
 */

t_entite * creer_entite_destructible(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite(id, x, y, w, h, est_relatif);
    nouv->destructible = creer_destructible(id);
    nouv->est_obstacle = VRAI;
    return nouv;
}

/**
 * @brief Détruit une structure d'entité destructible et libère la mémoire associée.
 * 
 * @param d Pointeur vers le pointeur de la structure d'entité destructible à détruire.
 */

void detruire_destructible(t_destructible ** d) {
    if (*d) {
        free(*d);
    }
    *d = NULL;
}
