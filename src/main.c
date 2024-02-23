#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "tour.h"
#include "menu.h"

#define TITRE_FENETRE "Dig & Rush"
#define TAILLE_L 640
#define TAILLE_H 480
#define FPS 60

int main() {
    SDL_Window * fenetre;
    SDL_Renderer * rend;
    SDL_Event event;
    SDL_Texture * texture_fond ; // Texture pour l'image de fond
    
    // Définition des boutons
    SDL_Rect jouer_btn = {TAILLE_L / 2 - 100, TAILLE_H / 2 - 60, 200, 50};
    SDL_Rect quitter_btn = {TAILLE_L / 2 - 100, TAILLE_H / 2, 200, 50};
    
    int fin = 0;


    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialisation de SDL_image pour le support des images PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur IMG_Init : %s\n", IMG_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création de la fenêtre
    if (!(fenetre = SDL_CreateWindow(TITRE_FENETRE,
				    SDL_WINDOWPOS_CENTERED,
				    SDL_WINDOWPOS_CENTERED,
				    TAILLE_L,
				    TAILLE_H,
				    SDL_WINDOW_FULLSCREEN))) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Création du renderer
    if (!(rend = SDL_CreateRenderer(fenetre, -1, 0))) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Chargement de l'image PNG dans une texture
    if (! (texture_fond = IMG_LoadTexture(rend,"ressources/Menu/Background_Menu/Fond_Menu.png")) ){
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Initialisation de SDL_ttf pour le rendu du texte
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Charger une police
    TTF_Font* font = TTF_OpenFont("ressources/Menu/Police/font1.ttf", 50);
    if (font == NULL) {
        fprintf(stderr, "Erreur TTF_OpenFont : %s\n", TTF_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Définir la couleur du texte
    SDL_Color texte_couleur = {255, 255, 255, 255}; // Blanc
    SDL_Color texte_couleur_boutons = {0, 0, 0, 0}; // Noir

    // Rendu du titre du jeu
    SDL_Surface* surface_titre = TTF_RenderText_Solid(font, "Dig & Rush", texte_couleur);
    SDL_Texture* texture_titre = SDL_CreateTextureFromSurface(rend, surface_titre);
    SDL_Rect titre_rect = {TAILLE_L / 2 - surface_titre->w / 2, 50, surface_titre->w, surface_titre->h};

    // Créer une texture pour le texte du bouton Jouer
    SDL_Surface* surface_jouer = TTF_RenderText_Solid(font, "Jouer", texte_couleur_boutons);
    SDL_Texture* texture_jouer = SDL_CreateTextureFromSurface(rend, surface_jouer);
    SDL_Rect jouer_txte_rect = {jouer_btn.x + (jouer_btn.w - surface_jouer->w) / 2, jouer_btn.y + (jouer_btn.h - surface_jouer->h) / 2, surface_jouer->w, surface_jouer->h};

    // Créer une texture pour le texte du bouton Quitter
    SDL_Surface* surface_quitter = TTF_RenderText_Solid(font, "Quitter", texte_couleur_boutons);
    SDL_Texture* texture_quitter = SDL_CreateTextureFromSurface(rend, surface_quitter);
    SDL_Rect quitter_texte_rect = {quitter_btn.x + (quitter_btn.w - surface_quitter->w) / 2, quitter_btn.y + (quitter_btn.h - surface_quitter->h) / 2, surface_quitter->w, surface_quitter->h};

    // Libérer les surfaces après la création des textures
    SDL_FreeSurface(surface_jouer);
    SDL_FreeSurface(surface_quitter);

    while (!fin) {

        // Nettoyer le renderer
        SDL_RenderClear(rend);

        // Dessiner la texture de fond
        SDL_RenderCopy(rend, texture_fond, NULL, NULL);

        // Dessiner les boutons
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        SDL_RenderFillRect(rend, &jouer_btn);
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); 
        SDL_RenderFillRect(rend, &quitter_btn);

        // Dessiner le texte sur les boutons
        SDL_RenderCopy(rend, texture_jouer, NULL, &jouer_txte_rect);
        SDL_RenderCopy(rend, texture_quitter, NULL, &quitter_texte_rect);

        // Dessiner le titre du jeu
        SDL_RenderCopy(rend, texture_titre, NULL, &titre_rect);
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT:
                fin = 1;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                    case SDL_SCANCODE_Q:
                        fin = 1;
                        break;
                    default:
                        break;
                }
	        }
	    }

        // Mettre à jour l'écran
        SDL_RenderPresent(rend);
        SDL_Delay(1000/FPS);

    }
    
    // Nettoyage
    SDL_DestroyTexture(texture_fond);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    SDL_DestroyTexture(texture_titre);
    SDL_FreeSurface(surface_titre);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_SUCCESS);
}