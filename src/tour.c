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

    t_entite * fond, * fond_tour, * perso;

    int largeur_tour = TAILLE_L/2;
    int x_tour = (TAILLE_L-largeur_tour)/2;
    int largeur_perso_src = 64;
    int hauteur_perso_src = 64;
    int largeur_perso = largeur_perso_src;
    int hauteur_perso = hauteur_perso_src;

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

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }

    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&perso);
}
