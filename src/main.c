#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TITRE_FENETRE "jeu projet L2"
#define TAILLE_L 640
#define TAILLE_H 480

int main() {
    SDL_Window * fenetre;
    SDL_Renderer * rend;
    SDL_Texture * textureFond ; // Texture pour l'image de fond

    
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialisation de SDL_image pour le support des images PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création de la fenêtre
    if (!(fenetre = SDL_CreateWindow(TITRE_FENETRE,
				    SDL_WINDOWPOS_CENTERED,
				    SDL_WINDOWPOS_CENTERED,
				    TAILLE_L,
				    TAILLE_H,
				    0))) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création du renderer
    if (!(rend = SDL_CreateRenderer(fenetre, -1, 0))) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Chargement de l'image PNG dans une texture
    if (! (textureFond = IMG_LoadTexture(rend,"../update.png")) ){
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

   // Nettoyer le renderer
    SDL_RenderClear(rend);
    // Dessiner la texture de fond
    SDL_RenderCopy(rend, textureFond, NULL, NULL);
    // Mettre à jour l'écran
    SDL_RenderPresent(rend);

    // Attendre un peu avant de quitter
    SDL_Delay(5000);

    // Nettoyage
    SDL_DestroyTexture(textureFond);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
