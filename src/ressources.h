#ifndef RESSOURCES_H
#define RESSOURCES_H

#include "uthash.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define TAILLE_MAX_CHEMIN 200
#define TAILLE_MAX_ID 100

/**
 * @brief Structure représentant un élément de la table de hachage des ressources textures
 */
typedef struct {
    SDL_Texture * texture; /**< texture chargée depuis le fichier ressource */
    char id[TAILLE_MAX_ID]; /**< identifiant de la ressource dans la table de hachage */
    UT_hash_handle hh; /**< handle de la table de hachage */
} t_texture; 

extern t_texture * textures;

/**
 * @brief Valeurs possibles pour le type d’animation d’une entité à un moment donné
 */
typedef enum { ANIM_NULLE, REPOS, CHUTE_G, CHUTE_D, DEPL_G, DEPL_D, CREUSER, ATTQ_G, ATTQ_D, ANIM_MORT, ANIM_MORT_STATIQUE } t_id_anim;

typedef struct {
    t_id_anim id;
    int x_sprite_ini;
    int y_sprite;
    int w_sprite;
    int h_sprite;
    int decalage_dest_x;
    int decalage_dest_y;
    int longueur;
    float vitesse_anim;
    SDL_FRect * hitboxes_attaque;
} t_animation;


typedef struct {
    SDL_Texture * texture;
    t_animation ** animations;
    int n_animations;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_spritesheet; 

extern t_spritesheet * spritesheets;

typedef struct {
    Mix_Chunk * tampon;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_son; 

extern t_son * sons;

typedef struct {
    Mix_Music * tampon;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_musique; 

extern t_musique * musiques;

typedef struct {
    TTF_Font * police;
    char id[TAILLE_MAX_ID];
    UT_hash_handle hh;
} t_police; 

extern t_police * polices;


void init_ressources(SDL_Renderer *);

SDL_Texture * recuperer_texture(const char*);

t_spritesheet * recuperer_spritesheet(const char*);

t_son * recuperer_son(const char*, int);
t_musique * recuperer_musique(const char*, int);
int jouer_audio(int, const char*, int);

TTF_Font * recuperer_police(const char*);

t_animation * recuperer_animation(t_animation **, int, t_id_anim);

void detruire_ressources(void);

#endif /* RESSOURCES_H */
