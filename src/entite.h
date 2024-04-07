#ifndef ENTITE_H
#define ENTITE_H

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
    SDL_FRect * rect_dst;
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
    SDL_FRect hitbox;
    /** booléen régissant l’affichage du rectangle de collision */
    int doit_afficher_hitbox;

     /** booléen indiquant si l’entité est en collision avec une autre à sa gauche */
    int a_collision_g;
     /** booléen indiquant si l’entité est en collision avec une autre à sa droite */
    int a_collision_d;
     /** booléen indiquant si l’entité est en collision avec une autre au-dessus */
    int a_collision_h;
     /** booléen indiquant si l’entité est en collision avec une autre au-dessous */
    int a_collision_b;
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
} t_entite;

t_entite * creer_entite(const char *, float, float, float, float, int);
t_entite * creer_entite_depuis_texture(SDL_Texture *, float, float, float, float, int);
t_entite * creer_entite_depuis_spritesheet(const char *, float, float, float, float, int);
void detruire_entite(t_entite **);

void afficher_entite(SDL_Renderer*, t_entite*);
extern void changer_pos_rel(t_entite*, float, float);
void changer_hitbox(t_entite *, int, int, int, int);
SDL_FRect convertir_vers_absolu(SDL_FRect *, SDL_FRect);
void deplacer(t_entite *);
void animer(t_entite *, long long int compteur_frames);
void definir_animations(t_entite *, int, ...);
void changer_animation(t_entite *, t_id_anim);

#endif
