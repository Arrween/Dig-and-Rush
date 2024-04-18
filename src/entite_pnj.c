#include <string.h>

#include "entite_pnj.h"
#include "entite_perso.h"
#include "entite.h"
#include "constantes.h"


/**
 * @brief Comportement par défaut pour un PNJ oisif.
 */

void comportement_oisif(void) {
    return;
}

/**
 * @brief Comportement par défaut pour un PNJ en patrouille.
 * 
 * @param pnj Pointeur vers l'entité du PNJ.
 */
void comportement_patrouille(t_entite * pnj, t_entite * perso) {
    if (pnj->pnj->est_mort)
        return;
    int a_change_sens = FAUX;

    if (!perso->perso->est_mort && SDL_HasIntersectionF(&pnj->pnj->hitbox_attaque, &perso->hitbox)) {
        if (pnj->deplacement == GAUCHE) {
            pnj->id_animation_suivante = DEPL_G;
            changer_animation(pnj, ATTQ_G);
            jouer_audio(3, pnj->pnj->id_son_attaque, 0);
        }
        else if (pnj->deplacement == DROITE) {
            pnj->id_animation_suivante = DEPL_D;
            changer_animation(pnj, ATTQ_D);
            jouer_audio(3, pnj->pnj->id_son_attaque, 0);
        }
        pnj->deplacement = REPOS_MVT;
        perso_prendre_coup(perso);
    }
    if (pnj->animation_courante->id == ATTQ_G || pnj->animation_courante->id == ATTQ_D)
        return;

    if (pnj->deplacement == REPOS_MVT
        || (pnj->deplacement == DROITE && pnj->hitbox.x + pnj->hitbox.w >= pnj->pnj->x_patrouille_d)
        || pnj->collisions.d
        ) {
        pnj->deplacement = GAUCHE;
        changer_animation(pnj, DEPL_G);
        changer_pos_rel(pnj, -1, 0); // pour sortir de l’état de collision
        if (pnj->deplacement != REPOS_MVT)
            a_change_sens = VRAI;
    }
    else if ((pnj->deplacement == GAUCHE && pnj->hitbox.x <= pnj->pnj->x_patrouille_g)
             || pnj->collisions.g
             ) {
        pnj->deplacement = DROITE;
        changer_animation(pnj, DEPL_D);
        changer_pos_rel(pnj, 1, 0); // pour sortir de l’état de collision
        a_change_sens = VRAI;
    }
    if (!pnj->collisions.b) {
        if (pnj->deplacement == GAUCHE) {
            pnj->deplacement = DROITE;
            changer_animation(pnj, DEPL_D);
            changer_pos_rel(pnj, 1, 0); // pour sortir de l’état de non-collision
            a_change_sens = VRAI;
        }
        else if (pnj->deplacement == DROITE) {
            pnj->deplacement = GAUCHE;
            changer_animation(pnj, DEPL_G);
            changer_pos_rel(pnj, -1, 0); // pour sortir de l’état de non-collision
            a_change_sens = VRAI;
        }
    }
    if (a_change_sens)
        appliquer_reflexion_hitbox(pnj, &(pnj->pnj->hitbox_attaque));
}



/**
 * @brief Comportement par défaut pour un PNJ volant en patrouille.
 * 
 * @param pnj Pointeur vers l'entité du PNJ.
 */
