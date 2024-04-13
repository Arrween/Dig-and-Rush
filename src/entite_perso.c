#include <string.h>

#include "entite_perso.h"
#include "entite.h"
#include "constantes.h"


/**
 * @brief Crée une nouvelle structure de personnage joueur avec les attributs spécifiés.
 * 
 * @param id Identifiant de la ressource utilisée pour définir les valeurs par défaut.
 * @param e entité contenant cette structure t_perso
 * @return Pointeur vers la nouvelle structure de personnage joueur créée, NULL en cas d'échec.
 */

t_perso * creer_perso(char * id, t_entite * e) {
    t_perso * nouv = malloc(sizeof(t_perso));
    nouv->parent = e;
    if (strcmp(id, "matt") == 0) {
        nouv->parent->vitesse = 1.;
        changer_hitbox(nouv->parent, &(nouv->parent->hitbox), 26, 22, 51, 73.4, VRAI);
        // définir initialement sur la droite, sera modifié par le déplacement
        changer_hitbox(nouv->parent, &(nouv->hitbox_attaque), 50, 45, 60, 45, VRAI);
        nouv->doit_afficher_hitbox_attaque = VRAI;
        nouv->parent->doit_afficher_hitbox = VRAI;
    }
    else {
    }
    nouv->est_mort = FAUX;
    return nouv;
}


/**
 * @brief Crée une nouvelle structure d'entité personnage joueur avec les attributs spécifiés.
 * 
 * @param id Identifiant de l'entité personnage joueur.
 * @param x Position en x de l'entité personnage joueur.
 * @param y Position en y de l'entité personnage joueur.
 * @param w Largeur de l'entité personnage joueur.
 * @param h Hauteur de l'entité personnage joueur.
 * @param est_relatif 1 si les coordonnées sont relatives, 0 si elles sont absolues.
 * @return Pointeur vers la nouvelle structure d'entité personnage joueur créée, NULL en cas d'échec.
 */

t_entite * creer_entite_perso(char * id, float x, float y, float w, float h, int est_relatif) {
    t_entite * nouv = creer_entite_depuis_spritesheet(id, x, y, w, h, est_relatif);
    nouv->perso = creer_perso(id, nouv);
    return nouv;
}

/**
 * @brief Détruit une structure de personnage joueur et libère la mémoire associée.
 * 
 * @param p Pointeur vers le pointeur de la structure de personnage joueur à détruire.
 */

void detruire_perso(t_perso ** p) {
    if (*p) {
        free(*p);
    }
    *p = NULL;
}
