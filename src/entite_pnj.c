#include <string.h>

#include "entite_pnj.h"
#include "entite.h"
#include "constantes.h"

void comportement_oisif(void) {
    return;
}

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
* @brief crée un nouveau `t_pnj` et définit les valeurs par défaut de
*       ses attributs en fonction du type de ressource
* @param id identifiant de ressource utilisé pour distinguer les valeurs par défaut à affecter
*/
t_pnj * creer_pnj(char * id) {
    t_pnj * nouv = malloc(sizeof(t_pnj));
    if (strcmp(id, "squelette") == 0) {
        nouv->comportement = comportement_patrouille;
        nouv->valeur_vaincu = 10;
        nouv->est_ecrasable = VRAI;
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

t_entite * creer_entite_pnj(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite(id, x, y, w, h, est_relatif);
    nouv->pnj = creer_pnj(id);
    return nouv;
}

t_entite * creer_entite_pnj_depuis_spritesheet(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite_depuis_spritesheet(id, x, y, w, h, est_relatif);
    nouv->pnj = creer_pnj(id);
    return nouv;
}

void detruire_pnj(t_pnj ** p) {
    if (*p) {
        free(*p);
    }
    *p = NULL;
}
