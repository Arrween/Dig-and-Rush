/**
 *  @file
 *  @brief Gestion des entités (tout objet devant être affiché)
 */

#include <string.h>

#include "entite.h"
#include "entite_destructible.h"
#include "entite_pnj.h"
#include "entite_perso.h"
#include "entite_bonus.h"
#include "constantes.h"

/** @brief Variable globale pour stocker les numéros des entités. */
int numeros_entites = 0;

/**
 * @brief Convertit des coordonnées relatives en coordonnées absolues.
 * @param rect Le rectangle avec les coordonnées relatives.
 * @param rect_abs Le rectangle avec les coordonnées absolues de base
 * @return SDL_Rect Le rectangle avec les coordonnées absolues calculées.
 */

SDL_FRect convertir_vers_absolu(SDL_FRect * rect, SDL_FRect rect_abs) {
    SDL_FRect retour;
    retour.x = rect_abs.x + rect_abs.w * rect->x/100;
    retour.y = rect_abs.y + rect_abs.h * rect->y/100;
    retour.w = rect_abs.w * rect->w/100;
    retour.h = rect_abs.h * rect->h/100;
    return retour;
}

/**
 * @brief Affiche une entité à l'écran en utilisant SDL.
 * 
 * Cette fonction prend en charge l'affichage d'entités (comme des personnages, objets, etc.)
 * sur le rendu spécifié en paramètre. Elle gère à la fois les entités avec position
 * relative (par rapport à une zone de jeu définie) et absolue.
 * 
 * @param rend Pointeur vers le rendu SDL où l'entité sera affichée.
 * @param e Pointeur vers l'entité à afficher.
 * 
 * Les étapes principales sont :
 * 1. Calcul des coordonnées et dimensions pour l'affichage de l'entité et sa hitbox,
 *    en tenant compte de la configuration de la zone de jeu.
 * 2. Si l'entité doit être affichée avec une position relative, conversion de sa position
 *    et dimension à une position absolue sur l'écran.
 * 3. Affichage de l'entité à l'aide de SDL_RenderCopy.
 * 4. Si l'entité doit afficher sa hitbox, celle-ci est dessinée autour de l'entité
 *    en utilisant une couleur spécifique.
*/

void afficher_entite(SDL_Renderer * rend, t_entite * e) {

    // Définition de la zone de jeu
    SDL_FRect zone_jeu = {TAILLE_L/4., 0, TAILLE_L/2., TAILLE_H};

    // Vérification des coordonnées source et destination
    SDL_Rect * src = e->rect_src->w >= 0 ? e->rect_src : NULL;
    SDL_FRect * dst = e->rect_dst->w >= 0 ? e->rect_dst : NULL;

    // Si les coordonnées sont relatives, convertir en coordonnées absolues
    if (e->est_relatif) {
        SDL_FRect rect_absolu = convertir_vers_absolu(dst, zone_jeu);
        SDL_RenderCopyF(rend, e->texture, src, &rect_absolu);
    }
    // Sinon, afficher directement à l'emplacement donné
    else
        SDL_RenderCopyF(rend, e->texture, src, dst);

    // Affichage de la hitbox de l'entité si nécessaire
    if (e->doit_afficher_hitbox) {
        SDL_FRect rect_absolu = convertir_vers_absolu(&(e->hitbox), zone_jeu);
        SDL_SetRenderDrawColor(rend, COULEUR_HITBOX_R, COULEUR_HITBOX_V,
                               COULEUR_HITBOX_B, COULEUR_HITBOX_A);
        SDL_RenderDrawRectF(rend, &rect_absolu);
    }

    // Affichage de la hitbox d'attaque du PNJ si nécessaire
    if (e->pnj && e->pnj->doit_afficher_hitbox_attaque) {
        SDL_FRect rect_absolu = convertir_vers_absolu(&(e->pnj->hitbox_attaque), zone_jeu);
        SDL_SetRenderDrawColor(rend, COULEUR_HITBOX_ATTQ_R, COULEUR_HITBOX_ATTQ_V,
                               COULEUR_HITBOX_ATTQ_B, COULEUR_HITBOX_ATTQ_A);
        SDL_RenderDrawRectF(rend, &rect_absolu);
    }

    // Affichage de la hitbox d'attaque du personnage si nécessaire
    if (e->perso && e->perso->doit_afficher_hitbox_attaque) {
        SDL_FRect rect_absolu = convertir_vers_absolu(&(e->perso->hitbox_attaque), zone_jeu);
        SDL_SetRenderDrawColor(rend, COULEUR_HITBOX_ATTQ_R, COULEUR_HITBOX_ATTQ_V,
                               COULEUR_HITBOX_ATTQ_B, COULEUR_HITBOX_ATTQ_A);
        SDL_RenderDrawRectF(rend, &rect_absolu);
    }
}


