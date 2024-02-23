#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"

#define TITRE_FENETRE "Dig & Rush"
#define TAILLE_L 640
#define TAILLE_H 480
#define FPS 60

// Fonction qui initialise la SDL
void initialiser_sdl(){
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

// Fonction qui initialise la SDL_IMG
void initialiser_sdl_img(){
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

// Fonction qui crée la fenêtre du jeu
SDL_Window * creation_fenetre(){
    SDL_Window * fenetre ;
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
    return (fenetre);
}

// Fonction qui crée le renderer
SDL_Renderer * creation_renderer(SDL_Window ** fenetre){
    SDL_Renderer * rend ;
    if (!(rend = SDL_CreateRenderer(*fenetre, -1, 0))) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*fenetre);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    return rend;
}

//Fonction qui charge l'image de fond
SDL_Texture * chargement_image(char * chemin, SDL_Renderer ** rend, SDL_Window ** fenetre){
    SDL_Texture * texture_fond ;
    if (!(texture_fond = IMG_LoadTexture(*rend, chemin)) ){
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(*rend);
        SDL_DestroyWindow(*fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return texture_fond ;
}

// Fonction qui initialise la SDL_ttf
void initialiser_sdl_ttf() {
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

// Fonction qui charge la police à partir d'un certain chemin
TTF_Font * chargement_font(char * chemin, int taille, SDL_Renderer ** rend, SDL_Window ** fenetre) {
    TTF_Font * font = TTF_OpenFont(chemin, taille);
        if (font == NULL) {
            fprintf(stderr, "Erreur TTF_OpenFont : %s\n", TTF_GetError());
            SDL_DestroyWindow(*fenetre);
            SDL_DestroyRenderer(*rend);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
    return font ;
}
