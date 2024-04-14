#ifndef NUIT_H
#define NUIT_H

#include <SDL2/SDL.h>

#include "entite.h"
#include "constantes.h"

#define MAX_RAYON_OMBRE TAILLE_H
#define FACTEUR_MIN_RAYON_OMBRE 4 // multipli par la largeur du personnage
#define FACTEUR_OBSCURCISSEMENT 6.

#define PAS_TRANSITION 5 // en pourcentage par frame

#define CANAL_MUS_JOUR 8
#define CANAL_MUS_NUIT 9

typedef struct {
    float min_alpha;
    float max_alpha;
    float alpha;
    float min_rayon;
    float max_rayon;
    float rayon;
    float min_musique;
    float max_musique;
    float volume_musique_jour;
    int est_active;
    int est_active_prec;
    t_entite * centre;
    SDL_FRect etendue;
    SDL_Texture * texture_ombre;
    SDL_Texture * texture_jour;
    SDL_Texture * texture_nuit;
} t_nuit;

t_nuit * creer_nuit(SDL_Renderer*, t_entite* centre, SDL_FRect zone_jeu,
                    SDL_Texture* texture_jour, SDL_Texture* texture_nuit);
void transitionner_nuit(t_nuit*);
void detruire_nuit(t_nuit ** nuit);

#endif
