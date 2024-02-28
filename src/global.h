#include <SDL2/SDL.h>
#include "uthash.h"

#define TAILLE_MAX_CHEMIN 200
#define TAILLE_MAX_ID 100

typedef struct {
    SDL_Texture * texture;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_ressource; 

extern t_ressource * ressources;
