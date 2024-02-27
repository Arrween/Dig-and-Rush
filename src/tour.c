#include "tour.h"

void boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int fin = 0;

    while (!fin) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

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
}
