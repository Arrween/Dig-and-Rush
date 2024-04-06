#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "tour.h"
#include "constantes.h"
#include "entite.h"
#include "morceaux_niveau.h"
#include "listes.h"

#define N 10

// Vérifie les collisions d’une entité avec une liste d’autres
void verif_collision(t_entite * e1, int i_liste, float * correction_defilement) {
    SDL_FPoint e1_hg = {e1->hitbox.x, e1->hitbox.y + 0.05 * e1->hitbox.h};
    SDL_FPoint e1_hd = {e1->hitbox.x + e1->hitbox.w, e1->hitbox.y + 0.05 * e1->hitbox.h};
    SDL_FPoint e1_bg = {e1->hitbox.x, e1->hitbox.y + 0.8 * e1->hitbox.h};
    SDL_FPoint e1_bd = {e1->hitbox.x + e1->hitbox.w, e1->hitbox.y + 0.8 * e1->hitbox.h};
    SDL_FPoint e1_b = {e1->hitbox.x + 0.5 * e1->hitbox.w, e1->hitbox.y + e1->hitbox.h};

    e1->a_collision_g = FAUX;
    e1->a_collision_d = FAUX;
    e1->a_collision_h = FAUX;
    e1->a_collision_b = FAUX;

    en_tete(i_liste);
    while (!hors_liste(i_liste)) {
        t_entite * e2 = valeur_elt(i_liste);

        int collision_g = SDL_PointInFRect(&e1_hg, &(e2->hitbox)) || SDL_PointInFRect(&e1_bg, &(e2->hitbox));
        int collision_d = SDL_PointInFRect(&e1_hd, &(e2->hitbox)) || SDL_PointInFRect(&e1_bd, &(e2->hitbox));
        int collision_h = SDL_PointInFRect(&e1_hg, &(e2->hitbox)) || SDL_PointInFRect(&e1_hg, &(e2->hitbox));
        int collision_b = SDL_PointInFRect(&e1_b, &(e2->hitbox));
        e1->a_collision_g = e1->a_collision_g || collision_g;
        e1->a_collision_d = e1->a_collision_d || collision_d;
        e1->a_collision_h = e1->a_collision_h || collision_h;
        e1->a_collision_b = e1->a_collision_b || collision_b;

        if (collision_g && !collision_d) {
            float depassement = e2->hitbox.x + e2->hitbox.w - e1->hitbox.x;
            if (depassement >= 2)
                e1->changer_pos_rel(e1, depassement, 0);
        }
        if (collision_d && !collision_g) {
            float depassement = e2->hitbox.x - (e1->hitbox.x + e1->hitbox.w);
            if (depassement <= -2)
                e1->changer_pos_rel(e1, depassement, 0);
        }
        if (collision_b) {
            float depassement = e2->hitbox.y - (e1->hitbox.y + e1->hitbox.h);
            if (depassement <= 0)
                *correction_defilement = depassement;
        }


        suivant(i_liste);
    }
}


/**
 * @brief vérifie qu’une entité peut creuser une autre entité selon leurs positions
 * @param e entité essayant de creuser
 * @param bloc entité qu’on essaye de creuser
 * @return VRAI si l’entité `e` est en position de creuser l’entité `bloc`
 */
int verifier_peut_creuser(t_entite * e, t_entite * bloc) {
    if (strcmp(bloc->type, "bloc_terre") != 0)
        return FAUX;
    int e_x1 = e->hitbox.x;
    int e_x2 = e->hitbox.x + e->hitbox.w;
    int e_y2 = e->hitbox.y + e->hitbox.h;
    int b_x1 = bloc->hitbox.x;
    int b_x2 = bloc->hitbox.x + bloc->hitbox.w;
    int b_y1 = bloc->hitbox.y;
    // définit le dépassement horizontal autorisé pour que le creusage soit possible
    // en pourcentage de la largeur de l’entité
    int depassement_x = e->hitbox.w * 0.4;
    int depassement_y = 5;
    return e_x1 >= b_x1 - depassement_x &&
           e_x2 <= b_x2 + depassement_x &&
           e_y2 >= b_y1 &&
           e_y2 <= b_y1 + depassement_y;
}


int boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int doit_boucler = VRAI;
    long long compteur_frames = 0;
    float pas_defilement = 0;
    const int DUREE_CREUSER = 8; // Nombre de frames de l'animation "creuser"
    int i_liste = 0;
    t_entite * entite_courante;

    srand(time(NULL));

    init_liste(i_liste);

    int doit_quitter = FAUX;

    t_entite * fond, * fond_tour, * fond_tour_2, * perso;

    // Initialisation des entités de fond et de personnage
    fond = creer_entite("fond_jeu", -1, -1, -1, -1, FAUX);
    fond_tour = creer_entite("fond_tour", 0, 0, 100, 100, VRAI);
    fond_tour_2 = creer_entite("fond_tour", 0, 100, 100, 100, VRAI);

    perso = creer_entite_depuis_spritesheet("matt", 40, 20, 18, 12, VRAI);
    
    generer_morceau_niveau(i_liste);

    changer_hitbox(perso, 26, 22, 51, 74);
    perso->doit_afficher_hitbox = VRAI;

    // Boucle de jeu principale
    while (doit_boucler) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    doit_boucler = FAUX;
                    doit_quitter = VRAI ;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.repeat) break;
                    // Gestion des touches du clavier
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            doit_boucler = FAUX;
                            break;
                        case SDL_SCANCODE_H:
                            perso->doit_afficher_hitbox = !perso->doit_afficher_hitbox;
                            break;
                        case SDL_SCANCODE_A:
                            perso->deplacement = GAUCHE;
                            if (perso->a_collision_b)
                                changer_animation(perso, DEPL_G);
                            break;
                        case SDL_SCANCODE_D:
                            perso->deplacement = DROITE;
                            if (perso->a_collision_b)
                                changer_animation(perso, DEPL_D);
                            break;
                        case SDL_SCANCODE_S:
                            if (perso->a_collision_b){
                                changer_animation(perso, CREUSER); 
                                en_queue(i_liste);
                                while(!hors_liste(i_liste)) {
                                    entite_courante = valeur_elt(i_liste);
                                    if (verifier_peut_creuser(perso, entite_courante))
                                        oter_elt(i_liste);
                                    else
                                        precedent(i_liste);
                                }
                            }
                            break;
                        case SDL_SCANCODE_W:
                            if (perso->a_collision_b) {
                                if (perso->sens_regard == GAUCHE)
                                    changer_animation(perso, ATTQ_G);
                                else if (perso->sens_regard == DROITE)
                                    changer_animation(perso, ATTQ_D);
                            }
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
                        case SDL_SCANCODE_W:
                            if (perso->a_collision_b)
                                changer_animation(perso, REPOS);
                            perso->deplacement = REPOS_MVT;
                            break;
                        default:
                            break;
                    }
            }
        }

        // Affichage des entités
        SDL_RenderClear(rend);

        fond->afficher(rend, fond);
        fond_tour->afficher(rend, fond_tour);
        fond_tour_2->afficher(rend, fond_tour_2);
        perso->afficher(rend, perso);

        en_tete(i_liste);
        while (!hors_liste(i_liste)) {
            entite_courante = valeur_elt(i_liste);
            entite_courante->afficher(rend, entite_courante);
            suivant(i_liste);
        }

        // Gestion des collisions
        float correction_defilement = 0;
        verif_collision(perso, i_liste, &correction_defilement);

        // Déplacement et animation du personnage
        if (! perso->a_collision_b)
            changer_animation(perso, perso->sens_regard == GAUCHE ? CHUTE_G : CHUTE_D);
        deplacer(perso);
        animer(perso, compteur_frames);

        // Logique de défilement des obstacles
        if (!perso->a_collision_b) {
            // Calcul du pas de défilement en fonction de la vitesse de chute et du compteur de frames
            if (VITESSE_CHUTE >= 1) {
                pas_defilement = VITESSE_CHUTE;
            } else if (compteur_frames % (int)(1 / (VITESSE_CHUTE)) == 0) {
                pas_defilement = 1;
            } else {
                pas_defilement = 0;
            }
        }
        else {
            pas_defilement = correction_defilement;
        }

        // Déplacement relatif des obstacles pour simuler le défilement
        en_tete(i_liste);
        while (!hors_liste(i_liste)) {
            entite_courante = valeur_elt(i_liste);
            entite_courante->changer_pos_rel(entite_courante, 0, -pas_defilement);
            suivant(i_liste);
        }
        if (!perso->a_collision_b) {
            fond_tour->changer_pos_rel(fond_tour, 0, -pas_defilement);
            fond_tour_2->changer_pos_rel(fond_tour_2, 0, -pas_defilement);
        }

        // Génération de nouvelles entités et des fonds de tour alternés une fois qu’une hauteur de tour a défilé
        float bas_fond_tour = fond_tour->rect_dst->y + fond_tour->rect_dst->h;
        float bas_fond_tour_2 = fond_tour_2->rect_dst->y + fond_tour_2->rect_dst->h;
        if (bas_fond_tour < 0) {
            detruire_entite(&fond_tour);
            fond_tour = creer_entite("fond_tour", 0, bas_fond_tour_2, 100, 100, VRAI);
        }
        if (bas_fond_tour_2 < 0) {
            detruire_entite(&fond_tour_2);
            fond_tour_2 = creer_entite("fond_tour", 0, bas_fond_tour, 100, 100, VRAI);
        }
        if (bas_fond_tour < 0 || bas_fond_tour_2 < 0) {
            // suppression des entités ayant défilé au-dessus de la zone de jeu
            en_queue(i_liste);
            while (!hors_liste(i_liste)) {
                entite_courante = valeur_elt(i_liste);
                if (entite_courante->rect_dst->y + entite_courante->rect_dst->h < 0) {
                    oter_elt(i_liste);
                    detruire_entite(&entite_courante);
                }
                else
                    precedent(i_liste);
            }

            generer_morceau_niveau(i_liste);
        }

        SDL_RenderPresent(rend);
        SDL_Delay(1000 / FPS); // Contrôle du taux de rafraîchissement
        compteur_frames++;
    }

    // Libération des ressources
    en_queue(i_liste);
    while (!hors_liste(i_liste)) {
        entite_courante = valeur_elt(i_liste);
        detruire_entite(&entite_courante);
        oter_elt(i_liste);
    }
    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&fond_tour_2);
    detruire_entite(&perso);

    return doit_quitter ;
}
