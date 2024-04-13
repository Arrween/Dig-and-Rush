/**
 * @file main.c
 * @brief Programme principal pour afficher des personnages et gérer leur sélection.
 */

#include <stdio.h>
#include <stdlib.h> // Pour les fonctions malloc et free
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entite.h"
#include "selection_personnages.h"
#include "ressources.h"
#include "constantes.h"
#include "spritesheets.h"

#define VRAI 1


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// Déclaration de la variable globale
const char* personnage_selectionne = NULL;


/**
 * @brief Fonction pour charger et afficher les images.
 * 
 * @param renderer Le renderer SDL où les images doivent être affichées.
 * @param personnages La structure de personnages contenant les textures à afficher.
 */


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


/**
 * @brief Fonction pour créer et afficher les personnages.
 * 
 * @param renderer Le renderer SDL où les personnages doivent être affichés.
 */

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

// Fonction pour sélectionner un personnage
void selectionner_personnage(const char* nom_personnage) {
    // Mettre à jour la variable personnage_selectionne
    personnage_selectionne = nom_personnage;
}