/**
 * @brief Change les coordonnées et les dimensions d'un rectangle.
 * @param rect Le rectangle à modifier.
 * @param x La nouvelle coordonnée en x.
 * @param y La nouvelle coordonnée en y.
 * @param w La nouvelle largeur.
 * @param h La nouvelle hauteur.
 */

void changer_rect(SDL_FRect * rect, float x, float y, float w, float h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}



/**
 * @brief Change les coordonnées source d'une entité.
 * @param e L'entité à modifier.
 * @param x La nouvelle coordonnée source en x.
 * @param y La nouvelle coordonnée source en y.
 * @param w La nouvelle largeur source.
 * @param h La nouvelle hauteur source.
 */

void changer_rect_src_entite(t_entite * e, int x, int y, int w, int h) {
    changer_rect((SDL_FRect*)e->rect_src, (float)x, (float)y, (float)w, (float)h);
}



/**
 * @brief Change les coordonnées et les dimensions d'un rectangle destination d'une entité.
 * @param e L'entité dont le rectangle destination doit être modifié.
 * @param x La nouvelle coordonnée en x.
 * @param y La nouvelle coordonnée en y.
 * @param w La nouvelle largeur.
 * @param h La nouvelle hauteur.
 */
void changer_rect_dst_entite(t_entite * e, float x, float y, float w, float h) {
    changer_rect(e->rect_dst, x, y, w, h);
}


/**
 * @brief Change les coordonnées source d'une entité.
 * @param e L'entité dont les coordonnées source doivent être modifiées.
 * @param x La nouvelle coordonnée source en x.
 * @param y La nouvelle coordonnée source en y.
 */
void changer_sprite(t_entite * e, int x, int y) {
    e->rect_src->x = x;
    e->rect_src->y = y;
}


/**
 * @brief Change la position d'une entité.
 * @param e L'entité dont la position doit être modifiée.
 * @param x La nouvelle coordonnée en x.
 * @param y La nouvelle coordonnée en y.
 */
void changer_pos_entite(t_entite * e, float x, float y) {
    // Sauvegarde des anciennes coordonnées x et y
    float ancien_x = e->rect_dst->x;
    float ancien_y = e->rect_dst->y;

    // Calcul de la nouvelle coordonnée x de la hitbox par rapport à la position absolue
    float nouv_hitbox_x = e->hitbox.x + x - ancien_x;

    // Vérification si la nouvelle position de la hitbox est valide (dans les limites de l'écran)
    if (nouv_hitbox_x > 0 && nouv_hitbox_x < 100) {
        // Mise à jour de la position de l'entité et de la hitbox en x
        e->rect_dst->x = x;
        e->hitbox.x = nouv_hitbox_x;
    }

    // Mise à jour de la position de l'entité en y et de la position y de la hitbox
    e->rect_dst->y = y;
    e->hitbox.y += y - ancien_y;

    // Mise à jour des coordonnées de la hitbox d'attaque du PNJ, le cas échéant
    if (e->pnj) {
        e->pnj->hitbox_attaque.x += x - ancien_x;
        e->pnj->hitbox_attaque.y += y - ancien_y;
    }

    // Mise à jour des coordonnées de la hitbox d'attaque du personnage, le cas échéant
    if (e->perso) {
        e->perso->hitbox_attaque.x += x - ancien_x;
        e->perso->hitbox_attaque.y += y - ancien_y;
    }
}




