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
    e->affichage->rect_src->y = (e->affichage->rect_src->y + dy) % 80;
}

void boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int doit_boucler = SDL_TRUE;
    int repere_defilement = 0;

    SDL_Texture * tex_obstacle;
    t_entite * fond, * fond_tour, * perso, * obstacle;

    tex_obstacle = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TAILLE_L, TAILLE_H);
    SDL_SetRenderTarget(rend, tex_obstacle);
    SDL_SetRenderDrawColor(rend, 180, 180, 80, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, NULL);
    obstacle = creer_entite_depuis_texture(tex_obstacle, 0, 110, 50, 5, SDL_TRUE);

    fond = creer_entite("fond_menu", -1, -1, -1, -1, SDL_FALSE);

    fond_tour = creer_entite("fond_tour", 0, 0, 100, 100, SDL_TRUE);
    fond_tour->changer_rect_src(fond_tour, 0, 0, 48, 80);

    perso = creer_entite_depuis_spritesheet("jack", 40, 20, 18, 12, SDL_TRUE);
    perso->changer_sprite(perso, 0, 6);

    int x_sprite = 0;
    int y_sprite = 0;
    int doit_deplacer_g = 0;
    int doit_deplacer_d = 0;

    while (doit_boucler) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.repeat)
                        break;
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            doit_boucler = SDL_FALSE;
                            break;
                        case SDL_SCANCODE_A:
                            doit_deplacer_g = 1;
                            break;
                        case SDL_SCANCODE_D:
                            doit_deplacer_d = 1;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_A:
                            doit_deplacer_g = 0;
                            perso->changer_sprite(perso, 0, 6);
                            break;
                        case SDL_SCANCODE_D:
                            doit_deplacer_d = 0;
                            perso->changer_sprite(perso, 0, 6);
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
        
        if (doit_deplacer_g || doit_deplacer_d) {
            int dx = doit_deplacer_g ? -1 : 1;
            int y_sprite_nouv = doit_deplacer_g ? 9 : 11;
            perso->deplacer_rel(perso, dx, 0);
            if (y_sprite != y_sprite_nouv) {
                x_sprite = 0;
                y_sprite = y_sprite_nouv;
            }
            else
                x_sprite = (x_sprite + 1) % 9;
            perso->changer_sprite(perso, x_sprite, y_sprite);
        }

        if (!a_collision(perso, obstacle)) {
            obstacle->deplacer_rel(obstacle, 0, -1);
            defiler(fond_tour, 1);
            repere_defilement++;
        }

        if (repere_defilement > 100) {
        }

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
    }

    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&perso);
}
