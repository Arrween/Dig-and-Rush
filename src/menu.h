/**
 * @file menu.h
 * @brief Fichier d'en-tête pour la gestion des menus.
 */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Déclarations des fonctions externes

extern void initialiser_sdl(); // Fonction pour initialiser SDL

extern void initialiser_sdl_img(); // Fonction pour initialiser SDL_image

extern SDL_Window * creation_fenetre(); // Fonction pour créer une fenêtre SDL

extern SDL_Renderer * creation_renderer(SDL_Window ** fenetre); // Fonction pour créer un renderer SDL

extern SDL_Texture * chargement_image(char * chemin, SDL_Renderer ** rend, SDL_Window ** fenetre); // Fonction pour charger une image SDL_Texture

extern SDL_Surface * chargement_surface(char * chemin, SDL_Renderer ** rend, SDL_Window ** fenetre);

extern SDL_Texture * chargement_texture_from_surface(SDL_Renderer * renderer, SDL_Surface * surface, SDL_Window * window);


extern void initialiser_sdl_ttf(); // Fonction pour initialiser SDL_ttf

extern TTF_Font * chargement_font(char * chemin, int taille, SDL_Renderer ** rend, SDL_Window ** fenetre); // Fonction pour charger une police TTF_Font

// Données des boutons

enum { PAGE_MENU, PAGE_MENU_PERSONNAGES, PAGE_MENU_PARAMETRES };


typedef struct s_etat t_etat;

/**
 * @brief Structure représentant un bouton du menu.
 */
typedef struct { 
    SDL_Texture * texture; /**< Texture du bouton. */
    SDL_Rect rect; /**< Rectangle délimitant la zone du bouton. */
    void (*action) (t_etat * etat);/**< Fonction à exécuter lors du clic sur le bouton. */
    char nom[100];/**< Nom du bouton. */
} t_bouton;

/**
 * @brief Structure représentant l'état du menu.
 */
struct s_etat {
    int doit_quitter;/**< Indique si le menu doit être quitté. */
    int est_fullscreen;/**< Indique si le menu est en mode plein écran. */
    int i_menu;/**< Indice de la page de menu actuelle. */
    t_bouton ** boutons;/**< Tableau de pointeurs vers les boutons du menu. */
    SDL_Window * fenetre;/**< Pointeur vers la fenêtre SDL du menu. */
};

void action_volume(t_etat *);
void action_option(t_etat * );
void action_fullscreen(t_etat *);
void action_home(t_etat *);
void action_quitter(t_etat *);
void action_jouer(t_etat *);
void action_personnages(t_etat *);
void action_parametres(t_etat *);
void action_continue(t_etat * );
void action_retour(t_etat *);
void action_nulle(void);

#endif // MENU_H
