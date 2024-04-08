#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "tour.h"
#include "constantes.h"
#include "entite.h"
#include "entite_destructible.h"
#include "entite_pnj.h"
#include "morceaux_niveau.h"
#include "listes.h"

#define MAX_RAYON_OMBRE (TAILLE_H)
#define FACTEUR_MIN_RAYON_OMBRE 4 // multipli par la largeur du personnage
#define FACTEUR_OBSCURCISSEMENT 6.
#define PAS_RAYON_OMBRE 18

#define PAS_ALPHA_FOND 7

#define N 10

/**
 * @brief vérifie les collisions d’une entité avec les obstacles
 * @param e1 entité possiblement en collision
 * @param correction_defilement valeur à retourner si `e1` est le personnage joueur
 */
void verif_collision(t_entite * e1, float * correction_defilement) {
    // points haut gauche, haut droit etc., décalés sur l’axe vertical pour ne pas déclencher
    // une collision à la fois sur la gauche, la droite et le bas
    SDL_FPoint e1_hg = {e1->hitbox.x, e1->hitbox.y + 0.05 * e1->hitbox.h};
    SDL_FPoint e1_hd = {e1->hitbox.x + e1->hitbox.w, e1->hitbox.y + 0.05 * e1->hitbox.h};
    SDL_FPoint e1_bg = {e1->hitbox.x, e1->hitbox.y + 0.8 * e1->hitbox.h};
    SDL_FPoint e1_bd = {e1->hitbox.x + e1->hitbox.w, e1->hitbox.y + 0.8 * e1->hitbox.h};
    // point au milieu du bas de la hitbox
    SDL_FPoint e1_b = {e1->hitbox.x + 0.5 * e1->hitbox.w, e1->hitbox.y + e1->hitbox.h};

    e1->a_collision_g = FAUX;
    e1->a_collision_d = FAUX;
    e1->a_collision_h = FAUX;
    e1->a_collision_b = FAUX;

    en_tete(I_LISTE_ENTITES);
    while (!hors_liste(I_LISTE_ENTITES)) {
        t_entite * e2 = valeur_elt(I_LISTE_ENTITES);
        
        if (!e2->est_obstacle) {
            suivant(I_LISTE_ENTITES);
            continue;
        }

        // il y a collision à gauche, droite ou en haut si les deux points correspondants sont
        // dans la hitbox de l’entité candidate
        int collision_g = SDL_PointInFRect(&e1_hg, &(e2->hitbox)) || SDL_PointInFRect(&e1_bg, &(e2->hitbox));
        int collision_d = SDL_PointInFRect(&e1_hd, &(e2->hitbox)) || SDL_PointInFRect(&e1_bd, &(e2->hitbox));
        int collision_h = SDL_PointInFRect(&e1_hg, &(e2->hitbox)) || SDL_PointInFRect(&e1_hg, &(e2->hitbox));
        // il y a collision en bas si le point au milieu du bas de la hitbox et dans la hitbox candidate
        int collision_b = SDL_PointInFRect(&e1_b, &(e2->hitbox));
        // l’entité a une collision à gauche si elle est en collision à gauche avec au moins une entité de la liste
        // idem pour droite, haut, bas
        e1->a_collision_g = e1->a_collision_g || collision_g;
        e1->a_collision_d = e1->a_collision_d || collision_d;
        e1->a_collision_h = e1->a_collision_h || collision_h;
        e1->a_collision_b = e1->a_collision_b || collision_b;

        // replacement de l’entité si le chevauchement avec l’entité en collision est non négligeable
        if (collision_g && !collision_d) {
            float depassement = e2->hitbox.x + e2->hitbox.w - e1->hitbox.x;
            if (depassement >= 2)
                changer_pos_rel(e1, depassement, 0);
        }
        if (collision_d && !collision_g) {
            float depassement = e2->hitbox.x - (e1->hitbox.x + e1->hitbox.w);
            if (depassement <= -2)
                changer_pos_rel(e1, depassement, 0);
        }
        // replacement pour le chevauchement par le bas, utilisation du défilement car on suppose ici
        // que l’entité est le personnage joueur (à adapter quand nécessaire)
        if (correction_defilement != NULL && collision_b) {
            float depassement = e2->hitbox.y - (e1->hitbox.y + e1->hitbox.h);
            if (depassement <= 0)
                *correction_defilement = depassement;
        }


        suivant(I_LISTE_ENTITES);
    }
}

