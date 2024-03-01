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
