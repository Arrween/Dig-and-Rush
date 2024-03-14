#include <limits.h>
#include <stdarg.h>

#include "entite.h"
#include "ressources.h"
#include "spritesheets.h"
#include "constantes.h"

SDL_Rect convertir_vers_absolu(SDL_Rect * rect, int x_abs, int y_abs,
                                                int w_abs, int h_abs) {
    SDL_Rect retour;
    retour.x = x_abs + w_abs * rect->x/100;
    retour.y = y_abs + h_abs * rect->y/100;
    retour.w = w_abs * rect->w/100;
    retour.h = h_abs * rect->h/100;
    return retour;
}

void afficher_entite(SDL_Renderer * rend, t_entite * e) {

    int largeur_tour = TAILLE_L/2;
    int x_tour = (TAILLE_L-largeur_tour)/2;
    int x_mur_g = (int) (x_tour + largeur_tour * 0.07);
    int x_mur_d = (int) (x_tour + largeur_tour * 0.93);
    int largeur_zone_jeu = x_mur_d - x_mur_g;

    SDL_Rect * src = e->affichage->rect_src->x != -1 ?
                                        e->affichage->rect_src : NULL;
    if (e->est_relatif) {
        SDL_Rect rect_absolu = convertir_vers_absolu(e->affichage->rect_dst,
                                                     x_tour, 0,
                                                     largeur_zone_jeu,
                                                     TAILLE_H);
        SDL_RenderCopy(rend, e->affichage->texture, src,
                       &rect_absolu);
    }
    else
        SDL_RenderCopy(rend, e->affichage->texture, src,
                       e->affichage->rect_dst);
    if (e->doit_afficher_hitbox) {
        SDL_Rect rect_absolu = convertir_vers_absolu(&(e->hitbox),
                                                     x_tour, 0,
                                                     largeur_zone_jeu,
                                                     TAILLE_H);
        SDL_SetRenderDrawColor(rend, COULEUR_HITBOX_R, COULEUR_HITBOX_V,
                               COULEUR_HITBOX_B, COULEUR_HITBOX_A);
        SDL_RenderDrawRect(rend, &rect_absolu);
    }
}

