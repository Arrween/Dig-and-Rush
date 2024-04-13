#ifndef PERSOS_H
#define PERSOS_H

#include <SDL2/SDL.h>

extern const char *personnage_selectionne; // Déclaration externe de la variable globale


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

void selectionner_personnage(const char* nom_personnage);

#endif /* PERSOS_H */
