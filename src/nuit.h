/**
 *  @file
 *  @brief Implémentation d’un effet de nuit
 *
 *  L’effet consiste à cacher la zone de jeu derrière une texture
 *  remplie de noir à l’exception d’un cercle centré sur le joueur
 *  avec une transition de transparence entre la zone cachée et la
 *  zone visible.
 */

#ifndef NUIT_H
#define NUIT_H

#include <SDL2/SDL.h>

#include "entite.h"
#include "constantes.h"

/** @brief maximum du rayon de la zone visible à la fin de la transition
*          vers le mode jour
*/
#define MAX_RAYON_VISIBLE TAILLE_H
/** @brief valeur multipliée par la largeur du personnage pour donner le rayon
*          minimum de la zone visible en mode nuit
*/
#define FACTEUR_MIN_RAYON_VISIBLE 4
/** @brief caractérise la rapidité de la transition de transparence entre la
*          zone visible et la zone cachée
*/
#define FACTEUR_OBSCURCISSEMENT 6.

/** @brief pas, en pourcentage par frame, de la transition entre le jour et
*          la nuit ou l’inverse
*/
#define PAS_TRANSITION 5

/** @brief objet contenant les variables à modifier pour la transition entre
    *      jour et nuit ainsi que les paramètres nécessaires au calcul de la
    *      transition
    */
typedef struct {
    float alpha;                /**< transparence de la texture de fond du jour */
    float min_alpha;            /**< minimum de `t_nuit::alpha` */
    float max_alpha;            /**< maximum de `t_nuit::alpha` */
    float rayon;                /**< rayon de la zone visible */
    float min_rayon;            /**< maximum de `t_nuit::rayon` */
    float max_rayon;            /**< maximum de `t_nuit::rayon` */
    float volume_musique_jour;  /**< volume de la musique jouée en mode jour */
    float min_musique;          /**< maximum de `t_nuit::volume_musique_jour` */
    float max_musique;          /**< maximum de `t_nuit::volume_musique_jour` */
    int est_active;             /**< indique si l’effet de nuit est actif */
    int est_active_prec;        /**< indique si l’effet de nuit était précédemment actif */
    t_entite * centre;          /**< entité sur laquelle centrer l’effet de nuit */
    SDL_FRect etendue;          /**< zone à laquelle appliquer l’effet de nuit */
    SDL_Texture * texture_ombre;/**< texture à afficher pour créer l’effet de nuit */
    SDL_Texture * texture_jour; /**< texture du fond de jeu en mode jour */
    SDL_Texture * texture_nuit; /**< texture du fond de jeu en mode nuit */
} t_nuit;

t_nuit * creer_nuit(SDL_Renderer*, t_entite* centre, SDL_FRect zone_jeu,
                    SDL_Texture* texture_jour, SDL_Texture* texture_nuit);
void transitionner_nuit(t_nuit*);
void basculer_nuit(t_nuit*);
void detruire_nuit(t_nuit ** nuit);

#endif
