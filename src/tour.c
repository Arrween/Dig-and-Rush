/**
 * @file tour.c
 * @brief Implémentation des fonctions liées à la gestion de la tour.
 */

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "ressources.h"
#include "tour.h"
#include "constantes.h"
#include "entite.h"
#include "entite_destructible.h"
#include "entite_bonus.h"
#include "entite_pnj.h"
#include "entite_perso.h"
#include "nuit.h"
#include "morceaux_niveau.h"
#include "listes.h"
#include "texte.h"


#define Y_PREMIERE_LIGNE 80
#define DELAI_CREUSAGE 18 // en nombre de frames
#define DELAI_PERTE_SCORE 150 // en nombre de frames
#define DELAI_JOUR 750 // en nombre de frames
#define DELAI_NUIT 300 // en nombre de frames


/**
 * @brief Fonction pour vérifier si un point est dans un rectangle à virgule flottante.
 * 
 * @param p Le point à vérifier.
 * @param r Le rectangle dans lequel vérifier.
 * @return SDL_TRUE si le point est dans le rectangle, sinon SDL_FALSE.
 */

SDL_bool PointInFRect(const SDL_FPoint* p, const SDL_FRect* r) {
    if (p->x >= r->x && p->x < r->x + r->w &&
        p->y >= r->y && p->y < r->y + r->h) {
        return SDL_TRUE;
    } else {
        return SDL_FALSE;
    }
}

/**
 * @brief vérifie les collisions d’une entité avec les obstacles
 * @param e1 entité possiblement en collision
 * @param correction_defilement valeur à retourner si `e1` est le personnage joueur
 */
void verif_collision(t_entite * e1, float * correction_defilement, int * score, t_texte * texte_score) {
    // points haut gauche, haut droit etc., décalés sur l’axe vertical pour ne pas déclencher
    // une collision à la fois sur la gauche, la droite et le bas
    SDL_FPoint e1_hg = {e1->hitbox.x, e1->hitbox.y + 0.05 * e1->hitbox.h};
    SDL_FPoint e1_hd = {e1->hitbox.x + e1->hitbox.w, e1->hitbox.y + 0.05 * e1->hitbox.h};
    SDL_FPoint e1_bg = {e1->hitbox.x, e1->hitbox.y + 0.8 * e1->hitbox.h};
    SDL_FPoint e1_bd = {e1->hitbox.x + e1->hitbox.w, e1->hitbox.y + 0.8 * e1->hitbox.h};
    // point au milieu du bas de la hitbox
    SDL_FPoint e1_b = {e1->hitbox.x + 0.5 * e1->hitbox.w, e1->hitbox.y + e1->hitbox.h};

    // if (strcmp(e1->type, "matt") == 0) return;

    e1->collisions.g = NULL;
    e1->collisions.d = NULL;
    e1->collisions.h = NULL;
    e1->collisions.b = NULL;

    liste_en_tete(I_LISTE_ENTITES);
    while (!hors_liste(I_LISTE_ENTITES)) {
        t_entite * e2 = liste_lire(I_LISTE_ENTITES);
        
        if (e1 == e2
            || (!e2->est_obstacle && !e2->bonus && !(e2->pnj && e2->pnj->est_ecrasable)) 
            || (e2->pnj && e2->pnj->est_mort)
            || (e1->pnj && e2->pnj)
            || (e1->pnj && e2->bonus)
            || (e2->perso && e2->perso->est_mort)
            || (e2->pnj && e1->perso && e1->perso->temps_invu > 0)
            || (e2->pnj && e1->perso && e1->perso->est_mort)) {
            liste_suivant(I_LISTE_ENTITES);
            continue;
        }

        // il y a collision à gauche, droite ou en haut si les deux points correspondants sont
        // dans la hitbox de l’entité candidate
        int collision_g = PointInFRect(&e1_hg, &(e2->hitbox)) || PointInFRect(&e1_bg, &(e2->hitbox));
        int collision_d = PointInFRect(&e1_hd, &(e2->hitbox)) || PointInFRect(&e1_bd, &(e2->hitbox));
        int collision_h = PointInFRect(&e1_hg, &(e2->hitbox)) || PointInFRect(&e1_hg, &(e2->hitbox));
        // il y a collision en bas si le point au milieu du bas de la hitbox et dans la hitbox candidate
        int collision_b = PointInFRect(&e1_b, &(e2->hitbox));
        // l’entité a une collision à gauche si elle est en collision à gauche avec au moins une entité de la liste
        // idem pour droite, haut, bas
        if (collision_g)
            e1->collisions.g = e2;
        if (collision_d)
            e1->collisions.d = e2;
        if (collision_h)
            e1->collisions.h = e2;
        if (collision_b)
            e1->collisions.b = e2;

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
            if (depassement < 0)
                *correction_defilement = depassement;
        }

        // ramassage de bonus
        if ((e2->bonus && e1->perso) && (collision_b || collision_d || collision_g || collision_h)) {
            jouer_audio(CANAL_BONUS, e2->bonus->id_son, 0);
            *score += e2->bonus->valeur;
            changer_texte(texte_score, "SCORE : %i", *score);
            e1->perso->vie += e2->bonus->soin;
            if (e1->perso->vie > e1 ->perso->vie_max)
                e1->perso->vie = e1->perso->vie_max;
            detruire_entite(&e2);
            liste_retirer(I_LISTE_ENTITES);
        }

        liste_suivant(I_LISTE_ENTITES);
    }
}



