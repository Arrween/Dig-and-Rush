#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tour.h"
#include "constantes.h"
#include "entite.h"
#include "ressources.h"
#include "spritesheets.h"

void verif_collision(t_entite * e1, t_entite * e2) {
    int a_collision = SDL_HasIntersection(&(e1->hitbox), &(e2->hitbox));
    e1->a_collision = e2->a_collision = a_collision;
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
    perso->changer_sprite(perso, X_PERSO_REPOS, Y_PERSO_REPOS);

    changer_hitbox(perso, 26, 24, 51, 76);
    perso->doit_afficher_hitbox = SDL_TRUE;
    obstacle->doit_afficher_hitbox = SDL_TRUE;

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
                            perso->deplacement = GAUCHE;
                            break;
                        case SDL_SCANCODE_D:
                            perso->deplacement = DROITE;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_D:
                            perso->deplacement = REPOS;
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
        
        verif_collision(perso, obstacle);
        deplacer(perso);

        if (!perso->a_collision) {
            obstacle->changer_pos_rel(obstacle, 0, -1);
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
