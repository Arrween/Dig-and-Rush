#include <SDL2/SDL.h>

enum { REPS, GAUCHE, DROITE, HAUT, BAS };
typedef enum { REPOS, DEPL_G, DEPL_D, CREUSER } t_animation;

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
    SDL_Rect hitbox;
    int doit_afficher_hitbox;

    int a_collision;
    int a_animations;
    int deplacement;
    int sens_regard;
    int x_sprite;
    int y_sprite;

    t_animation * animations;
    int n_animations;
    t_animation animation_courante;

    void (*afficher) (SDL_Renderer *, struct s_entite *);
    void (*changer_rect_src) (struct s_entite *, int, int, int, int);
    void (*changer_rect_dst) (struct s_entite *, int, int, int, int);
    void (*changer_sprite) (struct s_entite *, int, int);
    void (*changer_pos) (struct s_entite *, int, int);
    void (*changer_dims) (struct s_entite *, int, int);
    void (*changer_pos_rel) (struct s_entite *, int, int);
    void (*creuser) (struct s_entite *);
} t_entite;

t_entite * creer_entite(const char *, int, int, int, int, int);
t_entite * creer_entite_depuis_texture(SDL_Texture *, int, int, int, int, int);
t_entite * creer_entite_depuis_spritesheet(const char *, int, int, int, int, int);
void detruire_entite(t_entite **);

void changer_hitbox(t_entite *, int, int, int, int);
SDL_Rect convertir_vers_absolu(SDL_Rect *, int, int, int, int);
void deplacer(t_entite *);
void animer(t_entite *);
void definir_animations(t_entite *, int, ...);
