#ifndef ENTITE_H
#define ENTITE_H

#include <SDL2/SDL.h>
#include "ressources.h"


/** \brief Sens de déplacement d’une entité
*  
*/
enum { REPOS_MVT, GAUCHE, DROITE, HAUT, BAS };

typedef struct s_entite t_entite;

typedef struct s_collision {
    t_entite * g; /** gauche */
    t_entite * d; /** droite */
    t_entite * h; /** haut */
    t_entite * b; /** bas */
} t_collision;

typedef struct s_destructible t_destructible;
typedef struct s_bonus t_bonus;
typedef struct s_pnj t_pnj;
typedef struct s_perso t_perso;
/** \brief Objet général pour toute entité affichée à l’écran
*
*/
typedef struct s_entite {
    /** numéro de l’entité */    
    int numero;
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
    /** indique si le rectangle de destination doit être restauré */
    int doit_restaurer_dst;

     /** booléen indiquant si le rect_dst est relatif à la zone de jeu (la tour) ou absolu (relatif à la fenêtre) */
    int est_relatif;
    /** rectangle utilisé pour le calcul des collisions */
    SDL_FRect hitbox;
    /** booléen régissant l’affichage du rectangle de collision */
    int doit_afficher_hitbox;
    /** collisions */
    t_collision collisions; 

    /** sens vers lequel se déplace l’entité */
    int deplacement;
    /** sens de déplacement précédent */
    int deplacement_prec;
    /** sens (gauche ou droite) vers lequel regarde l’entité */
    int sens_regard;
    /** sens du regard précédent */
    int sens_regard_prec;
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
    /** ID de l'animation suivante */
    t_id_anim id_animation_suivante;

    /** vitesse de déplacement en pourcentage de la zone de jeu frame d’affichage */
    float vitesse;

    /** sous-structure affectée si l’entité est destructible */
    t_destructible * destructible;
    /** booléen indiquant si l’entité est un obstacle */
    int est_obstacle;
    /** sous-structure affectée si l’entité est un personnage non-joueur */
    t_pnj * pnj;
    /** sous-structure affectée si l’entité est un personnage joueur */
    t_perso * perso;
    /** sous-structure affectée si l’entité est un bonus ramassable */
    t_bonus * bonus;

} t_entite;

t_entite * creer_entite(const char *, float, float, float, float, int);
t_entite * creer_entite_depuis_texture(SDL_Texture *, float, float, float, float, int);
t_entite * creer_entite_depuis_spritesheet(const char *, float, float, float, float, int);
void detruire_entite(t_entite **);

void afficher_entite(SDL_Renderer*, t_entite*);
extern void changer_pos_rel(t_entite*, float, float);
void changer_hitbox(t_entite *, SDL_FRect*, float, float, float, float, int);
void appliquer_reflexion_hitbox(t_entite*, SDL_FRect*);
SDL_FRect convertir_vers_absolu(SDL_FRect *, SDL_FRect);
void deplacer(t_entite *, long long int);
void animer(t_entite *, long long int compteur_frames);
void definir_animations(t_entite *, int, ...);
void changer_animation(t_entite *, t_id_anim);

#endif