/**
 * @brief Fonction pour gérer le creusage.
 * 
 * @param a_creuser Entité à creuser.
 */
void creuser(t_entite * a_creuser) {
    liste_en_queue(I_LISTE_ENTITES);
    while(!hors_liste(I_LISTE_ENTITES)) {
        t_entite * elem = liste_lire(I_LISTE_ENTITES);
        if (a_creuser == elem && elem->destructible) {
            jouer_audio(CANAL_CREUSAGE, elem->destructible->id_son, 0);
            liste_retirer(I_LISTE_ENTITES);
            detruire_entite(&elem);
            break;
        }
        else
            liste_precedent(I_LISTE_ENTITES);
    }
}


/**
 * @brief Lit le score depuis un fichier et le met à jour si nécessaire.
 * 
 * @param nouveau_score Le nouveau score à comparer.
 * @return 1 si un nouveau record est enregistré, -1 si le record n'est pas battu, 0 si premier score enregistré.
 */
int lire_score(int nouveau_score) {
    int ancien_score;
    FILE *fichier;

    // Ouvrir le fichier en mode lecture
    fichier = fopen("score.txt", "r");

    // Vérifier si le fichier existe
    if (fichier != NULL) {
        // Lire l'ancien score
        fscanf(fichier, "%d", &ancien_score);
        // Fermer le fichier
        fclose(fichier);

        // Comparer les scores
        if (nouveau_score > ancien_score) {
            // Ouvrir le fichier en mode écriture pour écraser l'ancien score
            fichier = fopen("score.txt", "w");
            // Écrire le nouveau score dans le fichier
            fprintf(fichier, "%d", nouveau_score);
            // Fermer le fichier
            fclose(fichier);
            return 1; // Nouveau score enregistré
        } 
        else {
            return -1; // Record non battu
        }
    } else {
        // Créer un nouveau fichier score.txt et y écrire le nouveau score
        fichier = fopen("score.txt", "w");
        fprintf(fichier, "%d", nouveau_score);
        fclose(fichier);
        return 0; //Premier score enregistré
    }

    return 0;
}