void comportement_patrouille_vol(t_entite * pnj, t_entite * perso) {
    if (pnj->pnj->est_mort)
        return;
    int a_change_sens = FAUX;

    if (!perso->perso->est_mort && SDL_HasIntersectionF(&pnj->pnj->hitbox_attaque, &perso->hitbox)) {
        if (pnj->deplacement == GAUCHE) {
            pnj->id_animation_suivante = DEPL_G;
            changer_animation(pnj, ATTQ_G);
            jouer_audio(3, pnj->pnj->id_son_attaque, 0);
        }
        else if (pnj->deplacement == DROITE) {
            pnj->id_animation_suivante = DEPL_D;
            changer_animation(pnj, ATTQ_D);
            jouer_audio(3, pnj->pnj->id_son_attaque, 0);
        }
        pnj->deplacement = REPOS_MVT;
        perso_prendre_coup(perso);
    }

    if (pnj->animation_courante->id == ATTQ_G || pnj->animation_courante->id == ATTQ_D)
        return;

    if (pnj->deplacement == REPOS_MVT
        || (pnj->deplacement == DROITE && pnj->hitbox.x + pnj->hitbox.w >= pnj->pnj->x_patrouille_d)
        || pnj->collisions.d
        ) {
        pnj->deplacement = GAUCHE;
        changer_animation(pnj, DEPL_G);
        changer_pos_rel(pnj, -1, 0); // pour sortir de l’état de collision
        if (pnj->deplacement != REPOS_MVT)
            a_change_sens = VRAI;
    }
    else if ((pnj->deplacement == GAUCHE && pnj->hitbox.x <= pnj->pnj->x_patrouille_g)
             || pnj->collisions.g
             ) {
        pnj->deplacement = DROITE;
        changer_animation(pnj, DEPL_D);
        changer_pos_rel(pnj, 1, 0); // pour sortir de l’état de collision
        a_change_sens = VRAI;
    }
    if (a_change_sens)
        appliquer_reflexion_hitbox(pnj, &(pnj->pnj->hitbox_attaque));
}




void comportement_patrouille_statique(t_entite *pnj, t_entite *perso) {
    if (pnj->pnj->est_mort)
        return;

    if (!perso->perso->est_mort && SDL_HasIntersectionF(&pnj->pnj->hitbox_attaque, &perso->hitbox)) {
        if (pnj->deplacement == REPOS_MVT) {
            // Choisir l'animation d'attaque en fonction de la direction du PNJ
            if (perso->hitbox.x > pnj->hitbox.x) {
                changer_animation(pnj, ATTQ_D);
            } else {
                changer_animation(pnj, ATTQ_G);
            }
            jouer_audio(3, pnj->pnj->id_son_attaque, 0);
        }
        pnj->deplacement = REPOS_MVT;
        perso_prendre_coup(perso);
    } else {
        // Si le PNJ ne rencontre pas la hitbox du personnage, il revient en position de repos
        changer_animation(pnj, REPOS);
    }
}











void pnj_mourir(t_entite * pnj, int * score, t_texte * texte_score) {
    if (!pnj->pnj || (pnj->pnj && pnj->pnj->est_mort))
        return;
    pnj->id_animation_suivante = ANIM_MORT_STATIQUE;
    changer_animation(pnj, ANIM_MORT);
    pnj->deplacement = REPOS_MVT;
    pnj->pnj->est_mort = VRAI;
    *score += pnj->pnj->valeur_vaincu;
    changer_texte(texte_score, "SCORE : %i", *score);
    jouer_audio(4, pnj->pnj->id_son_mort, 0);
}

/**
 * @brief Crée une nouvelle structure de PNJ avec les attributs spécifiés.
 * 
 * @param id Identifiant de la ressource utilisée pour définir les valeurs par défaut.
 * @param e entité contenant cette structure t_pnj
 * @return Pointeur vers la nouvelle structure de PNJ créée, NULL en cas d'échec.
 */

