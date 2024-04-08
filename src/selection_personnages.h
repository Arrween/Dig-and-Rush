#ifndef PERSOS_H
#define PERSOS_H

#define N 1

#include <SDL2/SDL.h>


// Structure pour représenter un personnage avec sa texture et sa position
typedef struct {
    SDL_Texture* texture;
    SDL_Rect position;
} Persos;


// Fonction pour afficher les personnages sur le renderer SDL
void afficher_images(SDL_Renderer* renderer, Persos personnages);

// Fonction pour créer et afficher les personnages
void afficher_persos(SDL_Renderer* renderer);

// Fonction principale pour afficher les personnages
void afficher_personnages(SDL_Renderer* renderer, Persos personnages);

void gerer_clic_personnage(SDL_Event event, t_entite* perso, SDL_Rect* rect, int* selectionne);


void afficher_personnage_selectionne(SDL_Renderer* renderer, t_entite* perso, int selectionne);


#endif /* PERSOS_H */