/**
 * @brief Change les dimensions d'une entité.
 * @param e L'entité dont les dimensions doivent être modifiées.
 * @param w La nouvelle largeur.
 * @param h La nouvelle hauteur.
 */

void changer_dims(t_entite * e, float w, float h) {
    e->rect_dst->w = w;
    e->rect_dst->h = h;
}


/**
 * @brief Change la position relative d'une entité.
 * @param e L'entité dont la position relative doit être modifiée.
 * @param dx Le déplacement en x.
 * @param dy Le déplacement en y.
 */
void changer_pos_rel(t_entite * e, float dx, float dy) {
    changer_pos_entite(e, e->rect_dst->x + dx,
                           e->rect_dst->y + dy);
}



/**
 * @brief Change la hitbox d'une entité.
 * @param e L'entité dont la hitbox doit être modifiée.
 * @param hitbox Le rectangle de la hitbox.
 * @param x La coordonnée x de la hitbox.
 * @param y La coordonnée y de la hitbox.
 * @param w La largeur de la hitbox.
 * @param h La hauteur de la hitbox.
 * @param avec_coord_rect_dst Indique si les coordonnées sont relatives au rectangle destination.
 */
void changer_hitbox(t_entite * e, SDL_FRect * hitbox, float x, float y, float w, float h, int avec_coord_rect_dst) {
    float dst_x = e->rect_dst->x;
    float dst_y = e->rect_dst->y;
    float dst_w = e->rect_dst->w;
    float dst_h = e->rect_dst->h;
    if (avec_coord_rect_dst) {
        hitbox->x = dst_x + dst_w * x/100;
        hitbox->y = dst_y + dst_h * y/100;
        hitbox->w = dst_w * w/100;
        hitbox->h = dst_h * h/100;
    }
    else {
        hitbox->x = x;
        hitbox->y = y;
        hitbox->w = w;
        hitbox->h = h;
    }
}


/**
 * @brief Applique une réflexion horizontale à la hitbox d'une entité.
 * @param e L'entité dont la hitbox doit être réfléchie.
 * @param hitbox Le rectangle de la hitbox.
 */
void appliquer_reflexion_hitbox(t_entite * e, SDL_FRect * hitbox) {
    changer_hitbox(e, hitbox, 2*e->hitbox.x + e->hitbox.w - hitbox->x - hitbox->w,
                   hitbox->y, hitbox->w, hitbox->h, FAUX);
}


/**
 * @brief Calcule le pas de déplacement selon la vitesse de l'entité.
 * @param compteur_frames Le compteur de frames.
 * @param vitesse La vitesse de l'entité.
 * @return int Le pas de déplacement.
 */
int calculer_pas_selon_vitesse(long long int compteur_frames, float vitesse) {
    if (vitesse >= 1)
        return (int) vitesse;
    else if (compteur_frames % (int) (1/(vitesse)) == 0)
        return 1;
    else
        return 0;
}


/**
 * @brief Déplace une entité en fonction de son déplacement actuel.
 * @param e L'entité à déplacer.
 * @param compteur_frames Le compteur de frames.
 */
void deplacer(t_entite * e, long long int compteur_frames) {
    float depl_x = 0;
    float depl_y = 0;
    if (e->deplacement == GAUCHE && !e->collisions.g)
        depl_x = -1;
    if (e->deplacement == DROITE && !e->collisions.d)
        depl_x = 1;
    if (e->deplacement == HAUT && !e->collisions.h)
        depl_y = -1;
    if (e->deplacement == BAS && !e->collisions.b)
        depl_y = 1;
    int pas = calculer_pas_selon_vitesse(compteur_frames, e->vitesse);
    depl_x *= pas;
    depl_y *= pas;
    if (e->deplacement != REPOS_MVT) {
        changer_pos_rel(e, depl_x, depl_y);
        if (e->deplacement == GAUCHE || e->deplacement == DROITE) {
            e->sens_regard = e->deplacement;
            if (e->perso && e->sens_regard != e->sens_regard_prec)
                appliquer_reflexion_hitbox(e, &(e->perso->hitbox_attaque));
            e->sens_regard_prec = e->sens_regard;
        }
    }
}