void porter_coup(t_entite * e) {
    en_tete(I_LISTE_ENTITES);
    while (!hors_liste(I_LISTE_ENTITES)) {
        t_entite * elem = valeur_elt(I_LISTE_ENTITES);
        if (elem->pnj) {
            // SDL_FRect hitbox_attaque = e->animation_courante->hitboxes_attaque[e->x_sprite];
            if (!elem->pnj->est_mort && SDL_HasIntersectionF(&e->hitbox, &elem->hitbox)) {
                elem->id_animation_suivante = ANIM_MORT_STATIQUE;
                changer_animation(elem, ANIM_MORT);
                elem->deplacement = REPOS_MVT;
                elem->pnj->est_mort = VRAI;
            }
        }
        suivant(I_LISTE_ENTITES);
    }
}

/**
 * @brief vérifie qu’une entité peut creuser une autre entité selon leurs positions
 * @param e entité essayant de creuser
 * @param bloc entité qu’on essaye de creuser
 * @return VRAI si l’entité `e` est en position de creuser l’entité `bloc`
 */
int verifier_peut_creuser(t_entite * e, t_entite * bloc) {
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

void calculer_ombre(SDL_Texture * tex_ombre, int rayon,
                    t_entite * perso,
                    SDL_FRect rect_zone) {
    SDL_PixelFormat * format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    Uint32 * pixels;
    int pitch;
    // permettre d’écrire les pixels de la texture
    SDL_LockTexture(tex_ombre, NULL, (void**)&pixels, &pitch);

    SDL_FRect rect_perso_abs;

    // gestion manuelle du décalage pour les animations d’attaque, pas idéal
    float w, h;
    if (perso->animation_courante->id == ATTQ_G || perso->animation_courante->id == ATTQ_D) {
        SDL_FRect rect_perso_decale = {perso->rect_dst->x + perso->animation_courante->decalage_dest_x, perso->rect_dst->y + perso->animation_courante->decalage_dest_y, perso->rect_dst->w, perso->rect_dst->h};
        rect_perso_abs = convertir_vers_absolu(&rect_perso_decale, rect_zone);
        w = rect_perso_abs.w / 3;
        h = rect_perso_abs.h / 3;
    }
    else {
        rect_perso_abs = convertir_vers_absolu(perso->rect_dst, rect_zone);
        w = rect_perso_abs.w;
        h = rect_perso_abs.h;
    }

    for (int i = rect_zone.x; i < rect_zone.x + rect_zone.w; i++) {
        for (int j = rect_zone.y; j < rect_zone.y + rect_zone.h; j++) {
            int dx = (i-(rect_perso_abs.x+w/2));
            int dy = (j-(rect_perso_abs.y+h/2));
            int carre_distance = dx*dx + dy*dy;
            // si la distance euclidienne entre le point (i,j) de la zone
            // de jeu et le centre du personnage est plus grande que `rayon`
            // écrire un pixel d’autant moins transparent qu’il est loin
            if (carre_distance > rayon*rayon) {
                float alpha = ((float)(carre_distance - rayon*rayon)) / (TAILLE_L/2*TAILLE_L/2 + TAILLE_H/2*TAILLE_H/2) * 255. * FACTEUR_OBSCURCISSEMENT;
                pixels[j * TAILLE_L + i] = SDL_MapRGBA(format, 0, 0, 0, alpha < 255 ? alpha : 255);
            }
            // sinon écrire un pixel transparent
            else
                pixels[j * TAILLE_L + i] = SDL_MapRGBA(format, 0, 0, 0, 0);
        }
    }

    // fin de l’écriture des pixels
    SDL_UnlockTexture(tex_ombre);
    SDL_FreeFormat(format);
}


int boucle_jeu(SDL_Renderer * rend) {
    SDL_Event event;
    int doit_boucler = VRAI;
    long long compteur_frames = 0;
    float pas_defilement = 0;
    const int DUREE_CREUSER = 8; // Nombre de frames de l'animation "creuser"

    srand(time(NULL));

    init_liste(I_LISTE_ENTITES);

    int doit_quitter = FAUX;

    t_entite * fond, * fond_tour, * fond_tour_2, * perso;
    t_entite * fond_nuit;

    // Initialisation des entités de fond et de personnage
    fond = creer_entite("fond_jeu", -1, -1, -1, -1, FAUX);
    fond_nuit = creer_entite("fond_jeu_nuit", -1, -1, -1, -1, FAUX);
    fond_tour = creer_entite("fond_tour", 0, 0, 100, 100, VRAI);
    fond_tour_2 = creer_entite("fond_tour", 0, 100, 100, 100, VRAI);

    perso = creer_entite_depuis_spritesheet("matt", 40, 20, 15, 12, VRAI);
    perso->vitesse = 1;
    
    generer_morceau_niveau(-1);

    generer_murs();

    changer_hitbox(perso, 26, 22, 51, 74);

    perso->doit_afficher_hitbox = VRAI;
    int lumiere_est_allumee = VRAI;
    int lumiere_est_allumee_prec = FAUX;

    // texture pour afficher une ombre quand la lumière est éteinte
    SDL_FRect zone_jeu = {TAILLE_L/4, 0, TAILLE_L/2, TAILLE_H};
    SDL_Texture * tex_ombre = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TAILLE_L, TAILLE_H);
    SDL_SetTextureBlendMode(tex_ombre, SDL_BLENDMODE_BLEND);
    int rayon_ombre = FACTEUR_MIN_RAYON_OMBRE * perso->rect_dst->w;
    SDL_SetTextureBlendMode(fond->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(fond_nuit->texture, SDL_BLENDMODE_BLEND);
    int alpha_fond = 0;

    // compteur de FPS
    SDL_Color couleur_fps = {0,0,0,255};
    TTF_Font * police = TTF_OpenFont("ressources/Menu/Police/font1.ttf", 50);
    SDL_Surface * surface_fps = TTF_RenderText_Solid(police, "", couleur_fps);
    SDL_Rect dst_fps = {20, TAILLE_H-40, 100, 30};
    SDL_Texture * tex_fps = NULL;

    // chronométrage du temps de chaque frame
    clock_t chrono_deb, chrono_fin;
    int microsec_par_frame;

    // Boucle de jeu principale
    while (doit_boucler) {
        chrono_deb = clock();
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
                        case SDL_SCANCODE_L:
                            lumiere_est_allumee = !lumiere_est_allumee;
                            lumiere_est_allumee_prec = !lumiere_est_allumee;
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
                                en_queue(I_LISTE_ENTITES);
                                while(!hors_liste(I_LISTE_ENTITES)) {
                                    t_entite * elem = valeur_elt(I_LISTE_ENTITES);
                                    if (elem->destructible && verifier_peut_creuser(perso, elem)) {
                                        elem->destructible->action_destruction();
                                        oter_elt(I_LISTE_ENTITES);
                                        detruire_entite(&elem);
                                    }
                                    else
                                        precedent(I_LISTE_ENTITES);
                                }
                            }
                            break;
                        case SDL_SCANCODE_W:
                            if (perso->a_collision_b) {
                                if (perso->sens_regard == GAUCHE)
                                    changer_animation(perso, ATTQ_G);
                                else if (perso->sens_regard == DROITE)
                                    changer_animation(perso, ATTQ_D);
                                porter_coup(perso);
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

        SDL_RenderClear(rend);

        if (lumiere_est_allumee && !lumiere_est_allumee_prec) {
            alpha_fond += PAS_ALPHA_FOND;
            if (alpha_fond >= 255)
                alpha_fond = 255;

            rayon_ombre += PAS_RAYON_OMBRE;
            if (rayon_ombre >= MAX_RAYON_OMBRE)
                rayon_ombre = MAX_RAYON_OMBRE;
            calculer_ombre(tex_ombre, rayon_ombre, perso, zone_jeu);

            if (alpha_fond >= 255 && rayon_ombre >= MAX_RAYON_OMBRE)
                lumiere_est_allumee_prec = VRAI;
        }
        else if (!lumiere_est_allumee && lumiere_est_allumee_prec) {
            alpha_fond -= PAS_ALPHA_FOND;
            if (alpha_fond <= 0)
                alpha_fond = 0;

            rayon_ombre -= PAS_RAYON_OMBRE;
            if (rayon_ombre <= FACTEUR_MIN_RAYON_OMBRE*perso->rect_dst->w)
                rayon_ombre = FACTEUR_MIN_RAYON_OMBRE * perso->rect_dst->w;
            calculer_ombre(tex_ombre, rayon_ombre, perso, zone_jeu);

            if (alpha_fond <= 0 && rayon_ombre <= FACTEUR_MIN_RAYON_OMBRE*perso->rect_dst->w)
                lumiere_est_allumee_prec = FAUX;
        }
        else if (!lumiere_est_allumee) {
            calculer_ombre(tex_ombre, rayon_ombre, perso, zone_jeu);
        }

        // Affichage des entités
        SDL_SetTextureAlphaMod(fond->texture, alpha_fond);
        SDL_SetTextureAlphaMod(fond_nuit->texture, 255-alpha_fond);
        afficher_entite(rend, fond);
        afficher_entite(rend, fond_nuit);

        afficher_entite(rend, fond_tour);
        afficher_entite(rend, fond_tour_2);
        afficher_entite(rend, perso);

        en_tete(I_LISTE_ENTITES);
        while (!hors_liste(I_LISTE_ENTITES)) {
            t_entite * elem =  valeur_elt(I_LISTE_ENTITES);
            afficher_entite(rend, elem);
            suivant(I_LISTE_ENTITES);
        }

        SDL_RenderCopy(rend, tex_ombre, NULL, NULL);

        SDL_DestroyTexture(tex_fps);
        tex_fps = SDL_CreateTextureFromSurface(rend, surface_fps);
        SDL_RenderCopy(rend, tex_fps, NULL, &dst_fps);

        SDL_RenderPresent(rend);

        // Gestion des collisions
        float correction_defilement = 0;
        verif_collision(perso, &correction_defilement);

        // Déplacement et animation du personnage
        if (! perso->a_collision_b)
            changer_animation(perso, perso->sens_regard == GAUCHE ? CHUTE_G : CHUTE_D);
        else if (perso->animation_courante->id == CHUTE_G || perso->animation_courante->id == CHUTE_D)
            changer_animation(perso, REPOS);
        deplacer(perso, compteur_frames);
        animer(perso, compteur_frames);

        // évolution du comportement des pnj
        t_entite * pnjs[100]; // tableau temporaire pour stocker les pnjs trouvés car la vérification de collision interfère avec le parcours de la liste
        int n_pnjs = 0;
        en_tete(I_LISTE_ENTITES);
        while (!hors_liste(I_LISTE_ENTITES)) {
            t_entite * elem = valeur_elt(I_LISTE_ENTITES);
            if (elem->pnj)
                pnjs[n_pnjs++] = elem;
            suivant(I_LISTE_ENTITES);
        }
        for (int i = 0; i < n_pnjs; i++) {
            verif_collision(pnjs[i], NULL);
            pnjs[i]->pnj->comportement(pnjs[i]);
            deplacer(pnjs[i], compteur_frames);
            animer(pnjs[i], compteur_frames);
        }

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
        en_tete(I_LISTE_ENTITES);
        while (!hors_liste(I_LISTE_ENTITES)) {
            t_entite * elem = valeur_elt(I_LISTE_ENTITES);
            changer_pos_rel(elem, 0, -pas_defilement);
            suivant(I_LISTE_ENTITES);
        }
        if (!perso->a_collision_b) {
            changer_pos_rel(fond_tour, 0, -pas_defilement);
            changer_pos_rel(fond_tour_2, 0, -pas_defilement);
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
            en_queue(I_LISTE_ENTITES);
            while (!hors_liste(I_LISTE_ENTITES)) {
                t_entite * elem = valeur_elt(I_LISTE_ENTITES);
                if (elem->rect_dst->y + elem->rect_dst->h < 0) {
                    oter_elt(I_LISTE_ENTITES);
                    detruire_entite(&elem);
                }
                else
                    precedent(I_LISTE_ENTITES);
            }

            generer_morceau_niveau(-1);
            generer_murs();
        }

        compteur_frames++;

        chrono_fin = clock();
        microsec_par_frame = (chrono_fin - chrono_deb) * 1000000 / CLOCKS_PER_SEC;
        // printf("%i μs\n", microsec_par_frame);
        char txt_fps[30];
        if (compteur_frames % 10 == 0) {
            sprintf(txt_fps, "%.2f FPS", 1000000./microsec_par_frame);
            SDL_FreeSurface(surface_fps);
            surface_fps = TTF_RenderText_Solid(police, txt_fps, couleur_fps);
        }

        int attente = 1000 / FPS - microsec_par_frame/1000;

        SDL_Delay(attente > 0 ? attente : 0); // Contrôle du taux de rafraîchissement
    }

    // Libération des ressources
    en_queue(I_LISTE_ENTITES);
    while (!hors_liste(I_LISTE_ENTITES)) {
        t_entite * elem = valeur_elt(I_LISTE_ENTITES);
        detruire_entite(&elem);
        oter_elt(I_LISTE_ENTITES);
    }
    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&fond_tour_2);
    detruire_entite(&perso);

    return doit_quitter ;
}
