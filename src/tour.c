#include <stdlib.h>

#include "tour.h"
#include "constantes.h"
#include "affichage.h"

void boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int fin = 0;

    SDL_Texture * fond, * fond_tour, * mur, * perso;
    t_affichage * aff_fond, * aff_fond_tour, * aff_mur_g, * aff_mur_d, * aff_perso;

    int largeur_tour = TAILLE_L/2;
    int x_tour = (TAILLE_L-largeur_tour)/2;
    int largeur_mur = TAILLE_L/2/8;
    int largeur_perso = TAILLE_L/20;
    int hauteur_perso = TAILLE_H/20;

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

    aff_fond = creer_affichage(fond, -1, -1, -1, -1);
    aff_fond_tour = creer_affichage(fond_tour, x_tour, 0, largeur_tour, TAILLE_H);
    aff_mur_g = creer_affichage(mur, x_tour, 0, largeur_mur, TAILLE_H);
    aff_mur_d = creer_affichage(mur, x_tour + largeur_tour - largeur_mur, 0, largeur_mur, TAILLE_H);
    aff_perso = creer_affichage(perso, x_tour + 3*largeur_mur, TAILLE_H/5, largeur_perso, hauteur_perso);

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

        aff_fond->render(rend, aff_fond);
        aff_fond_tour->render(rend, aff_fond_tour);
        aff_mur_g->render(rend, aff_mur_g);
        aff_mur_d->render(rend, aff_mur_d);
        aff_perso->render(rend, aff_perso);

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }

    detruire_affichage(&aff_fond);
    detruire_affichage(&aff_fond_tour);
    detruire_affichage(&aff_mur_g);
    detruire_affichage(&aff_mur_d);
    detruire_affichage(&aff_perso);
}
