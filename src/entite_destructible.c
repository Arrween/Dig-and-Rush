#include <string.h>

#include "entite_destructible.h"
#include "entite.h"
#include "constantes.h"

void action_destruction_essai(void) {
    // printf("objet destructible détruit en jeu\n");
}

void init_destructible(t_destructible * d, char * id) {
    if (strcmp(id, "bloc_terre") == 0)
        strcpy(d->id_son, "destruction_bloc");
}

t_destructible * creer_destructible(char * id) {
    t_destructible * nouv = malloc(sizeof(t_destructible));
    nouv->action_destruction = action_destruction_essai;
    init_destructible(nouv, id);
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
