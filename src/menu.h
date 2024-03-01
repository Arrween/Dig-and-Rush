#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


void initialiser_sdl();
void initialiser_sdl_img();
SDL_Window * creation_fenetre();
SDL_Renderer * creation_renderer(SDL_Window ** fenetre);
SDL_Texture * chargement_image(char * chemin, SDL_Renderer ** rend, SDL_Window ** fenetre);
void initialiser_sdl_ttf();
TTF_Font * chargement_font(char * chemin, int taille, SDL_Renderer ** rend, SDL_Window ** fenetre);