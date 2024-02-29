#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tour.h"
#include "constantes.h"
#include "entite.h"
#include "ressources.h"

void boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int fin = 0;

    SDL_Texture * tex_fond_tour, * tex_mur;
    t_entite * fond, * perso;
    t_entite * fond_tour, * mur_g, * mur_d;

    int largeur_tour = TAILLE_L/2;
    int x_tour = (TAILLE_L-largeur_tour)/2;
    int largeur_mur = TAILLE_L/2/8;
    int largeur_perso_src = 64;
    int hauteur_perso_src = 64;
    int largeur_perso = largeur_perso_src;
    int hauteur_perso = hauteur_perso_src;

    tex_fond_tour = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur_tour, TAILLE_H);
    tex_mur = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur_mur, TAILLE_H);

    SDL_SetRenderTarget(rend, tex_fond_tour);
    SDL_SetRenderDrawColor(rend, 80, 80, 80, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, tex_mur);
    SDL_SetRenderDrawColor(rend, 200, 200, 200, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, NULL);

    fond = creer_entite("fond_menu");

    fond_tour = creer_entite_depuis_texture(tex_fond_tour);
    fond_tour->changer_rect_dst(fond_tour,x_tour, 0, largeur_tour, TAILLE_H);
    mur_g = creer_entite_depuis_texture(tex_mur);
    mur_g->changer_rect_dst(mur_g, x_tour, 0, largeur_mur, TAILLE_H);
    mur_d = creer_entite_depuis_texture(tex_mur);
    mur_d->changer_rect_dst(mur_d, x_tour+largeur_tour-largeur_mur, 0, largeur_mur, TAILLE_H);

    perso = creer_entite("jack");
    perso->changer_dims_spritesheet(perso, 64, 64);
    perso->changer_pos_spritesheet(perso, 0, 6);
    perso->changer_rect_dst(perso, x_tour+3*largeur_mur, TAILLE_H/5, largeur_perso, hauteur_perso);

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

        fond->afficher(rend, fond);
        fond_tour->afficher(rend, fond_tour);
        mur_g->afficher(rend, mur_g);
        mur_d->afficher(rend, mur_d);
        perso->afficher(rend, perso);

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }

    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&mur_g);
    detruire_entite(&mur_d);
    detruire_entite(&perso);
}
