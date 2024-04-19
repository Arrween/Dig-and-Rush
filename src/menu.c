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

void action_parametres(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PARAMETRES;
    jouer_audio(CANAL_BTN_MENU, "confirmation", 0);

      
}

void action_option(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PARAMETRES;
    etat->boutons[4]->texture = recuperer_texture("options"); 
    jouer_audio(CANAL_BTN_MENU, "confirmation", 0);
 
}

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

void action_personnages(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PERSONNAGES;
    jouer_audio(CANAL_BTN_MENU, "confirmation", 0);

}

void action_fullscreen(t_etat * etat) {
    if (etat->boutons[0]->texture == recuperer_texture("bouton_fullscreen_off"))
        etat->boutons[0]->texture = recuperer_texture("bouton_fullscreen_on");
    else
        etat->boutons[0]->texture = recuperer_texture("bouton_fullscreen_off");
    etat->est_fullscreen = !etat->est_fullscreen;
    SDL_SetWindowFullscreen(etat->fenetre, etat->est_fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void action_home(t_etat * etat) {
    etat->i_menu = PAGE_MENU;
    jouer_audio(CANAL_BTN_MENU, "confirmation", 0);

}

void action_jouer(t_etat * etat) {
    etat->i_menu = PAGE_MENU_PERSONNAGES;
    jouer_audio(CANAL_BTN_MENU, "confirmation", 0);
}

void action_retour(t_etat *etat) {
    if (etat->i_menu > 0) {
        etat->i_menu--; // Décrémenter l'index du menu pour revenir en arrière
    }
    jouer_audio(CANAL_BTN_MENU, "confirmation", 0);

}

void action_quitter(t_etat * etat) {
    etat->doit_quitter = VRAI;
    jouer_audio(CANAL_BTN_MENU, "confirmation", 0);

}

void action_nulle(void) {
}