void changer_rect(SDL_Rect * rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void changer_rect_src_entite(t_entite * e, int x, int y, int w, int h) {
    changer_rect(e->affichage->rect_src, x, y, w, h);
}

void changer_rect_dst_entite(t_entite * e, int x, int y, int w, int h) {
    changer_rect(e->affichage->rect_dst, x, y, w, h);
}

void changer_sprite(t_entite * e, int x, int y) {
    e->affichage->rect_src->x = x * e->affichage->rect_src->w;
    e->affichage->rect_src->y = y * e->affichage->rect_src->h;
}

void changer_pos_entite(t_entite * e, int x, int y) {
    int xx = e->affichage->rect_dst->x;
    int yy = e->affichage->rect_dst->y;
    int nouv_hitbox_x = e->hitbox.x + x - xx;
    if (nouv_hitbox_x > 0 && nouv_hitbox_x < 100) {
        e->affichage->rect_dst->x = x;
        e->hitbox.x = nouv_hitbox_x;
    }
    e->affichage->rect_dst->y = y;
    e->hitbox.y += y - yy;
}

void changer_dims(t_entite * e, int w, int h) {
    e->affichage->rect_dst->w = w;
    e->affichage->rect_dst->h = h;
}

void changer_pos_rel_entite(t_entite * e, int dx, int dy) {
    changer_pos_entite(e, e->affichage->rect_dst->x + dx,
                           e->affichage->rect_dst->y + dy);
}

void changer_hitbox(t_entite * e, int x, int y, int w, int h) {
    int dst_x = e->affichage->rect_dst->x;
    int dst_y = e->affichage->rect_dst->y;
    int dst_w = e->affichage->rect_dst->w;
    int dst_h = e->affichage->rect_dst->h;
    if (x > INT_MIN)
        e->hitbox.x = dst_x + dst_w * x/100;
    if (y > INT_MIN)
        e->hitbox.y = dst_y + dst_h * y/100;
    if (w > -1)
        e->hitbox.w = dst_w * w/100;
    if (h > -1)
        e->hitbox.h = dst_h * h/100;
}

void deplacer(t_entite * e) {
    if (e->deplacement != REPOS) {
        e->changer_pos_rel(e,
            e->deplacement == GAUCHE ? -1 : (e->deplacement == DROITE?1:0),
            e->deplacement == HAUT ? -1 : (e->deplacement == BAS ? 1 : 0)
        );
        if (e->deplacement == GAUCHE || e->deplacement == DROITE)
            e->sens_regard = e->deplacement;
    }
}

void definir_animations(t_entite * e, int n_animations, ...) {
    if (n_animations <= 0)
        return;
    e->animations = realloc(e->animations, sizeof(t_animation) * n_animations);
    va_list ap;
    va_start(ap, n_animations);
    // décalage de 1 car REPOS par défaut en position 0
    for (int i = 1; i < n_animations + 1; i++)
        e->animations[i] = va_arg(ap, int);
    e->n_animations = n_animations + 1;
}

int calculer_pas_anim(int compteur_frames, float vitesse_anim) {
    if (vitesse_anim >= 1)
        return (int) vitesse_anim;
    else if (compteur_frames % (int) (1/(vitesse_anim)) == 0)
        return 1;
    else
        return 0;
}

void animer(t_entite * e, int compteur_frames) {
    int i;
    int pas_anim;
    for (i = 0; i < e->n_animations && e->animations[i] != e->animation_courante; i++);
    if (i >= e->n_animations)
        return;
    if (e->animation_courante == REPOS) {
        if (e->a_collision)
            e->changer_sprite(e, X_PERSO_REPOS, Y_PERSO_REPOS);
        else
            e->changer_sprite(e, X_PERSO_CHUTE,
                                      e->sens_regard == GAUCHE ?
                                        Y_PERSO_CHUTE_G : Y_PERSO_CHUTE_D);
    }
    else if (e->animation_courante == DEPL_G || e->animation_courante == DEPL_D) {
        if (e->a_collision) {
            pas_anim = calculer_pas_anim(compteur_frames, VITESSE_ANIM_MARCHE);
            e->y_sprite = e->animation_courante == DEPL_G ?
                            Y_PERSO_PELLE_MARCHE_G : Y_PERSO_PELLE_MARCHE_D;
            e->x_sprite = (e->x_sprite + pas_anim) % LONGUEUR_ANIM_MARCHE;
        }
        else {
            e->y_sprite = e->deplacement == GAUCHE ?
                            Y_PERSO_CHUTE_G : Y_PERSO_CHUTE_D;
            e->x_sprite = X_PERSO_CHUTE;
        }
        e->changer_sprite(e, e->x_sprite, e->y_sprite);
    }
    else if (e->animation_courante == CREUSER) {
        pas_anim = calculer_pas_anim(compteur_frames, VITESSE_ANIM_CREUSAGE);
        e->y_sprite = Y_PERSO_CREUSER;
        e->x_sprite = (e->x_sprite + pas_anim) % LONGUEUR_ANIM_CREUSAGE;
        e->changer_sprite(e, e->x_sprite, e->y_sprite);
    }
}

t_entite * creer_entite_depuis_texture(SDL_Texture * texture,
                                       int x, int y, int w, int h,
                                       int est_relatif) {
    t_entite * nouv;
    nouv = malloc(sizeof(t_entite));

    nouv->affichage = creer_affichage(texture);

    nouv->afficher = afficher_entite;
    nouv->changer_rect_src = changer_rect_src_entite;
    nouv->changer_rect_dst = changer_rect_dst_entite;
    nouv->changer_sprite = changer_sprite;
    nouv->changer_pos = changer_pos_entite;
    nouv->changer_dims = changer_dims;
    nouv->changer_pos_rel = changer_pos_rel_entite;

    if (x == -1 && y == -1 && w == -1 && h == -1) {
        free(nouv->affichage->rect_dst);
        nouv->affichage->rect_dst = NULL;
    }
    else {
        // ne pas utiliser changer_pos_entite car nécessite hitbox, qui elle-même nécessite rect_dst
        nouv->affichage->rect_dst->x = x;
        nouv->affichage->rect_dst->y = y;
        nouv->changer_dims(nouv, w, h);
        changer_hitbox(nouv, 0, 0, 100, 100);
    }
    nouv->doit_afficher_hitbox = FAUX;
    nouv->est_relatif = est_relatif;
    nouv->deplacement = REPOS;
    nouv->sens_regard = DROITE;
    nouv->x_sprite = nouv->y_sprite = 0;

    nouv->animations = malloc(sizeof(t_animation));
    nouv->animations[0] = REPOS;
    nouv->animation_courante = REPOS;

    return nouv;
}

t_entite * creer_entite_depuis_spritesheet(const char * id,
                                           int x, int y, int w, int h,
                                           int est_relatif) {
    t_spritesheet * spritesheet = recuperer_spritesheet(id);
    t_entite * nouv = creer_entite_depuis_texture(spritesheet->texture, x, y, w, h, est_relatif);
    nouv->affichage->rect_src->w = spritesheet->sprite_l;
    nouv->affichage->rect_src->h = spritesheet->sprite_h;
    nouv->a_animations = VRAI;
    return nouv;
}

t_entite * creer_entite(const char * id, int x, int y, int w, int h,
                        int est_relatif) {
    SDL_Texture * texture = recuperer_texture(id);
    return creer_entite_depuis_texture(texture, x, y, w, h, est_relatif);
}

void detruire_entite(t_entite ** e) {
    if (*e) {
        detruire_affichage(&((*e)->affichage));
        if ((*e)->animations)
            free((*e)->animations);
        free(*e);
    }
    *e = NULL;
}

t_affichage * creer_affichage(SDL_Texture * texture) {
    t_affichage * nouv = malloc(sizeof(t_affichage));

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

    return nouv;
}

// ne pas détruire (*aff)->texture, detruire_ressources s’en charge
void detruire_affichage(t_affichage ** affichage) {
    if (*affichage) {
        if ((*affichage)->rect_src)
            free((*affichage)->rect_src);
        if ((*affichage)->rect_dst)
            free((*affichage)->rect_dst);
        if (*affichage)
            free(*affichage);
    }
    *affichage = NULL;
}
