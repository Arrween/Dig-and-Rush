/** @file
 *  @brief Implémentation de la sélection des personnages (source)
 */

#ifndef PERSOS_H
#define PERSOS_H

#include <SDL2/SDL.h>

#include "ressources.h"

extern char personnage_selectionne[TAILLE_MAX_ID]; // Déclaration externe de la variable globale


// Structure pour représenter un personnage avec sa texture et sa position
typedef struct {
    SDL_Texture* texture; ///< La texture du personnage
    SDL_Rect position; ///< La position du personnage
} Persos;


/**
 * @brief Fonction pour afficher les images des personnages.
 * 
 * @param renderer Le renderer SDL où les images doivent être affichées.
 * @param personnages La structure de personnages contenant les textures à afficher.
 */

// Fonction pour afficher les personnages sur le renderer SDL
void afficher_images(SDL_Renderer* renderer, Persos personnages);


/**
 * @brief Fonction pour créer et afficher les personnages.
 * 
 * @param renderer Le renderer SDL où les personnages doivent être affichés.
 */

// Fonction pour créer et afficher les personnages
void afficher_persos(SDL_Renderer* renderer);



/**
 * @brief Fonction principale pour afficher les personnages.
 * 
 * Cette fonction combine l'affichage des images et des personnages.
 * 
 * @param renderer Le renderer SDL où les personnages doivent être affichés.
 * @param personnages La structure de personnages contenant les textures à afficher.
 */

// Fonction principale pour afficher les personnages
void afficher_personnages(SDL_Renderer* renderer, Persos personnages);


/**
 * @brief Fonction pour sélectionner un personnage.
 * 
 * Cette fonction met à jour la variable globale personnage_selectionne avec le nom du personnage sélectionné.
 * 
 * @param nom_personnage Le nom du personnage à sélectionner.
 */

void selectionner_personnage(const char* nom_personnage);

#endif /* PERSOS_H */
