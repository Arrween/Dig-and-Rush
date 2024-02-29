#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tour.h"
#include "constantes.h"
#include "entite.h"
#include "ressources.h"

int a_collision(t_entite * e1, t_entite * e2) {
    return SDL_HasIntersection(e1->affichage->rect_dst,
                               e2->affichage->rect_dst);
}

void defiler(t_entite * e, int dy) {
    e->affichage->rect_src->y += dy;
}

void boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int fin = 0;

    SDL_Texture * tex_obstacle;
    t_entite * fond, * fond_tour, * perso, * obstacle;

    int largeur_tour = TAILLE_L/2;
    int x_tour = (TAILLE_L-largeur_tour)/2;
    int x_mur_g = (int) (x_tour + largeur_tour * 0.07);
    int x_mur_d = (int) (x_tour + largeur_tour * 0.93);
    int largeur_perso_src = 64;
    int hauteur_perso_src = 64;
    int largeur_perso = largeur_perso_src;
    int hauteur_perso = hauteur_perso_src;

    tex_obstacle = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TAILLE_L, TAILLE_H);

    SDL_SetRenderTarget(rend, tex_obstacle);
    SDL_SetRenderDrawColor(rend, 180, 180, 80, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, NULL);
    obstacle = creer_entite_depuis_texture(tex_obstacle);
    obstacle->changer_rect_dst(obstacle, x_mur_g, TAILLE_H*.8, x_mur_d-x_mur_g, TAILLE_H*.05);
    obstacle->changer_pos(obstacle, 0, 80);

    fond = creer_entite("fond_menu");

    fond_tour = creer_entite("essai_fond_tour");
    fond_tour->changer_rect_src(fond_tour, 0, 3, 48, 41);
    fond_tour->changer_rect_dst(fond_tour,x_tour, 0, largeur_tour, TAILLE_H);

    perso = creer_entite("jack");
    perso->changer_dims_spritesheet(perso, 64, 64);
    perso->changer_pos_spritesheet(perso, 0, 6);
    perso->changer_rect_dst(perso, x_tour+2*largeur_perso, TAILLE_H/5, largeur_perso, hauteur_perso);

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
        perso->afficher(rend, perso);
        obstacle->afficher(rend, obstacle);
        
        if (!a_collision(perso, obstacle)) {
            obstacle->changer_pos_delta(obstacle, 0, -1);
            defiler(fond_tour, 1);
        }

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }

    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&perso);
}