/**
 * @brief Anime une entité en fonction de son animation actuelle.
 * @param e L'entité à animer.
 * @param compteur_frames Le compteur de frames.
 */
void animer(t_entite * e, long long int compteur_frames) {
    int pas_anim;
    t_animation * anim = e->animation_courante;

    if (anim->longueur == 1)
        pas_anim = 0;
    else
        pas_anim = calculer_pas_selon_vitesse(compteur_frames, anim->vitesse_anim);

    if (anim->longueur == 1)
        e->x_sprite = anim->x_sprite_ini;
    else
        e->x_sprite = (e->x_sprite + pas_anim) % anim->longueur;
    e->y_sprite = anim->y_sprite;

    changer_sprite(e, e->x_sprite * anim->w_sprite, e->y_sprite);
    if (e->id_animation_suivante != ANIM_NULLE && e->x_sprite == anim->longueur - 1) {
        changer_animation(e, e->id_animation_suivante);
        e->id_animation_suivante = ANIM_NULLE;
    }
}


/**
 * @brief Change l'animation courante d'une entité.
 * @param e L'entité dont l'animation doit être changée.
 * @param id_anim L'identifiant de la nouvelle animation.
 */
void changer_animation(t_entite * e, t_id_anim id_anim) {
    t_animation * anim = recuperer_animation(e->animations, e->n_animations, id_anim);
    if (anim) {
        e->x_sprite = 0;

        if (e->rect_src->w != -1) {
            e->w_src_prec = e->rect_src->w;
            e->h_src_prec = e->rect_src->h;
        }
        else {
            e->w_src_prec = anim->w_sprite;
            e->h_src_prec = anim->h_sprite;
        }

        if (anim != e->animation_courante) {
            e->rect_dst->x += e->dec_x_dst_prec;
            e->rect_dst->y += e->dec_y_dst_prec;
            e->rect_dst->x -= anim->decalage_dest_x;
            e->rect_dst->y -= anim->decalage_dest_y;
            e->dec_x_dst_prec = anim->decalage_dest_x;
            e->dec_y_dst_prec = anim->decalage_dest_y;
        }

        e->animation_courante = anim;
        e->rect_src->x = anim->x_sprite_ini;
        e->rect_src->y = anim->y_sprite;
        e->rect_src->w = anim->w_sprite;
        e->rect_src->h = anim->h_sprite;

        e->rect_dst->w *= anim->w_sprite / (float)e->w_src_prec;
        e->rect_dst->h *= anim->h_sprite / (float)e->h_src_prec;
    }
}



/**
 * @brief Crée une nouvelle entité à partir d'une texture SDL.
 * @param texture La texture SDL à utiliser pour l'entité.
 * @param x La coordonnée en x de l'entité.
 * @param y La coordonnée en y de l'entité.
 * @param w La largeur de l'entité.
 * @param h La hauteur de l'entité.
 * @param est_relatif Indique si les coordonnées sont relatives.
 * @return t_entite* Un pointeur vers la nouvelle entité créée.
 */
