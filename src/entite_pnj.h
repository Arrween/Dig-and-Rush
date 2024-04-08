#ifndef ENTITE_PNJ_H
#define ENTITE_PNJ_H

#include "entite.h"

struct s_pnj {
    void (*comportement) (t_entite*);
    float x_patrouille_g;
    float x_patrouille_d;
    int est_mort;
    int valeur_vaincu;
};

t_entite * creer_entite_pnj_depuis_spritesheet(char*, float, float, float, float, int);
t_entite * creer_entite_pnj(char*, float, float, float, float, int);

#endif
