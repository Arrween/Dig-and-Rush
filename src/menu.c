/**
 * @file menu.c
 * @brief Progamme pour la gestion des menus.
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "menu.h"
#include "constantes.h"
#include "ressources.h"

// Fonction qui initialise la SDL
void initialiser_sdl(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

// Fonction qui initialise la SDL_IMG
void initialiser_sdl_img(){
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

// Fonction qui crée la fenêtre du jeu
SDL_Window * creation_fenetre(){
    SDL_Window * fenetre ;
    if (!(fenetre = SDL_CreateWindow(TITRE_FENETRE,
				    SDL_WINDOWPOS_CENTERED,
				    SDL_WINDOWPOS_CENTERED,
				    TAILLE_L,
				    TAILLE_H,
				    SDL_WINDOW_RESIZABLE))) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return (fenetre);
}

// Fonction qui crée le renderer
SDL_Renderer * creation_renderer(SDL_Window ** fenetre){
    SDL_Renderer * rend ;
    if (!(rend = SDL_CreateRenderer(*fenetre, -1, 0))) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*fenetre);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    return rend;
}

//Fonction qui charge l'image de fond
SDL_Texture * chargement_image(char * chemin, SDL_Renderer ** rend, SDL_Window ** fenetre){
    SDL_Texture * texture_fond ;
    if (!(texture_fond = IMG_LoadTexture(*rend, chemin)) ){
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(*rend);
        SDL_DestroyWindow(*fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return texture_fond ;
}

// Fonction qui charge une surface SDL à partir d'un fichier image
SDL_Surface * chargement_surface(char * chemin, SDL_Renderer ** rend, SDL_Window ** fenetre) {
    SDL_Surface * surface;
    if (!(surface = IMG_Load(chemin))) {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(*rend);
        SDL_DestroyWindow(*fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return surface;
}

// Fonction pour convertir une surface en texture
SDL_Texture * chargement_texture_from_surface(SDL_Renderer * renderer, SDL_Surface * surface, SDL_Window * window) {
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Erreur lors de la création de la texture : %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(surface); // Libérer la surface après la création de la texture
    return texture;
}
// Fonction qui initialise la SDL_ttf
void initialiser_sdl_ttf() {
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

// Fonction qui charge la police à partir d'un certain chemin
TTF_Font * chargement_font(char * chemin, int taille, SDL_Renderer ** rend, SDL_Window ** fenetre) {
    TTF_Font * font = TTF_OpenFont(chemin, taille);
        if (font == NULL) {
            fprintf(stderr, "Erreur TTF_OpenFont : %s\n", TTF_GetError());
            SDL_DestroyWindow(*fenetre);
            SDL_DestroyRenderer(*rend);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
    return font ;
}


/**
 * @brief Change le menu vers la page des paramètres.
 *
 * @param etat État du menu.
 */
void action_parametres(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PARAMETRES;
    jouer_audio(0, "confirmation", 0);
}


void action_option(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PARAMETRES;
    etat->boutons[4]->texture = recuperer_texture("options"); 
    jouer_audio(0, "confirmation", 0);
 
}

/**
 * @brief Active/désactive le volume du jeu.
 *
 * @param etat État du menu.
 */
void action_volume(t_etat * etat) {
    if (etat->boutons[1]->texture == recuperer_texture("bouton_volume_on")) {
        etat->boutons[1]->texture = recuperer_texture("bouton_volume_off");
        Mix_VolumeMusic(0);
        Mix_Volume(-1, 0);
    }
    else {
        etat->boutons[1]->texture = recuperer_texture("bouton_volume_on");
        Mix_VolumeMusic(MIX_MAX_VOLUME * FACTEUR_VOLUME_MUSIQUE_INI);
        Mix_Volume(-1, MIX_MAX_VOLUME * FACTEUR_VOLUME_SONS_INI);
    }
}

/**
 * @brief Change le menu vers la page des personnages.
 *
 * @param etat État du menu.
 */
void action_personnages(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PERSONNAGES;
    jouer_audio(0, "confirmation", 0);

}

/**
 * @brief Active/désactive le mode plein écran.
 *
 * @param etat État du menu.
 */
void action_fullscreen(t_etat * etat) {
    if (etat->boutons[0]->texture == recuperer_texture("bouton_fullscreen_off"))
        etat->boutons[0]->texture = recuperer_texture("bouton_fullscreen_on");
    else
        etat->boutons[0]->texture = recuperer_texture("bouton_fullscreen_off");
    etat->est_fullscreen = !etat->est_fullscreen;
    SDL_SetWindowFullscreen(etat->fenetre, etat->est_fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

/**
 * @brief Retourne au menu principal.
 *
 * @param etat État du menu.
 */
void action_home(t_etat * etat) {
    etat->i_menu = PAGE_MENU;
    jouer_audio(0, "confirmation", 0);

}
void action_jouer(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PERSONNAGES;
    jouer_audio(0, "confirmation", 0);
}

/**
 * @brief Revient en arrière dans le menu.
 *
 * @param etat État du menu.
 */
void action_retour(t_etat *etat) {
    if (etat->i_menu > 0) {
        etat->i_menu--; // Décrémenter l'index du menu pour revenir en arrière
    }
    jouer_audio(0, "confirmation", 0);

}

/**
 * @brief Quitte le jeu.
 *
 * @param etat État du menu.
 */
void action_quitter(t_etat * etat) {
    etat->doit_quitter = VRAI;
    jouer_audio(0, "confirmation", 0);

}

void action_nulle(void) {
}
