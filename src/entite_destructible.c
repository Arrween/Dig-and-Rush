#include "entite_destructible.h"
#include "entite.h"
#include "constantes.h"

void action_destruction_essai(void) {
    printf("objet destructible détruit en jeu\n");
}

t_destructible * creer_destructible(void) {
    t_destructible * nouv = malloc(sizeof(t_destructible));
    nouv->action_destruction = action_destruction_essai;
    return nouv;
}

t_entite * creer_entite_destructible(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite(id, x, y, w, h, est_relatif);
    nouv->destructible = creer_destructible();
    return nouv;
}

void detruire_destructible(t_destructible ** d) {
    if (*d) {
        // detruire_entite(&((*d)->entite));
        free(*d);
    }
    *d = NULL;
}
