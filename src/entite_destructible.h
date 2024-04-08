#ifndef ENTITE_DESTRUCTIBLE_H
#define ENTITE_DESTRUCTIBLE_H

#include "entite.h"
#include "constantes.h"

struct s_destructible {
    char id_son[TAILLE_MAX_ID];
};

t_entite * creer_entite_destructible(char*, float, float, float, float, int);

#endif
