#include "uthash.h"

#define TAILLE_MAX_CHEMIN 200
#define TAILLE_MAX_ID 100

typedef struct {
    SDL_Texture * texture;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_texture; 

extern t_texture * textures;

typedef struct {
    SDL_Texture * texture;
    int sprite_l, sprite_h;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_spritesheet; 

extern t_spritesheet * spritesheets;

typedef struct {
    SDL_AudioSpec spec;
    uint32_t length;
    uint8_t * buffer;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_son; 

extern t_son * sons;

void init_ressources(SDL_Renderer *);

SDL_Texture * recuperer_texture(const char*);
t_spritesheet * recuperer_spritesheet(const char*);
t_son * recuperer_son(const char*);

void detruire_ressources(void);
