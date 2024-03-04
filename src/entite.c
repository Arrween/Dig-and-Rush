#include "entite.h"
#include "ressources.h"

#include "constantes.h"

void afficher_entite(SDL_Renderer * rend, t_entite * e) {
    int largeur_tour = TAILLE_L/2;
    int x_tour = (TAILLE_L-largeur_tour)/2;
    int x_mur_g = (int) (x_tour + largeur_tour * 0.07);
    int x_mur_d = (int) (x_tour + largeur_tour * 0.93);
    int largeur_zone_jeu = x_mur_d - x_mur_g;

    SDL_Rect * src = e->affichage->rect_src->x != -1 ?
                                        e->affichage->rect_src : NULL;
    if (e->est_relatif) {
        SDL_Rect rect_absolu;
        rect_absolu.x = x_mur_g + largeur_zone_jeu * e->affichage->rect_dst->x/100;
        rect_absolu.y = TAILLE_H * e->affichage->rect_dst->y/100;
        rect_absolu.w = largeur_zone_jeu * e->affichage->rect_dst->w/100;
        rect_absolu.h = TAILLE_H * e->affichage->rect_dst->h/100;
        SDL_RenderCopy(rend, e->affichage->texture, src,
                       &rect_absolu);
    }
    else
        SDL_RenderCopy(rend, e->affichage->texture, src,
                       e->affichage->rect_dst);
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

void deplacer_entite(t_entite * e, int x, int y) {
    e->affichage->rect_dst->x = x >0 ? (x < 100 ? x : 100) : 0;
    e->affichage->rect_dst->y = y;
}

void changer_dims(t_entite * e, int w, int h) {
    e->affichage->rect_dst->w = w;
    e->affichage->rect_dst->h = h;
}

void deplacer_rel_entite(t_entite * e, int dx, int dy) {
    deplacer_entite(e, e->affichage->rect_dst->x + dx,
                           e->affichage->rect_dst->y + dy);
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
    nouv->deplacer = deplacer_entite;
    nouv->changer_dims = changer_dims;
    nouv->deplacer_rel = deplacer_rel_entite;

    if (x == -1 && y == -1 && w == -1 && h == -1) {
        free(nouv->affichage->rect_dst);
        nouv->affichage->rect_dst = NULL;
    }
    else {
        nouv->deplacer(nouv, x, y);
        nouv->changer_dims(nouv, w, h);
    }
    nouv->est_relatif = est_relatif;

    return nouv;
}

t_entite * creer_entite_depuis_spritesheet(const char * id,
                                           int x, int y, int w, int h,
                                           int est_relatif) {
    t_spritesheet * spritesheet = recuperer_spritesheet(id);
    t_entite * nouv = creer_entite_depuis_texture(spritesheet->texture, x, y, w, h, est_relatif);
    nouv->affichage->rect_src->w = spritesheet->sprite_l;
    nouv->affichage->rect_src->h = spritesheet->sprite_h;
    return nouv;
}

t_entite * creer_entite(const char * id, int x, int y, int w, int h,
                        int est_relatif) {
    SDL_Texture * texture = recuperer_texture(id);
    return creer_entite_depuis_texture(texture, x, y, w, h, est_relatif);
}

void detruire_entite(t_entite ** e) {
    detruire_affichage(&((*e)->affichage));
    free(*e);
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
    if ((*affichage)->rect_src)
        free((*affichage)->rect_src);
    if ((*affichage)->rect_dst)
        free((*affichage)->rect_dst);
    if (*affichage)
        free(*affichage);
    *affichage = NULL;
}
