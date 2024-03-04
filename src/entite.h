#include <SDL2/SDL.h>

typedef struct s_affichage {
    SDL_Texture * texture;
    SDL_Rect * rect_src;
    SDL_Rect * rect_dst;
} t_affichage;

t_affichage* creer_affichage(SDL_Texture *);
void detruire_affichage(t_affichage **);

typedef struct s_entite {
    t_affichage * affichage;
    int est_relatif;
    void (*afficher) (SDL_Renderer *, struct s_entite *);
    void (*changer_rect_src) (struct s_entite *, int, int, int, int);
    void (*changer_rect_dst) (struct s_entite *, int, int, int, int);
    void (*changer_sprite) (struct s_entite *, int, int);
    void (*changer_pos) (struct s_entite *, int, int);
    void (*changer_dims) (struct s_entite *, int, int);
    void (*changer_pos_delta) (struct s_entite *, int, int);
} t_entite;

t_entite * creer_entite(const char *, int, int, int, int, int);
t_entite * creer_entite_depuis_texture(SDL_Texture *, int, int, int, int, int);
t_entite * creer_entite_depuis_spritesheet(const char *, int, int, int, int, int);
void detruire_entite(t_entite **);
