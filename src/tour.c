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
    int doit_boucler = VRAI;
    int repere_defilement = 0;
    long long compteur_frames = 0;
    int pas_defilement = 0;

    SDL_Texture * tex_obstacle;
    t_entite * fond, * fond_tour, * perso, * obstacle, * obstacle2;
    t_entite * obstacle3 = NULL, * obstacle4 = NULL;

    tex_obstacle = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TAILLE_L, TAILLE_H);
    SDL_SetRenderTarget(rend, tex_obstacle);
    SDL_SetRenderDrawColor(rend, 180, 180, 80, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_SetRenderTarget(rend, NULL);
    obstacle = creer_entite_depuis_texture(tex_obstacle, 0, 110, 50, 5, VRAI);
    obstacle2 = creer_entite_depuis_texture(tex_obstacle, 50, 170, 50, 5, VRAI);

    fond = creer_entite("menu_fond", -1, -1, -1, -1, FAUX);

    fond_tour = creer_entite("fond_tour", 0, 0, 100, 100, VRAI);
    fond_tour->changer_rect_src(fond_tour, 0, 0, 48, 80);

    perso = creer_entite_depuis_spritesheet("jack", 40, 20, 18, 12, VRAI);
    perso->changer_sprite(perso, X_PERSO_REPOS, Y_PERSO_REPOS);
    definir_animations(perso, 4, REPOS, DEPL_G, DEPL_D, CREUSER);

    changer_hitbox(perso, 26, 24, 51, 76);
    perso->doit_afficher_hitbox = VRAI;
    obstacle->doit_afficher_hitbox = VRAI;

    while (doit_boucler) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.repeat)
                        break;
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            doit_boucler = FAUX;
                            break;
                        case SDL_SCANCODE_H:
                            perso->doit_afficher_hitbox = ! perso->doit_afficher_hitbox;
                            break;
                        case SDL_SCANCODE_A:
                            perso->deplacement = GAUCHE;
                            perso->animation_courante = DEPL_G;
                            break;
                        case SDL_SCANCODE_D:
                            perso->deplacement = DROITE;
                            perso->animation_courante = DEPL_D;
                            break;
                        case SDL_SCANCODE_S:
                            if (perso->a_collision)
                                perso->animation_courante = CREUSER;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_S:
                            perso->deplacement = perso->animation_courante = REPOS;
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
        obstacle2->afficher(rend, obstacle2);
        
        verif_collision(perso, obstacle);
        if (!perso->a_collision)
            verif_collision(perso, obstacle2);
        deplacer(perso);
        animer(perso);

        if (!perso->a_collision) {
            if (VITESSE_CHUTE >= 1)
                pas_defilement = (int) VITESSE_CHUTE;
            else if (compteur_frames % (int) (1/(VITESSE_CHUTE)) == 0)
                pas_defilement = 1;
            else
                pas_defilement = 0;
            obstacle->changer_pos_rel(obstacle, 0, -pas_defilement);
            obstacle2->changer_pos_rel(obstacle2, 0, -pas_defilement);
            defiler(fond_tour, pas_defilement);
            repere_defilement = repere_defilement + pas_defilement;
        }

        if (repere_defilement > 100) {
            detruire_entite(&obstacle3);
            detruire_entite(&obstacle4);
            obstacle3 = creer_entite_depuis_texture(tex_obstacle, 0, 110, 50, 5, VRAI);
            obstacle4 = creer_entite_depuis_texture(tex_obstacle, 50, 170, 50, 5, VRAI);
        }
        if (repere_defilement > 200) {
            repere_defilement = 0;
            detruire_entite(&obstacle);
            detruire_entite(&obstacle2);
            obstacle = creer_entite_depuis_texture(tex_obstacle, 0, 110, 50, 5, VRAI);
            obstacle2 = creer_entite_depuis_texture(tex_obstacle, 50, 170, 50, 5, VRAI);
        }

        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);
        compteur_frames++;
    }

    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&perso);
    detruire_entite(&obstacle);
    detruire_entite(&obstacle2);
    detruire_entite(&obstacle3);
    detruire_entite(&obstacle4);
}
