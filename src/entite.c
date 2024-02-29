#include "entite.h"
#include "ressources.h"

void afficher_entite(SDL_Renderer * rend, t_entite * e) {
    SDL_Rect * src = NULL;
    SDL_Rect * dst = NULL;
    if (e->affichage->rect_src->x != -1)
        src = e->affichage->rect_src;
    if (e->affichage->rect_dst->x != -1)
        dst = e->affichage->rect_dst;
    if (e->affichage->rect_spritesheet->x != -1) {
        src = malloc(sizeof(SDL_Rect));
        src->w = e->affichage->rect_spritesheet->w;
        src->h = e->affichage->rect_spritesheet->h;
        src->x = src->w * e->affichage->rect_spritesheet->x;
        src->y = src->h * e->affichage->rect_spritesheet->y;
        SDL_RenderCopy(rend, e->affichage->texture, src, dst);
        free(src);
    }
    else
        SDL_RenderCopy(rend, e->affichage->texture, src, dst);
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

void changer_dims_spritesheet_entite(t_entite * e, int w, int h) {
    e->affichage->rect_spritesheet->w = w;
    e->affichage->rect_spritesheet->h = h;
}

void changer_pos_spritesheet_entite(t_entite * e, int x, int y) {
    e->affichage->rect_spritesheet->x = x;
    e->affichage->rect_spritesheet->y = y;
}


t_entite * creer_entite_depuis_texture(SDL_Texture * texture) {
    t_entite * nouv;
    nouv = malloc(sizeof(t_entite));
    nouv->affichage = creer_affichage(texture);
    nouv->afficher = afficher_entite;
    nouv->changer_rect_src = changer_rect_src_entite;
    nouv->changer_rect_dst = changer_rect_dst_entite;
    nouv->changer_dims_spritesheet = changer_dims_spritesheet_entite;
    nouv->changer_pos_spritesheet = changer_pos_spritesheet_entite;
    return nouv;
}

t_entite * creer_entite(const char * id) {
    SDL_Texture * texture = recuperer_texture(id);
    return creer_entite_depuis_texture(texture);
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
    nouv->rect_spritesheet = malloc(sizeof(SDL_Rect));

    nouv->rect_src->x = -1;
    nouv->rect_src->y = -1;
    nouv->rect_src->w = -1;
    nouv->rect_src->h = -1;

    nouv->rect_dst->x = -1;
    nouv->rect_dst->y = -1;
    nouv->rect_dst->w = -1;
    nouv->rect_dst->h = -1;

    nouv->rect_spritesheet->x = -1;
    nouv->rect_spritesheet->y = -1;
    nouv->rect_spritesheet->w = -1;
    nouv->rect_spritesheet->h = -1;

    return nouv;
}

// ne pas détruire (*aff)->texture, detruire_ressources s’en charge
void detruire_affichage(t_affichage ** affichage) {
    free((*affichage)->rect_src);
    free((*affichage)->rect_dst);
    free(*affichage);
    *affichage = NULL;
}
