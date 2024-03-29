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

// Vérifie si deux entités se chevauchent
void verif_collision(t_entite * e1, t_entite * e2) {
    int a_collision = SDL_HasIntersection(&(e1->hitbox), &(e2->hitbox));
    e1->a_collision = e2->a_collision = a_collision;
}

int boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int doit_boucler = VRAI;
    int repere_defilement = 0;
    long long compteur_frames = 0;
    int pas_defilement = 0;
    int parite_defilement = 0;
    const int DUREE_CREUSER = 8; // Nombre de frames de l'animation "creuser"
    int i_liste = 0;
    t_entite * entite_courante;

    srand(time(NULL));

    init_liste(i_liste);

    int doit_quitter = FAUX;

    t_entite * fond, * fond_tour, * fond_tour_2, * perso;

    t_entite ** morceau_niveau = choisir_morceau_niveau();
    for (int i = 0; morceau_niveau[i] != NULL; i++)
        ajout_droit(i_liste, morceau_niveau[i]);

    // Initialisation des entités de fond et de personnage
    fond = creer_entite("fond_jeu", -1, -1, -1, -1, FAUX);
    fond_tour = creer_entite("fond_tour", 0, 0, 100, 100, VRAI);
    fond_tour_2 = creer_entite("fond_tour", 0, 100, 100, 100, VRAI);

    perso = creer_entite_depuis_spritesheet("matt", 40, 20, 18, 12, VRAI);
    
    changer_hitbox(perso, 26, 24, 51, 76);
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
                            printf("taille liste demandée %i\n", taille_liste(i_liste));
                            perso->doit_afficher_hitbox = !perso->doit_afficher_hitbox;
                            break;
                        case SDL_SCANCODE_A:
                            perso->deplacement = GAUCHE;
                            if (perso->a_collision)
                                changer_animation(perso, DEPL_G);
                            break;
                        case SDL_SCANCODE_D:
                            perso->deplacement = DROITE;
                            if (perso->a_collision)
                                changer_animation(perso, DEPL_D);
                            break;
                        case SDL_SCANCODE_S:
                            if (perso->a_collision){
                                changer_animation(perso, CREUSER); 
                                en_tete(i_liste);
                                while(!hors_liste(i_liste)) {
                                    entite_courante = valeur_elt(i_liste);
                                    if (strcmp(entite_courante->type, "bloc_terre") == 0 && SDL_HasIntersection(&(perso->hitbox), &(entite_courante->hitbox)))
                                        oter_elt(i_liste);
                                    suivant(i_liste);
                                }
                            }
                            break;
                        case SDL_SCANCODE_W:
                            if (perso->a_collision) {
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
                            if (perso->a_collision)
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
        perso->a_collision = FAUX;
        en_tete(i_liste);
        while (!hors_liste(i_liste)) {
            entite_courante = valeur_elt(i_liste);
            if (!perso->a_collision)
                verif_collision(perso, entite_courante);
            entite_courante->afficher(rend, entite_courante);
            suivant(i_liste);
        }

        // Déplacement et animation du personnage
        if (! perso->a_collision)
            changer_animation(perso, perso->sens_regard == GAUCHE ? CHUTE_G : CHUTE_D);
        deplacer(perso);
        animer(perso, compteur_frames);

        // Logique de défilement des obstacles
        if (!perso->a_collision) {
            // Calcul du pas de défilement en fonction de la vitesse de chute et du compteur de frames
            if (VITESSE_CHUTE >= 1) {
                pas_defilement = (int)VITESSE_CHUTE;
            } else if (compteur_frames % (int)(1 / (VITESSE_CHUTE)) == 0) {
                pas_defilement = 1;
            } else {
                pas_defilement = 0;
            }

            // Déplacement relatif des obstacles pour simuler le défilement
            en_tete(i_liste);
            while (!hors_liste(i_liste)) {
                entite_courante = valeur_elt(i_liste);
                entite_courante->changer_pos_rel(entite_courante, 0, -pas_defilement);
                suivant(i_liste);
            }

            fond_tour->changer_pos_rel(fond_tour, 0, -pas_defilement);
            fond_tour_2->changer_pos_rel(fond_tour_2, 0, -pas_defilement);

            repere_defilement += pas_defilement;
        }

        // Recréation des obstacles pour simuler un nouveau set d'obstacles après un certain défilement
        if (repere_defilement > 100 && parite_defilement == 0) {
            parite_defilement = 1;
            detruire_entite(&fond_tour);

            printf("taille liste avant destructions %i\n", taille_liste(i_liste));
            afficher_liste(i_liste);
            en_tete(i_liste);
            while (!hors_liste(i_liste)) {
                entite_courante = valeur_elt(i_liste);
                if (entite_courante->rect_dst->y + entite_courante->rect_dst->h < 0) {
                    oter_elt(i_liste);
                    detruire_entite(&entite_courante);
                }
                else
                    suivant(i_liste);
            }
            printf("taille liste après destructions %i\n", taille_liste(i_liste));
            afficher_liste(i_liste);

            en_tete(i_liste);
            morceau_niveau = choisir_morceau_niveau();
            for (int i = 0; morceau_niveau[i] != NULL; i++)
                ajout_droit(i_liste, morceau_niveau[i]);

            printf("taille liste après remplissage %i\n", taille_liste(i_liste));
            afficher_liste(i_liste);

            fond_tour = creer_entite("fond_tour",
                             0, fond_tour_2->rect_dst->y + fond_tour_2->rect_dst->h,
                             100, 100, VRAI);
        }
        if (repere_defilement >= 200 && parite_defilement == 1) {
            parite_defilement = 0;
            repere_defilement = 0;
            detruire_entite(&fond_tour_2);

            printf("taille liste avant destructions %i\n", taille_liste(i_liste));
            afficher_liste(i_liste);
            en_tete(i_liste);
            while (!hors_liste(i_liste)) {
                entite_courante = valeur_elt(i_liste);
                printf("%i + %i = %i\n", entite_courante->rect_dst->y, entite_courante->rect_dst->h, entite_courante->rect_dst->y + entite_courante->rect_dst->h);
                if (entite_courante->rect_dst->y + entite_courante->rect_dst->h < 0) {
                    oter_elt(i_liste);
                    detruire_entite(&entite_courante);
                }
                else 
                    suivant(i_liste);
            }
            printf("taille liste après destructions %i\n", taille_liste(i_liste));
            afficher_liste(i_liste);

            en_tete(i_liste);
            morceau_niveau = choisir_morceau_niveau();
            for (int i = 0; morceau_niveau[i] != NULL; i++)
                ajout_droit(i_liste, morceau_niveau[i]);
            printf("taille liste après remplissage %i\n", taille_liste(i_liste));
            afficher_liste(i_liste);

            fond_tour_2 = creer_entite("fond_tour",
                             0, fond_tour->rect_dst->y + fond_tour->rect_dst->h,
                             100, 100, VRAI);
        }

        SDL_RenderPresent(rend);
        SDL_Delay(1000 / FPS); // Contrôle du taux de rafraîchissement
        compteur_frames++;
    }

    // Libération des ressources
    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&fond_tour_2);
    detruire_entite(&perso);

    return doit_quitter ;
}
