#include <stdio.h>
#include <stdlib.h> // Pour les fonctions malloc et free
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entite.h"
#include "selection_personnages.h"
#include "ressources.h"
#include "constantes.h"
#include "spritesheets.h"

#define MAX_PERSONNAGES 1 // Nombre maximum de personnages
#define N 1
#define VRAI 1


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;





// Fonction pour charger et afficher les images
void afficher_images(SDL_Renderer* renderer, Persos personnages) {

    personnages.texture = recuperer_texture("koba"); // Utiliser la fonction recuperer_texture pour charger la texture

    // Obtenir les dimensions de la texture de l'image
    int largeur, hauteur;
    SDL_QueryTexture(personnages.texture, NULL, NULL, &largeur, &hauteur);
    
    // Calculer les coordonnées pour centrer l'image sur l'écran
    int x = (TAILLE_L - largeur) / 2;
    int y = (TAILLE_H - hauteur) / 2;
    
    // Définir le rectangle de destination pour le rendu
    SDL_Rect destRect = { x, y, largeur, hauteur };
    
    // Dessiner la texture de l'image sur le rendu
    SDL_RenderCopy(renderer, personnages.texture, NULL, &destRect);
    
}

// Fonction pour créer et afficher les personnages
void afficher_persos(SDL_Renderer* renderer) {
    t_entite* perso;
    t_entite* perso2;
    t_entite* perso3;

    // Créer un personnage
    perso = creer_entite_depuis_spritesheet("matt", 35, 33.5, 17, 17, VRAI);
    perso2 = creer_entite_depuis_spritesheet("jack", 50, 33.5, 17, 17, VRAI);
    perso3 = creer_entite_depuis_spritesheet("yohan", 35, 47, 17, 17, VRAI);

    
    // Afficher le personnage
    afficher_entite(renderer, perso);
    afficher_entite(renderer, perso2);
    afficher_entite(renderer, perso3);
}

// Fonction principale pour afficher les personnages
void afficher_personnages(SDL_Renderer* renderer, Persos personnages) {
    // Afficher les images
    afficher_images(renderer, personnages);
    
    // Afficher les personnages
    afficher_persos(renderer);
}

// Fonction pour gérer le clic sur un personnage
void gerer_clic_personnage(SDL_Event event, t_entite* perso, SDL_Rect* rect, int* selectionne) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        SDL_Point clickPoint = { event.button.x, event.button.y };
        if (SDL_PointInRect(&clickPoint, rect)) {
            // Inverser l'état de sélection du personnage
            *selectionne = (*selectionne == 0) ? 1 : 0;
        }
    }
}

// Fonction pour afficher le personnage sélectionné sur le côté de l'écran
void afficher_personnage_selectionne(SDL_Renderer* renderer, t_entite* perso, int selectionne) {
    if (selectionne) {
        // Dessiner le personnage sélectionné sur le côté droit de l'écran
        SDL_Rect destRect = { TAILLE_L - 100, (TAILLE_H - 50) / 2, 100, 50 };
        afficher_entite(renderer, perso);
    }
}
