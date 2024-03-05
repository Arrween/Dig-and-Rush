#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// #include "tour.h"
// #include "menu.h"

#define TITRE_FENETRE "Dig & Rush"
#define TAILLE_L 1000
#define TAILLE_H 1000
#define FPS 60

int main() {
    SDL_Window * fenetre;
    SDL_Renderer * rend;
    SDL_Event event;
    //SDL_Texture * textureFond ; // Texture pour l'image de fond
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
				    0))) {
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
    
    /* //Affichage d'une image d'un personnage
    SDL_Surface *personnage = NULL;
    SDL_Rect positionPerso;
    
    
    positionPerso.x = 320;
    positionPerso.y = 240;
    personnage = IMG_Load("git/ressources/Personnages/test_personnage.png");
    
    SDL_BlitSurface(personnage, NULL, fenetre, &positionPerso);
    
    SDL_FreeSurface(personnage);
    */
    
    /*
    // Chargement de l'image PNG dans une texture
    if (! (textureFond = IMG_LoadTexture(rend,"../update.png")) ){
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    */
    
//affichage personnage
SDL_Rect positionPersonnage;
    

  // Charger la sprite sheet
    SDL_Surface *spriteSheet = IMG_Load("../../ressources/Personnages/yohanPerso.png");
    if (!spriteSheet) {
        fprintf(stderr, "Erreur lors du chargement de la sprite sheet : %s\n", IMG_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Convertir la surface en texture
    SDL_Texture *texturePersonnage = SDL_CreateTextureFromSurface(rend, spriteSheet);

    if (!texturePersonnage) {
        fprintf(stderr, "Erreur lors de la création de la texture du personnage : %s\n", SDL_GetError());
        SDL_FreeSurface(spriteSheet);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(fenetre);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }


 int frameWidth = 64;  // Largeur d'une frame dans la sprite sheet
 int frameHeight = 64; // Hauteur d'une frame dans la sprite sheet
 int frameCount = 10;   // Nombre total de frames dans la sprite sheet


// Centrer le personnage dans la fenêtre
positionPersonnage.x = (TAILLE_L - frameWidth) / 2;
positionPersonnage.y = (TAILLE_H - frameHeight) / 2;
positionPersonnage.w = frameWidth;
positionPersonnage.h = frameHeight;

int currentFrame = 0;
    int direction = 0; // 0: vers le bas, 1: vers la gauche, 2: vers la droite, 3: vers le haut








    while (!fin) {
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

                    case SDL_SCANCODE_LEFT:
                        direction = 1;
                        positionPersonnage.x -= 15;
                        // Changer de frame pour l'animation
    			currentFrame = (currentFrame + 1) % frameCount;
                        break;

                    case SDL_SCANCODE_RIGHT:
                        direction = 2;
                        positionPersonnage.x += 15;
                        // Changer de frame pour l'animation
    			currentFrame = (currentFrame + 1) % frameCount;
                        break;

                    case SDL_SCANCODE_UP:
                        direction = 3;
                        positionPersonnage.y -= 15;
                        break;

                    case SDL_SCANCODE_DOWN:
                        direction = 0;
                        positionPersonnage.y += 15;
                        break;

                    default:
                        break;
                }
                break;
        }
    }

       // Nettoyer le renderer
	SDL_RenderClear(rend);
	
    // Afficher la frame actuelle
    SDL_Rect srcRect = { direction * frameWidth, currentFrame * frameHeight, frameWidth, frameHeight };
    SDL_RenderCopy(rend, texturePersonnage, &srcRect, &positionPersonnage);

    // Mettre à jour l'écran
    SDL_RenderPresent(rend);
    SDL_Delay(60 / FPS);

    
}

    // Nettoyage
    //SDL_DestroyTexture(textureFond);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
