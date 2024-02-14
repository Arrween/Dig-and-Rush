#include <stdio.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "tour.h"
#include "menu.h"

#define TITRE_FENETRE "jeu projet L2"
#define TAILLE_L 640
#define TAILLE_H 480
#define FPS 60

int main() {
    SDL_Window *fenetre;
    SDL_Renderer *rend;
    SDL_Event event;
    int fin = 0;

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

    while (!fin) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

		case SDL_QUIT:
		    fin = 1;
		    break;

		case SDL_KEYDOWN:
		    switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
			case SDL_SCANCODE_Q:
			    fin = 1;
			    break;
			default:
			    break;
		    }
	    }
	}

	SDL_RenderPresent(rend);
	SDL_Delay(1000/FPS);
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

