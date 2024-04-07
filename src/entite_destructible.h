#ifndef ENTITE_DESTRUCTIBLE_H
#define ENTITE_DESTRUCTIBLE_H

#include <SDL2/SDL.h>
#include "entite.h"

struct s_destructible {
    void (*action_destruction) (void);
};

t_entite * creer_entite_destructible(char*, float, float, float, float, int);

#endif
