#include <string.h>

#include "entite_destructible.h"
#include "entite.h"
#include "constantes.h"

/** 
* @brief crée un nouveau `t_destructible` et définit les valeurs par défaut de
*       ses attributs en fonction du type de ressource
* @param id identifiant de ressource utilisé pour distinguer les valeurs par défaut à affecter
*/
t_destructible * creer_destructible(char * id) {
    t_destructible * nouv = malloc(sizeof(t_destructible));
    if (strcmp(id, "bloc_terre") == 0)
        strcpy(nouv->id_son, "destruction_bloc");
    else
        strcpy(nouv->id_son, "destruction_bloc");
    return nouv;
}

t_entite * creer_entite_destructible(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite(id, x, y, w, h, est_relatif);
    nouv->destructible = creer_destructible(id);
    nouv->est_obstacle = VRAI;
    return nouv;
}

void detruire_destructible(t_destructible ** d) {
    if (*d) {
        free(*d);
    }
    *d = NULL;
}
