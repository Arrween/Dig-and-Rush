#include <SDL2/SDL.h>

typedef struct s_affichage {
    SDL_Texture * texture;
    SDL_Rect * rect_src;
    SDL_Rect * rect_dst;
    void (*render) (SDL_Renderer *, struct s_affichage *);
} t_affichage;

void render_affichage(SDL_Renderer *, t_affichage *);

t_affichage* creer_affichage(SDL_Texture *, int, int, int, int);
void detruire_affichage(t_affichage **);
