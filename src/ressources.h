/**
 * @file ressources.h
 * @brief Définitions des structures et fonctions liées aux ressources du jeu.
 */

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

/**
 * @brief Structure représentant une animation
 */
typedef struct {
    t_id_anim id; /**< Identifiant de l'animation */
    int x_sprite_ini; /**< Position initiale en x du sprite */
    int y_sprite; /**< Position en y du sprite */
    int w_sprite; /**< Largeur du sprite */
    int h_sprite; /**< Hauteur du sprite */
    int decalage_dest_x; /**< Décalage en x pour l'affichage */
    int decalage_dest_y; /**< Décalage en y pour l'affichage */
    int longueur; /**< Longueur de l'animation */
    float vitesse_anim; /**< Vitesse de l'animation */
} t_animation;


/**
 * @struct t_spritesheet
 * @brief Structure représentant une feuille de sprites.
 */

typedef struct t_spritesheet {
    SDL_Texture * texture; /**< Pointeur vers la texture SDL de la feuille de sprites. */
    char id[TAILLE_MAX_ID]; /**< Identifiant de la feuille de sprites. */
    t_animation ** animations; /**< Tableau des animations de la feuille de sprites. */
    int n_animations; /**< Nombre d'animations dans la feuille de sprites. */
    UT_hash_handle hh; /**< Handle pour utiliser la bibliothèque uthash. */
} t_spritesheet;

extern t_spritesheet * spritesheets;

/**
 * @brief Structure représentant un fichier audio chargé en mémoire.
 */
typedef struct {
    Mix_Chunk * tampon; /**< Pointeur vers le fichier audio chargé en mémoire */
    char id[TAILLE_MAX_ID]; /**< Identifiant de la ressource audio */
    UT_hash_handle hh; /**< Handle pour utiliser la bibliothèque uthash */
} t_son; 

extern t_son * sons;

/** @brief Structure représentant un fichier de musique */
typedef struct {
    Mix_Music * tampon; /**< Pointeur vers le fichier de musique */
    char id[TAILLE_MAX_ID]; /**< Identifiant de la musique dans la table de hachage */
    UT_hash_handle hh; /**< Handle de la table de hachage */
} t_musique;


extern t_musique * musiques;

/** @brief Structure représentant une police de caractères */
typedef struct {
    TTF_Font * police; /**< Pointeur vers la police de caractères */
    char id[TAILLE_MAX_ID]; /**< Identifiant de la police dans la table de hachage */
    UT_hash_handle hh; /**< Handle de la table de hachage */
} t_police;

extern t_police * polices;


void init_ressources(SDL_Renderer *);

SDL_Texture * recuperer_texture(const char*);

t_spritesheet * recuperer_spritesheet(const char*);

t_son * recuperer_son(const char*, int);
t_musique * recuperer_musique(const char*, int);
int jouer_audio(int canal, const char* id, int repetitions);

TTF_Font * recuperer_police(const char*);

t_animation * recuperer_animation(t_animation **, int, t_id_anim);

void detruire_ressources(void);

#endif /* RESSOURCES_H */
