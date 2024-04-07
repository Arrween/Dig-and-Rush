#include "entite_obstacle.h"
#include "entite.h"
#include "constantes.h"

t_entite * creer_entite_obstacle(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite(id, x, y, w, h, est_relatif);
    nouv->est_obstacle = VRAI;
    return nouv;
}