t_entite * creer_entite_depuis_texture(SDL_Texture * texture,
                                       float x, float y, float w, float h,
                                       int est_relatif) {
    t_entite * nouv;
    nouv = malloc(sizeof(t_entite));

    nouv->texture = texture;
    nouv->rect_src = malloc(sizeof(SDL_Rect));
    nouv->rect_dst = malloc(sizeof(SDL_Rect));
    nouv->rect_src->x = -1;
    nouv->rect_src->y = -1;
    nouv->rect_src->w = -1;
    nouv->rect_src->h = -1;
    nouv->rect_dst->x = -1;
    nouv->rect_dst->y = -1;
    nouv->rect_dst->w = -1;
    nouv->rect_dst->h = -1;

    // ne pas utiliser changer_pos_entite car nécessite hitbox, qui elle-même nécessite rect_dst
    nouv->rect_dst->x = x;
    nouv->rect_dst->y = y;
    changer_dims(nouv, w, h);
    changer_hitbox(nouv, &(nouv->hitbox), 0, 0, 100, 100, VRAI);

    nouv->doit_afficher_hitbox = FAUX;
    nouv->est_relatif = est_relatif;
    nouv->deplacement = REPOS_MVT;
    nouv->deplacement_prec = REPOS_MVT;
    nouv->sens_regard = DROITE;
    nouv->sens_regard_prec = nouv->sens_regard;
    nouv->x_sprite = nouv->y_sprite = 0;

    nouv->vitesse = 0;

    nouv->dec_x_dst_prec = 0;
    nouv->dec_y_dst_prec = 0;
    nouv->doit_restaurer_dst = FAUX;

    nouv->collisions.g = NULL;
    nouv->collisions.d = NULL;
    nouv->collisions.h = NULL;
    nouv->collisions.b = NULL;

    nouv->n_animations = 0;
    nouv->animations = NULL;
    nouv->id_animation_suivante = ANIM_NULLE;

    nouv->destructible = NULL;
    nouv->est_obstacle = FAUX;
    nouv->pnj = NULL;
    nouv->perso = NULL;
    nouv->bonus = NULL;

    return nouv;
}


/**
 * @brief Crée une nouvelle entité à partir d'une spritesheet.
 * @param id L'identifiant de la spritesheet à utiliser.
 * @param x La coordonnée en x de l'entité.
 * @param y La coordonnée en y de l'entité.
 * @param w La largeur de l'entité.
 * @param h La hauteur de l'entité.
 * @param est_relatif Indique si les coordonnées sont relatives.
 * @return t_entite* Un pointeur vers la nouvelle entité créée.
 */
t_entite * creer_entite_depuis_spritesheet(const char * id,
                                           float x, float y, float w, float h,
                                           int est_relatif) {
    t_spritesheet * spritesheet = recuperer_spritesheet(id);
    t_entite * nouv = creer_entite_depuis_texture(spritesheet->texture, x, y, w, h, est_relatif);
    nouv->animations = spritesheet->animations;
    nouv->n_animations = spritesheet->n_animations;
    changer_animation(nouv, REPOS);
    strcpy(nouv->type, id);
    nouv->numero = numeros_entites++;
    return nouv;
}



/**
 * @brief Crée une nouvelle entité à partir d'une texture chargée.
 * @param id L'identifiant de la texture à utiliser.
 * @param x La coordonnée en x de l'entité.
 * @param y La coordonnée en y de l'entité.
 * @param w La largeur de l'entité.
 * @param h La hauteur de l'entité.
 * @param est_relatif Indique si les coordonnées sont relatives.
 * @return t_entite* Un pointeur vers la nouvelle entité créée.
 */
t_entite * creer_entite(const char * id, float x, float y, float w, float h,
                        int est_relatif) {
    SDL_Texture * texture = recuperer_texture(id);
    t_entite * nouv = creer_entite_depuis_texture(texture, x, y, w, h, est_relatif);
    strcpy(nouv->type, id);
    nouv->numero = numeros_entites++;
    return nouv;
}



/**
 * @brief Détruit une entité et libère la mémoire allouée.
 * @param e Un pointeur vers le pointeur de l'entité à détruire.
 */

// ne pas détruire (*e)->texture, detruire_ressources s’en charge
void detruire_entite(t_entite ** e) {
    if (*e) {
        if ((*e)->rect_src)
            free((*e)->rect_src);
        if ((*e)->rect_dst)
            free((*e)->rect_dst);
        detruire_destructible(&((*e)->destructible));
        detruire_pnj(&((*e)->pnj));
        detruire_perso(&((*e)->perso));
        detruire_bonus(&((*e)->bonus));
        free(*e);
    }
    *e = NULL;
}