t_pnj * creer_pnj(char * id, t_entite * e) {
    t_pnj * nouv = malloc(sizeof(t_pnj));
    nouv->parent = e;
    nouv->doit_afficher_hitbox_attaque = FAUX;
    nouv->parent->doit_afficher_hitbox = FAUX;
    nouv->x_patrouille_g = 0;
    nouv->x_patrouille_d = 100;
    nouv->est_mort = FAUX;
    nouv->comportement = (void(*)(t_entite*, t_entite*)) comportement_oisif;
    nouv->valeur_vaincu = 0;
    nouv->est_ecrasable = FAUX;
    nouv->parent->vitesse = 1.;
    changer_hitbox(nouv->parent, &(nouv->hitbox_attaque), nouv->parent->hitbox.x,
                   nouv->parent->hitbox.y, nouv->parent->hitbox.w,
                   nouv->parent->hitbox.h, FAUX);
    strcpy(nouv->id_son_attaque, "silence");
    strcpy(nouv->id_son_mort, "silence");

    if (strcmp(id, "squelette") == 0) {
        nouv->comportement = comportement_patrouille;
        nouv->valeur_vaincu = 5;
        nouv->est_ecrasable = VRAI;
        nouv->parent->vitesse = 1./2;
        strcpy(nouv->id_son_attaque, "attaque_squelette");
        changer_hitbox(nouv->parent, &(nouv->parent->hitbox), 30, 50, 40, 55, VRAI);
        // définir initialement sur la droite, sera modifié par la patrouille
        changer_hitbox(nouv->parent, &(nouv->hitbox_attaque), 50, 70, 33, 20, VRAI);
    }
    else if (strcmp(id, "feu") == 0) {
        nouv->comportement = comportement_patrouille_vol;
        nouv->valeur_vaincu = 10;
        nouv->est_ecrasable = FAUX;
        nouv->parent->vitesse = 1./2;
        strcpy(nouv->id_son_attaque, "attaque_feu");
        strcpy(nouv->id_son_mort, "mort_feu");
        changer_hitbox(nouv->parent, &(nouv->parent->hitbox), 20, 30, 50, 55, VRAI);
        changer_hitbox(nouv->parent, &(nouv->hitbox_attaque), nouv->parent->hitbox.x,
                       nouv->parent->hitbox.y, nouv->parent->hitbox.w,
                       nouv->parent->hitbox.h, FAUX);
    }
    if (strcmp(id, "oncle") == 0) {
        nouv->comportement = comportement_patrouille_statique;
        nouv->valeur_vaincu = 20;
        nouv->est_ecrasable = VRAI;
        nouv->parent->vitesse = 1./2;
        strcpy(nouv->id_son_attaque, "attaque_perso");
        strcpy(nouv->id_son_mort, "mort_perso");
        changer_hitbox(nouv->parent, &(nouv->parent->hitbox), 15, 30, 80, 70, VRAI);
        changer_hitbox(nouv->parent, &(nouv->hitbox_attaque), nouv->parent->hitbox.x,
                       nouv->parent->hitbox.y, nouv->parent->hitbox.w,
                       nouv->parent->hitbox.h, FAUX);
    }


    return nouv;
}


/**
 * @brief Crée une nouvelle structure d'entité PNJ avec les attributs spécifiés.
 * 
 * @param id Identifiant de l'entité PNJ.
 * @param x Position en x de l'entité PNJ.
 * @param y Position en y de l'entité PNJ.
 * @param w Largeur de l'entité PNJ.
 * @param h Hauteur de l'entité PNJ.
 * @param est_relatif 1 si les coordonnées sont relatives, 0 si elles sont absolues.
 * @return Pointeur vers la nouvelle structure d'entité PNJ créée, NULL en cas d'échec.
 */

t_entite * creer_entite_pnj(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite(id, x, y, w, h, est_relatif);
    nouv->pnj = creer_pnj(id, nouv);
    return nouv;
}

/**
 * @brief Crée une nouvelle structure d'entité PNJ à partir d'une spritesheet avec les attributs spécifiés.
 * 
 * @param id Identifiant de l'entité PNJ.
 * @param x Position en x de l'entité PNJ.
 * @param y Position en y de l'entité PNJ.
 * @param w Largeur de l'entité PNJ.
 * @param h Hauteur de l'entité PNJ.
 * @param est_relatif 1 si les coordonnées sont relatives, 0 si elles sont absolues.
 * @return Pointeur vers la nouvelle structure d'entité PNJ créée, NULL en cas d'échec.
 */

t_entite * creer_entite_pnj_depuis_spritesheet(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite_depuis_spritesheet(id, x, y, w, h, est_relatif);
    nouv->pnj = creer_pnj(id, nouv);
    return nouv;
}

/**
 * @brief Détruit une structure de PNJ et libère la mémoire associée.
 * 
 * @param p Pointeur vers le pointeur de la structure de PNJ à détruire.
 */

void detruire_pnj(t_pnj ** p) {
    if (*p) {
        free(*p);
    }
    *p = NULL;
}
