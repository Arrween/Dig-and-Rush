#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tour.h"
#include "constantes.h"
#include "entite.h"
#include "ressources.h"
#include "spritesheets.h"

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
    int collision_detectee = SDL_FALSE;

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

    int x_sprite = 0;
    int y_sprite = 0;
    int sens_regard = DROITE;
    int est_en_deplacement = SDL_FALSE;
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
                            sens_regard = GAUCHE;
                            est_en_deplacement = SDL_TRUE;
                            break;
                        case SDL_SCANCODE_D:
                            sens_regard = DROITE;
                            est_en_deplacement = SDL_TRUE;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_D:
                            est_en_deplacement = SDL_FALSE;
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
        
        collision_detectee = a_collision(perso, obstacle);
        if (est_en_deplacement) {
            int dx = sens_regard == GAUCHE ? -1 : 1;
            perso->deplacer_rel(perso, dx, 0);
            if (collision_detectee) {
                y_sprite = sens_regard == GAUCHE ?
                            Y_PERSO_PELLE_MARCHE_G : Y_PERSO_PELLE_MARCHE_D;
                x_sprite = (x_sprite + 1) % LONGUEUR_ANIM_MARCHE;
            }
            else {
                y_sprite = sens_regard == GAUCHE ?
                            Y_PERSO_CHUTE_G : Y_PERSO_CHUTE_D;
                x_sprite = X_PERSO_CHUTE;
            }
            perso->changer_sprite(perso, x_sprite, y_sprite);
        }
        else {
            if (collision_detectee)
                perso->changer_sprite(perso, X_PERSO_REPOS, Y_PERSO_REPOS);
            else
                perso->changer_sprite(perso, X_PERSO_CHUTE,
                                      sens_regard == GAUCHE ?
                                        Y_PERSO_CHUTE_G : Y_PERSO_CHUTE_D);
        }

        if (!collision_detectee) {
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
