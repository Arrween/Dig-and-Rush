/**
 *  \file entite.c
 *  \brief Gestion des entités, tout objet devant être affiché
 */

#include <limits.h>
#include <string.h>

#include "entite.h"
#include "spritesheets.h"
#include "constantes.h"

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

    SDL_FRect zone_jeu = {TAILLE_L/4, 0, TAILLE_L/2, TAILLE_H};

    SDL_Rect * src = e->rect_src->w >= 0 ? e->rect_src : NULL;
    SDL_FRect * dst = e->rect_dst->w >= 0 ? e->rect_dst : NULL;
    if (e->est_relatif) {
        SDL_FRect rect_absolu = convertir_vers_absolu(dst, zone_jeu);
        SDL_RenderCopyF(rend, e->texture, src, &rect_absolu);
    }
    else
        SDL_RenderCopyF(rend, e->texture, src, dst);
    if (e->doit_afficher_hitbox) {
        SDL_FRect rect_absolu = convertir_vers_absolu(&(e->hitbox), zone_jeu);
        SDL_SetRenderDrawColor(rend, COULEUR_HITBOX_R, COULEUR_HITBOX_V,
                               COULEUR_HITBOX_B, COULEUR_HITBOX_A);
        SDL_RenderDrawRectF(rend, &rect_absolu);
    }
}

/*
 * 
 *
 *
 *
 */
