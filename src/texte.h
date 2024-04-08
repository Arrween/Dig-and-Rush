#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TAILLE_MAX_TEXTE 1000

typedef struct {
    TTF_Font * police;
    SDL_Surface * surface;
    SDL_Color couleur;
    SDL_Rect rect_dst;
    char contenu[TAILLE_MAX_TEXTE];
} t_texte;

t_texte * creer_texte(char * id, int r, int v, int b, int a,
                                 int x, int y, int w, int h);
void detruire_texte(t_texte**);
void changer_texte(t_texte*, char* format_contenu, ...);
void afficher_texte(SDL_Renderer*, t_texte*);
