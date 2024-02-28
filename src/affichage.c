#include <stdlib.h>

#include "affichage.h"

void render_affichage(SDL_Renderer * rend, t_affichage * affichage) {
    SDL_Rect * src = NULL;
    SDL_Rect * dst = NULL;
    if (affichage->rect_src->x != -1)
        src = affichage->rect_src;
    if (affichage->rect_dst->x != -1)
        dst = affichage->rect_dst;
    // if (dst)
    //     printf("affichage en %i %i %i %i\n", dst->x, dst->y, dst->w, dst->h);
    // else
    //     printf("affichage sur tout\n");
    SDL_RenderCopy(rend, affichage->texture, src, dst);
}

t_affichage* creer_affichage(SDL_Texture * texture,
                             int dst_x, int dst_y, int dst_w, int dst_h) {
    t_affichage * nouv = malloc(sizeof(t_affichage));

    nouv->texture = texture;
    nouv->rect_src = malloc(sizeof(SDL_Rect));
    nouv->rect_dst = malloc(sizeof(SDL_Rect));

    nouv->rect_src->x = -1;
    nouv->rect_src->y = -1;
    nouv->rect_src->w = -1;
    nouv->rect_src->h = -1;

    nouv->rect_dst->x = dst_x;
    nouv->rect_dst->y = dst_y;
    nouv->rect_dst->w = dst_w;
    nouv->rect_dst->h = dst_h;

    nouv->render = render_affichage;

    return nouv;
}

void detruire_affichage(t_affichage ** affichage) {
    SDL_DestroyTexture((*affichage)->texture);
    free((*affichage)->rect_src);
    free((*affichage)->rect_dst);
    free(*affichage);
    *affichage = NULL;
}