void changer_rect(SDL_FRect * rect, float x, float y, float w, float h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void changer_rect_src_entite(t_entite * e, int x, int y, int w, int h) {
    changer_rect((SDL_FRect*)e->rect_src, (float)x, (float)y, (float)w, (float)h);
}

void changer_rect_dst_entite(t_entite * e, float x, float y, float w, float h) {
    changer_rect(e->rect_dst, x, y, w, h);
}

void changer_sprite(t_entite * e, int x, int y) {
    e->rect_src->x = x;
    e->rect_src->y = y;
}

void changer_pos_entite(t_entite * e, float x, float y) {
    float xx = e->rect_dst->x;
    float yy = e->rect_dst->y;
    float nouv_hitbox_x = e->hitbox.x + x - xx;
    if (nouv_hitbox_x > 0 && nouv_hitbox_x < 100) {
        e->rect_dst->x = x;
        e->hitbox.x = nouv_hitbox_x;
    }
    e->rect_dst->y = y;
    e->hitbox.y += y - yy;
}

void changer_dims(t_entite * e, float w, float h) {
    e->rect_dst->w = w;
    e->rect_dst->h = h;
}

void changer_pos_rel(t_entite * e, float dx, float dy) {
    changer_pos_entite(e, e->rect_dst->x + dx,
                           e->rect_dst->y + dy);
}

void changer_hitbox(t_entite * e, int x, int y, int w, int h) {
    float dst_x = e->rect_dst->x;
    float dst_y = e->rect_dst->y;
    float dst_w = e->rect_dst->w;
    float dst_h = e->rect_dst->h;
    if (x > INT_MIN)
        e->hitbox.x = dst_x + dst_w * (float)x/100;
    if (y > INT_MIN)
        e->hitbox.y = dst_y + dst_h * (float)y/100;
    if (w > -1)
        e->hitbox.w = dst_w * (float)w/100;
    if (h > -1)
        e->hitbox.h = dst_h * (float)h/100;
}

int calculer_pas_selon_vitesse(long long int compteur_frames, float vitesse) {
    if (vitesse >= 1)
        return (int) vitesse;
    else if (compteur_frames % (int) (1/(vitesse)) == 0)
        return 1;
    else
        return 0;
}

void deplacer(t_entite * e, long long int compteur_frames) {
    float depl_x = 0;
    float depl_y = 0;
    if (e->deplacement == GAUCHE && !e->a_collision_g)
        depl_x = -1;
    if (e->deplacement == DROITE && !e->a_collision_d)
        depl_x = 1;
    if (e->deplacement == HAUT && !e->a_collision_h)
        depl_y = -1;
    if (e->deplacement == BAS && !e->a_collision_b)
        depl_y = 1;
    int pas = calculer_pas_selon_vitesse(compteur_frames, e->vitesse);
    depl_x *= pas;
    depl_y *= pas;
    if (e->deplacement != REPOS_MVT) {
        changer_pos_rel(e, depl_x, depl_y);
        if (e->deplacement == GAUCHE || e->deplacement == DROITE)
            e->sens_regard = e->deplacement;
    }
}

void animer(t_entite * e, long long int compteur_frames) {
    int pas_anim;
    t_animation * anim = e->animation_courante;

    if (anim->longueur == 1*anim->w_sprite)
        pas_anim = 0;
    else
        pas_anim = calculer_pas_selon_vitesse(compteur_frames, anim->vitesse_anim);

    if (! e->a_collision_b || anim->id == REPOS)
        e->x_sprite = anim->x_sprite_ini;
    else
        e->x_sprite = (e->x_sprite + pas_anim) % anim->longueur;
    e->y_sprite = anim->y_sprite;
    changer_sprite(e, e->x_sprite * anim->w_sprite, e->y_sprite);
}

void changer_animation(t_entite * e, t_id_anim id_anim) {
    t_animation * anim = recuperer_animation(e->animations, e->n_animations, id_anim);
    if (anim) {
        if (anim->decalage_dest_x != 0 || anim->decalage_dest_y != 0)
            e->doit_restaurer_dst = VRAI;
        if (anim->decalage_dest_x != 0)
            e->dec_x_dst_prec = anim->decalage_dest_x;
        if (anim->decalage_dest_y != 0)
            e->dec_y_dst_prec = anim->decalage_dest_y;

        if (e->rect_src->w != -1) {
            e->w_src_prec = e->rect_src->w;
            e->h_src_prec = e->rect_src->h;
        }
        else {
            e->w_src_prec = anim->w_sprite;
            e->h_src_prec = anim->h_sprite;
        }

        e->animation_courante = anim;
        e->rect_src->x = anim->x_sprite_ini;
        e->rect_src->y = anim->y_sprite;
        e->rect_src->w = anim->w_sprite;
        e->rect_src->h = anim->h_sprite;

        e->rect_dst->w *= anim->w_sprite / (float)e->w_src_prec;
        e->rect_dst->h *= anim->h_sprite / (float)e->h_src_prec;

        if (anim->decalage_dest_x == 0 && e->doit_restaurer_dst)
            e->rect_dst->x += e->dec_x_dst_prec;
        else
            e->rect_dst->x -= anim->decalage_dest_x;
        if (anim->decalage_dest_y == 0 && e->doit_restaurer_dst)
            e->rect_dst->y += e->dec_y_dst_prec;
        else
            e->rect_dst->y -= anim->decalage_dest_y;
        if ((anim->decalage_dest_x == 0 || anim->decalage_dest_y == 0) && e->doit_restaurer_dst)
            e->doit_restaurer_dst = FAUX;
    }
}

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
    changer_hitbox(nouv, 0, 0, 100, 100);

    nouv->doit_afficher_hitbox = FAUX;
    nouv->est_relatif = est_relatif;
    nouv->deplacement = REPOS;
    nouv->sens_regard = DROITE;
    nouv->x_sprite = nouv->y_sprite = 0;

    nouv->vitesse = 0;

    nouv->dec_x_dst_prec = 0;
    nouv->dec_y_dst_prec = 0;
    nouv->doit_restaurer_dst = FAUX;

    nouv->a_collision_g = FAUX;
    nouv->a_collision_d = FAUX;
    nouv->a_collision_b = FAUX;
    nouv->a_collision_h = FAUX;

    nouv->n_animations = 0;
    nouv->animations = NULL;

    nouv->destructible = NULL;
    nouv->est_obstacle = FAUX;
    nouv->pnj = NULL;

    return nouv;
}

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

t_entite * creer_entite(const char * id, float x, float y, float w, float h,
                        int est_relatif) {
    SDL_Texture * texture = recuperer_texture(id);
    t_entite * nouv = creer_entite_depuis_texture(texture, x, y, w, h, est_relatif);
    strcpy(nouv->type, id);
    nouv->numero = numeros_entites++;
    return nouv;
}

// ne pas détruire (*e)->texture, detruire_ressources s’en charge
void detruire_entite(t_entite ** e) {
    if (*e) {
        if ((*e)->rect_src)
            free((*e)->rect_src);
        if ((*e)->rect_dst)
            free((*e)->rect_dst);
        free(*e);
    }
    *e = NULL;
}
