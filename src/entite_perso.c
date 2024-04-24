/**
 * @file entite_perso.c
 * @brief Définition des fonctions pour la gestion des entités de personnages joueurs.
 * 
 * Ce fichier contient les définitions des fonctions pour la création, la manipulation
 * et la destruction des entités de personnages joueurs. Ces fonctions permettent de
 * gérer les actions des personnages joueurs, telles que attaquer, prendre des coups
 * et mourir, ainsi que la création et la destruction de ces entités.
 */

#include <string.h>
#include "entite_perso.h"
#include "entite.h"
#include "entite_pnj.h"
#include "listes.h"
#include "texte.h"
#include "constantes.h"



/**
 * @brief Porte un coup avec le personnage joueur.
 * 
 * Cette fonction vérifie si le personnage joueur attaque un ennemi. Si c'est le cas,
 * l'ennemi meurt, et le score est mis à jour. Ensuite, un son d'attaque est joué.
 * 
 * @param e Pointeur vers l'entité du personnage joueur.
 * @param score Pointeur vers le score du joueur.
 * @param texte_score Pointeur vers la structure texte pour afficher le score.
 */

void perso_porter_coup(t_entite * e, int * score, t_texte * texte_score) {
    // Vérifie si l'entité passée en paramètre représente un personnage contrôlé par le joueur
    if (!e->perso)
        return;  // Si ce n'est pas le cas,ça s'arrête

    SDL_FRect * hitbox_attaque = &e->perso->hitbox_attaque;

    liste_en_tete(I_LISTE_ENTITES);
    while (!hors_liste(I_LISTE_ENTITES)) {
        t_entite * elem = liste_lire(I_LISTE_ENTITES);
        if (elem->pnj) {
            if (SDL_HasIntersectionF(hitbox_attaque, &elem->hitbox))
                pnj_mourir(elem, score, texte_score);
        }
        liste_suivant(I_LISTE_ENTITES);
    }
    jouer_audio(CANAL_ATTAQUE_PERSO, e->perso->id_son_attaque, 0);
}


/**
 * @brief Gère la mort du personnage joueur.
 * 
 * Cette fonction est appelée lorsque le personnage joueur meurt. Elle change
 * l'animation du personnage, le marque comme mort, et joue un son de mort.
 * 
 * @param perso Pointeur vers l'entité du personnage joueur.
 */
void perso_mourir(t_entite * perso) {
    perso->id_animation_suivante = ANIM_MORT_STATIQUE;
    changer_animation(perso, ANIM_MORT);
    perso->deplacement = REPOS_MVT;
    perso->perso->est_mort = VRAI;
    jouer_audio(CANAL_MORT_PERSO, perso->perso->id_son_mort, 0);
    
}

/**
 * @brief Gère le fait que le personnage joueur reçoit un coup.
 * 
 * Cette fonction est appelée lorsque le personnage joueur reçoit un coup.
 * Elle diminue les points de vie du personnage joueur, le marque comme mort
 * s'il n'a plus de points de vie, ou joue un son de blessure sinon.
 * 
 * @param perso Pointeur vers l'entité du personnage joueur.
 */
void perso_prendre_coup(t_entite * perso) {
     // Vérifie si le personnage est en période d'invincibilité
    if (perso->perso->temps_invu > 0)
        return;
    perso->perso->vie--;
    if (perso->perso->vie <= 0)
        perso_mourir(perso);
    else{
        // Met à jour le temps d'invincibilité du personnage avec la valeur maximale
        perso->perso->temps_invu = perso->perso->temps_invu_max;
        jouer_audio(CANAL_BLESSURE_PERSO, perso->perso->id_son_blessure, 0);
    }
}

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

    // Affecte les attributs du personnage en fonction de son identifiant
    if (strcmp(id, "ania") == 0) {
        nouv->parent->vitesse = 2.;
        nouv->vie_max = nouv->vie = 3;
        strcpy(nouv->id_son_blessure, "blessure_perso_f");
        strcpy(nouv->id_son_mort, "mort_perso_f");
    }
    else {
        if (strcmp(id, "jack") == 0) {
        nouv->parent->vitesse = 2.;
        nouv->vie_max = nouv->vie = 2;
        }
        else if (strcmp(id, "matt") == 0) {
            nouv->parent->vitesse = 1.;
            nouv->vie_max = nouv->vie = 3;
        }
        else if (strcmp(id, "yohan") == 0) {
            nouv->parent->vitesse = 2.;
            nouv->vie_max = nouv->vie = 3;
        }
        else {
            nouv->parent->vitesse = 1.;
            nouv->vie_max = nouv->vie = 1;
        }
        strcpy(nouv->id_son_blessure, "blessure_perso");
        strcpy(nouv->id_son_mort, "mort_perso");
    }
    
    // Configuration des hitbox du personnage
    changer_hitbox(nouv->parent, &(nouv->parent->hitbox), 26, 22, 51, 73.4, VRAI);
    // définir initialement sur la droite, sera modifié par le déplacement
    changer_hitbox(nouv->parent, &(nouv->hitbox_attaque), 50, 45, 55, 45, VRAI);

    // Configuration des autres attributs du personnage
    nouv->doit_afficher_hitbox_attaque = FAUX;
    nouv->parent->doit_afficher_hitbox = FAUX;
    nouv->est_mort = FAUX;
    nouv->temps_invu = 0;
    nouv->temps_invu_max = 60;
    strcpy(nouv->id_son_attaque, "attaque_perso");
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
