#include <stdio.h>
#include <SDL2/SDL.h>

#define TITRE_FENETRE "jeu projet L2"
#define TAILLE_L 640
#define TAILLE_H 480

int main() {
    SDL_Window *fenetre;
    SDL_Renderer *rend;

    if (SDL_Init(SDL_INIT_VIDEO)) {
	fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
	exit(EXIT_FAILURE);
    }
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
    if (!(rend = SDL_CreateRenderer(fenetre, -1, 0))) {
	fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
	exit(EXIT_FAILURE);
    }
    SDL_RenderPresent(rend);
    SDL_Delay(5000);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

