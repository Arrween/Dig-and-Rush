#include <string.h>

#include "entite_pnj.h"
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

void comportement_patrouille(t_entite * pnj) {
    if (pnj->pnj->est_mort)
        return;
    if (pnj->deplacement == REPOS_MVT
        || (pnj->deplacement == DROITE && pnj->hitbox.x + pnj->hitbox.w >= pnj->pnj->x_patrouille_d)
        || pnj->collisions.d
        ) {
        pnj->deplacement = GAUCHE;
        changer_animation(pnj, DEPL_G);
    }
    else if ((pnj->deplacement == GAUCHE && pnj->hitbox.x <= pnj->pnj->x_patrouille_g)
             || pnj->collisions.g
             ) {
        pnj->deplacement = DROITE;
        changer_animation(pnj, DEPL_D);
    }
    if (!pnj->collisions.b) {
        if (pnj->deplacement == GAUCHE) {
            pnj->deplacement = DROITE;
            changer_animation(pnj, DEPL_D);
            changer_pos_rel(pnj, 1, 0);
        }
        else if (pnj->deplacement == DROITE) {
            pnj->deplacement = GAUCHE;
            changer_animation(pnj, DEPL_G);
            changer_pos_rel(pnj, -1, 0);
        }
    }
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
    if (strcmp(id, "squelette") == 0) {
        nouv->comportement = comportement_patrouille;
        nouv->valeur_vaincu = 10;
        nouv->est_ecrasable = VRAI;
        e->vitesse = 1./2;
        changer_hitbox(e, 20, 50, 50, 55);
    }
    //else if (strcmp(id, "feu") == 0) {
        // champs initiaux pour la boule de feu
    //}
    else if (strcmp(id, "feu") == 0) {
        nouv->comportement = comportement_patrouille;
        nouv->valeur_vaincu = 10;
        nouv->est_ecrasable = VRAI;
        e->vitesse = 1./2;
        changer_hitbox(e, 20, 50, 50, 55);
    }
    else {
        nouv->comportement = (void(*)(t_entite*)) comportement_oisif;
        nouv->valeur_vaincu = 0;
        nouv->est_ecrasable = FAUX;
    }
    nouv->x_patrouille_g = 0;
    nouv->x_patrouille_d = 100;
    nouv->est_mort = FAUX;
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
