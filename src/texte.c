#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdarg.h>

#include "texte.h"
#include "ressources.h"

void changer_texte(t_texte * texte, char * format_contenu, ...) {
    va_list ap;
    va_start(ap, format_contenu);
    char contenu[TAILLE_MAX_TEXTE];
    vsprintf(contenu, format_contenu, ap);
    texte->surface = TTF_RenderText_Solid(texte->police, contenu, texte->couleur);
    va_end(ap);
}

void afficher_texte(SDL_Renderer * rend, t_texte * texte) {
    SDL_Texture * texture = SDL_CreateTextureFromSurface(rend, texte->surface);
    SDL_RenderCopy(rend, texture, NULL, &texte->rect_dst);
    SDL_DestroyTexture(texture);
}

t_texte * creer_texte(char * id, int rouge, int vert, int bleu, int alpha, int x, int y, int w, int h) {
    TTF_Font * police = recuperer_police(id);
    if (!police) {
        fprintf(stderr, "creer_texte avec police « %s » a échoué\n", id);
        return NULL;
    }
    t_texte * nouv = malloc(sizeof(t_texte));
    nouv->police = police;
    strcpy(nouv->contenu, "");
    nouv->couleur.r = rouge;
    nouv->couleur.g = vert;
    nouv->couleur.b = bleu;
    nouv->couleur.a = alpha;
    nouv->rect_dst.x = x;
    nouv->rect_dst.y = y;
    nouv->rect_dst.w = w;
    nouv->rect_dst.h = h;
    nouv->surface = TTF_RenderText_Solid(nouv->police, nouv->contenu, nouv->couleur);
    return nouv;
}

void detruire_texte(t_texte ** texte) {
    if (*texte) {
        SDL_FreeSurface((*texte)->surface);
        free(*texte);
    }
    *texte = NULL;
}
