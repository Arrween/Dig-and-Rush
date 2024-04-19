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
#include "texte.h"

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

// Fonction qui initialise la SDL_IMG
void initialiser_sdl_ttf(){
    if (TTF_Init() < 0) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void initialiser_sdl_mixer(){
    if (Mix_OpenAudio(FREQ_AUDIO, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, TAILLE_TAMPON_AUDIO) < 0) {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
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
        Mix_VolumeMusic(VOLUME_MUSIQUE_INI);
        Mix_Volume(-1, VOLUME_SONS_INI);
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

void action_perso_commun(t_etat * etat, char * nom_perso) {
    printf("Personnage sélectionné : %s\n", nom_perso);

    char id_son[100] = "accroche_";
    strcat(id_son, nom_perso);
    jouer_audio(CANAL_ACCROCHE, id_son, 0);

    strcpy(etat->perso_selectionne, nom_perso);

    etat->tex_perso_selectionne = recuperer_texture(nom_perso);
}

void action_perso_reini(t_etat * etat) {
    strcpy(etat->perso_selectionne, "");
    etat->tex_perso_selectionne = NULL;
    etat->tex_barre_vie = NULL;
    etat->tex_barre_energie = NULL;
    changer_texte(etat->texte_perso_selectionne, "");
}

void action_perso_ania(t_etat * etat) {
    if (strcmp(etat->perso_selectionne, "ania") != 0) {
        action_perso_commun(etat, "ania");
        changer_couleur_texte(etat->texte_perso_selectionne, 255, 0, 0, 255);
        changer_texte(etat->texte_perso_selectionne, "Arrween");
        etat->tex_barre_vie = recuperer_texture("barre_de_vie_pleine");
        etat->tex_barre_energie = recuperer_texture("energie_pleine");
    }
    else
        action_perso_reini(etat);
}
void action_perso_jack(t_etat * etat) {
    if (strcmp(etat->perso_selectionne, "jack") != 0) {
        action_perso_commun(etat, "jack");
        changer_couleur_texte(etat->texte_perso_selectionne, 0, 0, 255, 255);
        changer_texte(etat->texte_perso_selectionne, "Stenfresh");
        etat->tex_barre_vie = recuperer_texture("barre_de_vie_semi");
        etat->tex_barre_energie = recuperer_texture("energie_pleine");
    }
    else
        action_perso_reini(etat);
}
void action_perso_matt(t_etat * etat) {
    if (strcmp(etat->perso_selectionne, "matt") != 0) {
        action_perso_commun(etat, "matt");
        changer_couleur_texte(etat->texte_perso_selectionne, 0, 255, 0, 255);
        changer_texte(etat->texte_perso_selectionne, "Idlusen");
        etat->tex_barre_vie = recuperer_texture("barre_de_vie_pleine");
        etat->tex_barre_energie = recuperer_texture("energie_semi");
    }
    else
        action_perso_reini(etat);
}
void action_perso_yohan(t_etat * etat) {
    if (strcmp(etat->perso_selectionne, "yohan") != 0) {
        action_perso_commun(etat, "yohan");
        changer_couleur_texte(etat->texte_perso_selectionne, 255, 165, 0, 255);
        changer_texte(etat->texte_perso_selectionne, "Aohyn");
        etat->tex_barre_vie = recuperer_texture("barre_de_vie_pleine");
        etat->tex_barre_energie = recuperer_texture("energie_pleine");
    }
    else
        action_perso_reini(etat);
}