int boucle_jeu(SDL_Renderer * rend, char * nom_perso) {
    SDL_Event event;
    int doit_boucler = VRAI;
    int est_en_pause = FAUX;
    long long compteur_frames = 0;
    float pas_defilement = 0;
    float repere_defilement = 100;
    int score = 0;
    int creusage_en_cours = FAUX; // Indicateur si l'animation de creusage est en cours
    int compteur_creusage = 0;
    int compteur_jour_nuit = 0;
    int record ;
    int score_enregistre = 0;

    srand(time(NULL));
    int n = rand()%10 + 1;

    init_liste(I_LISTE_ENTITES);

    Mix_HaltMusic();
    Mix_Volume(CANAL_MUS_JOUR, 0);
    jouer_audio(CANAL_MUS_JOUR, "musique_jour", -1);
    jouer_audio(CANAL_MUS_NUIT, "musique_nuit", -1);

    int doit_quitter = FAUX;

    // Initialisation des entités de fond et de personnage
    t_entite * fond ;
    if (n%2)
        fond = creer_entite("fond_jeu", -1, -1, -1, -1, FAUX);
    else
        fond = creer_entite("fond_jeu2", -1, -1, -1, -1, FAUX);

    t_entite * fond_nuit = creer_entite("fond_jeu_nuit", -1, -1, -1, -1, FAUX);
    t_entite * fond_tour = creer_entite("fond_tour", 0, 0, 100, 100, VRAI);
    t_entite * fond_tour_2 = creer_entite("fond_tour", 0, 100, 100, 100, VRAI);
    t_entite * perso = creer_entite_perso(nom_perso, 40, 20, 15, 12, VRAI);

    jouer_audio(CANAL_COQ, "coq", 0);

    generer_murs();

    generer_premiere_ligne(Y_PREMIERE_LIGNE);

    SDL_FRect zone_jeu = {TAILLE_L/4., 0, TAILLE_L/2., TAILLE_H};
    t_nuit * nuit = creer_nuit(rend, perso, zone_jeu, fond->texture, fond_nuit->texture);

    // compteur de FPS
    t_texte * texte_fps = creer_texte("police_defaut", 255, 255, 255, 255, 20, TAILLE_H-40, 100, 30);

    t_texte * texte_score = creer_texte("police_defaut", 255, 255, 255, 255, 20, 20, 160, 50);
    changer_texte(texte_score, "SCORE : %i", score);

    t_texte * texte_record = creer_texte("police_defaut", 255, 255, 255, 255, 450, 550, 360, 50);

    t_texte * texte_vie = creer_texte("police_defaut", 255, 255, 255, 255, 20, 80, 160, 50);
    changer_texte(texte_vie, "HEALTH : %i", perso->perso->vie);

    t_texte * texte_mort = creer_texte("police_defaut", 255, 255, 255, 255, 550, 500, 160, 50);
    changer_texte(texte_mort, "GAME OVER !");

    t_texte * texte_reessayer = creer_texte("police_defaut", 255, 255, 255, 255, 450, 600, 350, 50);
    changer_texte(texte_reessayer, "Press <escape> to retry");

    t_texte * texte_pause = creer_texte("police_defaut", 255, 255, 255, 255, 450, 550, 350, 50);
    changer_texte(texte_pause, "Paused, press <space> to resume.");

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

                    // arrêter le creusage si l’on appuie sur une touche
                    // différente de celle de creusage même si l’on a maintenu
                    // cette dernière enfoncée
                    if (event.key.keysym.scancode != SDL_SCANCODE_S)
                        creusage_en_cours = FAUX;

                    if (perso->perso->est_mort) {
                        
                        switch (event.key.keysym.scancode) {
                            case SDL_SCANCODE_ESCAPE:
                            case SDL_SCANCODE_Q:
                                doit_boucler = FAUX;
                                break;
                            default:
                                break;
                        }
                        continue;
                    }

                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            doit_boucler = FAUX;
                            break;
                        case SDL_SCANCODE_SPACE:
                            est_en_pause = !est_en_pause;
                            if (!est_en_pause){
                                if (nuit->est_active) {
                                    nuit->est_active = FAUX ;
                                }
                            }
                            break;
                        case SDL_SCANCODE_H:
                            perso->doit_afficher_hitbox = !perso->doit_afficher_hitbox;
                            perso->perso->doit_afficher_hitbox_attaque = !perso->perso->doit_afficher_hitbox_attaque;
                            liste_en_tete(I_LISTE_ENTITES);
                            while (!hors_liste(I_LISTE_ENTITES)) {
                                t_entite * elem = liste_lire(I_LISTE_ENTITES);
                                if (elem->pnj) {
                                    elem->doit_afficher_hitbox = perso->doit_afficher_hitbox;
                                    elem->pnj->doit_afficher_hitbox_attaque = perso->perso->doit_afficher_hitbox_attaque;
                                }
                                liste_suivant(I_LISTE_ENTITES);
                            }
                            break;
                        case SDL_SCANCODE_L:
                            basculer_nuit(nuit);
                            break;
                        case SDL_SCANCODE_A:
                            perso->deplacement_prec = perso->deplacement;
                            perso->deplacement = GAUCHE;
                            if (perso->collisions.b)
                                changer_animation(perso, DEPL_G);
                            break;
                        case SDL_SCANCODE_D:
                            perso->deplacement_prec = perso->deplacement;
                            perso->deplacement = DROITE;
                            if (perso->collisions.b)
                                changer_animation(perso, DEPL_D);
                            break;
                        case SDL_SCANCODE_S:
                            if (perso->collisions.b){
                                changer_animation(perso, CREUSER); 
                                perso->deplacement_prec = perso->deplacement;
                                perso->deplacement = REPOS_MVT;
                                compteur_creusage = 0; // Réinitialiser le compteur à chaque fois que la touche est enfoncée
                                creusage_en_cours = VRAI; // Marquer que l'animation de creusage est en cours
                            }
                            break;
                        case SDL_SCANCODE_W:
                            if (perso->collisions.b) {
                                perso->deplacement_prec = perso->deplacement;
                                perso->deplacement = REPOS_MVT;
                                if (perso->sens_regard == GAUCHE) {
                                    changer_animation(perso, ATTQ_G);
                                    creuser(perso->collisions.g);
                                }
                                else if (perso->sens_regard == DROITE) {
                                    changer_animation(perso, ATTQ_D);
                                    creuser(perso->collisions.d);
                                }
                                perso_porter_coup(perso, &score, texte_score);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_A:
                            if (perso->animation_courante->id == DEPL_G) {
                                if (perso->collisions.b)
                                    changer_animation(perso, REPOS);
                            }
                            if (perso->deplacement == perso->deplacement_prec)
                                perso->deplacement = REPOS_MVT;
                            else if (perso->deplacement == GAUCHE)
                                perso->deplacement = perso->deplacement_prec;
                            if (perso->deplacement_prec == GAUCHE)
                                perso->deplacement_prec = REPOS_MVT;
                            break;
                        case SDL_SCANCODE_D:
                            if (perso->animation_courante->id == DEPL_D) {
                                if (perso->collisions.b)
                                    changer_animation(perso, REPOS);
                            }
                            if (perso->deplacement == perso->deplacement_prec)
                                perso->deplacement = REPOS_MVT;
                            else if (perso->deplacement == DROITE)
                                perso->deplacement = perso->deplacement_prec;
                            if (perso->deplacement_prec == DROITE)
                                perso->deplacement_prec = REPOS_MVT;
                            break;
                        case SDL_SCANCODE_S:
                            if (perso->animation_courante->id == CREUSER) {
                                perso->deplacement = perso->deplacement_prec;
                                changer_animation(perso, REPOS);
                            }
                            creusage_en_cours = FAUX;
                            break;
                        case SDL_SCANCODE_W:
                            if (perso->animation_courante->id ==  ATTQ_G || perso->animation_courante->id == ATTQ_D) {
                                perso->deplacement = perso->deplacement_prec;
                                changer_animation(perso, REPOS);
                            }
                            break;
                        default:
                            break;
                    }
            }
           
        }
        if (est_en_pause) {
            nuit->est_active = VRAI ;
            SDL_Delay(1000/FPS);
        }

        

        if (creusage_en_cours) {
            compteur_creusage++;
            if (compteur_creusage >= DELAI_CREUSAGE) {
                creuser(perso->collisions.b);
                creusage_en_cours = FAUX;
            } 
        }
        
        SDL_RenderClear(rend);

        if (perso->perso->est_mort){
            nuit->est_active = VRAI;
        }

        transitionner_nuit(nuit);

        // Affichage des entités
        afficher_entite(rend, fond);
        afficher_entite(rend, fond_nuit);

        afficher_entite(rend, fond_tour);
        afficher_entite(rend, fond_tour_2);

        if (perso->perso->temps_invu > 0) {
            perso->perso->temps_invu--;
            // faire clignoter le perso une fois toutes les 4 frames
            if (perso->perso->temps_invu % 4 > 0)
                afficher_entite(rend, perso);
        }
        else
            afficher_entite(rend, perso);

        liste_en_tete(I_LISTE_ENTITES);
        while (!hors_liste(I_LISTE_ENTITES)) {
            t_entite * elem =  liste_lire(I_LISTE_ENTITES);
            afficher_entite(rend, elem);
            liste_suivant(I_LISTE_ENTITES);
        }

        if (nuit->est_active || nuit->est_active_prec)
            SDL_RenderCopy(rend, nuit->texture_ombre, NULL, NULL);

        afficher_texte(rend, texte_fps);
        afficher_texte(rend, texte_score);
        changer_texte(texte_vie, "HEALTH : %i", perso->perso->vie);
        afficher_texte(rend, texte_vie);
        if (est_en_pause) 
            afficher_texte(rend, texte_pause);

        if (perso->perso->est_mort) {
            afficher_texte(rend, texte_mort);
            afficher_texte(rend, texte_reessayer);
            record = lire_score(score);
            if (!score_enregistre){
                if (record > 0){
                    changer_texte(texte_record, "New record, good game ! %d", score);
                }
                else if (record < 0){
                    changer_texte(texte_record, "You failed to beat the record");
                }
                else 
                    changer_texte(texte_record, "Your first score is saved");
                score_enregistre = 1;
            }

            afficher_texte(rend, texte_record);
            

        }

        
        SDL_RenderPresent(rend);
        if (est_en_pause) {
            continue ;
        }

        // Gestion des collisions
        float correction_defilement = 0;
        verif_collision(perso, &correction_defilement, &score, texte_score);

        // Déplacement et animation du personnage
        if (!perso->collisions.b) {
            changer_animation(perso, perso->sens_regard == GAUCHE ? CHUTE_G : CHUTE_D);
            if (perso->deplacement != REPOS_MVT)
                perso->deplacement_prec = perso->deplacement;
            perso->deplacement = REPOS_MVT;
        }
        else if (perso->animation_courante->id == CHUTE_G || perso->animation_courante->id == CHUTE_D) {
            changer_animation(perso, REPOS);
            perso->deplacement = perso->deplacement_prec;
        }
        else if (perso->animation_courante->id == REPOS) {
            if (perso->deplacement == GAUCHE)
                changer_animation(perso, DEPL_G);
            else if (perso->deplacement == DROITE)
                changer_animation(perso, DEPL_D);
        }
        deplacer(perso, compteur_frames);
        animer(perso, compteur_frames);

        // évolution du comportement des pnj
        t_entite * pnjs[100]; // tableau temporaire pour stocker les pnjs trouvés car la vérification de collision interfère avec le parcours de la liste
        int n_pnjs = 0;
        liste_en_tete(I_LISTE_ENTITES);
        while (!hors_liste(I_LISTE_ENTITES)) {
            t_entite * elem = liste_lire(I_LISTE_ENTITES);
            if (elem->pnj)
                pnjs[n_pnjs++] = elem;
            liste_suivant(I_LISTE_ENTITES);
        }
        for (int i = 0; i < n_pnjs; i++) {
            // tue un ennemi qui peut être écrasé si le personnage lui tombe dessus
            if (pnjs[i]->pnj->est_ecrasable && perso->collisions.b == pnjs[i]) {
                pnj_mourir(pnjs[i], &score, texte_score);
                pnjs[i]->pnj->est_ecrasable = FAUX;
                continue;
            }
            // évolution du comportement du pnj i
            verif_collision(pnjs[i], NULL, NULL, NULL);
            pnjs[i]->pnj->comportement(pnjs[i], perso);
            deplacer(pnjs[i], compteur_frames);
            animer(pnjs[i], compteur_frames);
        }

        // Logique de défilement des obstacles
        if (!perso->collisions.b) {
            // Calcul du pas de défilement en fonction de la vitesse de chute et du compteur de frames
            if (VITESSE_CHUTE >= 1) {
                pas_defilement = VITESSE_CHUTE;
            } else if (compteur_frames % (int)(1 / (VITESSE_CHUTE)) == 0) { // avertissement de division par zéro à ignorer du fait que dans cette branche VITESSE_CHUTE est inférieur à 1
                pas_defilement = 1;
            } else {
                pas_defilement = 0;
            }
        }
        else {
            pas_defilement = correction_defilement;
        }
        repere_defilement += pas_defilement;

        // Déplacement relatif des obstacles pour simuler le défilement
        liste_en_tete(I_LISTE_ENTITES);
        while (!hors_liste(I_LISTE_ENTITES)) {
            t_entite * elem = liste_lire(I_LISTE_ENTITES);
            changer_pos_rel(elem, 0, -pas_defilement);
            liste_suivant(I_LISTE_ENTITES);
        }
        changer_pos_rel(fond_tour, 0, -pas_defilement);
        changer_pos_rel(fond_tour_2, 0, -pas_defilement);

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
            liste_en_queue(I_LISTE_ENTITES);
            while (!hors_liste(I_LISTE_ENTITES)) {
                t_entite * elem = liste_lire(I_LISTE_ENTITES);
                if (elem->rect_dst->y + elem->rect_dst->h < 0) {
                    liste_retirer(I_LISTE_ENTITES);
                    detruire_entite(&elem);
                }
                else
                    liste_precedent(I_LISTE_ENTITES);
            }

            generer_murs();
        }
        if (repere_defilement > 100) {
            generer_morceau_niveau(repere_defilement);
            repere_defilement = 0;
        }

        compteur_frames++;

        if (compteur_frames % DELAI_PERTE_SCORE == 0 && score > 0 && !perso->perso->est_mort) {
            score--;
            changer_texte(texte_score, "SCORE : %i", score);
        }

        compteur_jour_nuit++;

        if (nuit->est_active && compteur_jour_nuit == DELAI_NUIT) {
            basculer_nuit(nuit);
            compteur_jour_nuit = 0;
        }
        else if (!nuit->est_active && compteur_jour_nuit == DELAI_JOUR) {
            basculer_nuit(nuit);
            compteur_jour_nuit = 0;
        }

        chrono_fin = clock();
        microsec_par_frame = (chrono_fin - chrono_deb) * 1000000 / CLOCKS_PER_SEC;
        // printf("%i μs\n", microsec_par_frame);
        if (compteur_frames % 10 == 0)
            changer_texte(texte_fps, "%.2f FPS", 1000000./microsec_par_frame);

        int attente = 1000 / FPS - microsec_par_frame/1000;

        SDL_Delay(attente > 0 ? attente : 0); // Contrôle du taux de rafraîchissement
    }

    // Libération des ressources
    liste_en_queue(I_LISTE_ENTITES);
    while (!hors_liste(I_LISTE_ENTITES)) {
        t_entite * elem = liste_lire(I_LISTE_ENTITES);
        detruire_entite(&elem);
        liste_retirer(I_LISTE_ENTITES);
    }
    detruire_entite(&fond);
    detruire_entite(&fond_tour);
    detruire_entite(&fond_tour_2);
    detruire_entite(&perso);
    detruire_texte(&texte_fps);
    detruire_texte(&texte_score);
    detruire_texte(&texte_mort);
    detruire_texte(&texte_reessayer);
    detruire_nuit(&nuit);

    return doit_quitter ;
}
