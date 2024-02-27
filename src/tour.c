#include <stdlib.h>

#include "tour.h"
#include "config.h"

typedef struct {
    SDL_Texture* texture;
} t_objet;

void init_jeu(SDL_Renderer * rend, t_objet *objets) {
}

void boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int fin = 0;
    t_objet *objets;

    SDL_Texture *fond, *fond_tour, *mur, *perso;
    SDL_Rect rect_mur, rect_perso;
    int largeur_tour = TAILLE_L/2;
    int x_tour = (TAILLE_L-largeur_tour)/2;
    SDL_Rect rect_fond_tour = {x_tour, 0, largeur_tour, TAILLE_H};
    int largeur_mur = TAILLE_L/2/8;
    int largeur_perso = TAILLE_L/20;
    int hauteur_perso = TAILLE_H/20;

    objets = malloc(sizeof(t_objet));

    fond = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TAILLE_L, TAILLE_H);
    fond_tour = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur_tour, TAILLE_H);
    mur = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur_mur, TAILLE_H);
    perso = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur_perso, hauteur_perso);

    SDL_SetRenderTarget(rend, fond);
    SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, fond_tour);
    SDL_SetRenderDrawColor(rend, 80, 80, 80, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, mur);
    SDL_SetRenderDrawColor(rend, 200, 200, 200, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, perso);
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderFillRect(rend, NULL);

    SDL_SetRenderTarget(rend, NULL);

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
        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, fond, NULL, NULL);
        SDL_RenderCopy(rend, fond_tour, NULL, &rect_fond_tour);
        rect_mur.x = x_tour;
        rect_mur.y = 0;
        rect_mur.w = largeur_mur;
        rect_mur.h = TAILLE_H;
        SDL_RenderCopy(rend, mur, NULL, &rect_mur);
        rect_mur.x = x_tour + largeur_tour - largeur_mur;
        rect_mur.y = 0;
        rect_mur.w = largeur_mur;
        rect_mur.h = TAILLE_H;
        SDL_RenderCopy(rend, mur, NULL, &rect_mur);
        rect_perso.x = x_tour + 3*largeur_mur;
        rect_perso.y = TAILLE_H/5;
        rect_perso.w = largeur_perso;
        rect_perso.h = hauteur_perso;
        SDL_RenderCopy(rend, perso, NULL, &rect_perso);

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }
}
