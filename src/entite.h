#include <SDL2/SDL.h>
#include "ressources.h"

/** \brief Sens de déplacement d’une entité
*  
*/
enum { REPOS_MVT, GAUCHE, DROITE, HAUT, BAS };

/** \brief Objet général pour toute entité affichée à l’écran
*
*/
typedef struct s_entite {
    /** type de l’entité (bloc de terre, personnage, ennemi etc.) */
    char type[100];
     /** pixels à afficher */
    SDL_Texture * texture;
    /** sous-rectangle à afficher de la texture */
    SDL_Rect * rect_src;
    /** rectangle où afficher l’entité */
    SDL_Rect * rect_dst;
    /** stockage du décalage horizontal de la frame d’animation à appliquer au rectangle d’affichage, pour gérer des frames d’animations de tailles différentes sans avoir à définir manuellement des rectangles source adaptés à chacune (utilisé si l’entité est chargée d’une spritesheet) */
    int dec_x_dst_prec;
    /** voir dec_x_dst_prec */
    int dec_y_dst_prec;
    /** stockage de la largeur de la frame d’animation, pour pouvoir mettre à la même échelle des frames d’animation de tailles différente (utilisé si l’entité est chargée d’une spritesheet)  */
    int w_src_prec;
    /** voir w_src_prec */
    int h_src_prec; 
    int doit_restaurer_dst;

     /** booléen indiquant si le rect_dst est relatif à la zone de jeu (la tour) ou absolu (relatif à la fenêtre) */
    int est_relatif;
    /** rectangle utilisé pour le calcul des collisions */
    SDL_Rect hitbox;
    /** booléen régissant l’affichage du rectangle de collision */
    int doit_afficher_hitbox;

     /** booléen indiquant si l’entité est en collision avec une autre */
    int a_collision;
    /** sens vers lequel se déplace l’entité */
    int deplacement;
    /** sens (gauche ou droite) vers lequel regarde l’entité */
    int sens_regard;
    /** position horizontale dans la grille de la spritesheet, si l’entité est chargée d’une spritesheet */ 
    int x_sprite;
    /** voir x_sprite */
    int y_sprite;

     /** tableau des animations de l’entité (utilisé si l’entité est chargée d’une spritesheet) */
    t_animation ** animations;
    /** taille du tableau `animations` */
    int n_animations;
    /** position courante dans le tableau `animations` */
    t_animation * animation_courante;

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
void animer(t_entite *, long long int compteur_frames);
void definir_animations(t_entite *, int, ...);
void changer_animation(t_entite *, t_id_anim);
