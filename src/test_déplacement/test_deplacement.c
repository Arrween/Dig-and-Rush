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
SDL_Surface *personnage = NULL;
SDL_Texture *texturePersonnage = NULL;
SDL_Rect positionPersonnage;
    

// Charger l'image du personnage
personnage = IMG_Load("../../ressources/Personnages/test_personnage.png");   
if (!personnage) {
    fprintf(stderr, "Erreur lors du chargement de l'image du personnage : %s\n", IMG_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_FAILURE);
}


// Convertir la surface en texture
texturePersonnage = SDL_CreateTextureFromSurface(rend, personnage);

if (!texturePersonnage) {
    fprintf(stderr, "Erreur lors de la création de la texture du personnage : %s\n", SDL_GetError());
    SDL_FreeSurface(personnage);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_FAILURE);
}


// Définir la taille du personnage (taille image)
int nouvelleLargeur = 150;  // ajustez selon vos besoins
int nouvelleHauteur = 150; // ajustez selon vos besoins

// Centrer le personnage dans la fenêtre
int personnageWidth, personnageHeight;
SDL_QueryTexture(texturePersonnage, NULL, NULL, &personnageWidth, &personnageHeight);
positionPersonnage.x = (TAILLE_L - nouvelleLargeur) / 2;
positionPersonnage.y = (TAILLE_H - nouvelleHauteur) / 2;
positionPersonnage.w = nouvelleLargeur;
positionPersonnage.h = nouvelleHauteur;









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
                            positionPersonnage.x -= 15;  //modifier pour la vitesse de déplacement
                            break;

                        case SDL_SCANCODE_RIGHT:
                            positionPersonnage.x += 15;  //modifier pour la vitesse de déplacement
                            break;

                        case SDL_SCANCODE_UP:
                            positionPersonnage.y -= 15;  //modifier pour la vitesse de déplacement
                            break;

                        case SDL_SCANCODE_DOWN:
                            positionPersonnage.y += 15;  //modifier pour la vitesse de déplacement
                            break;
                            
			default:
			    break;
		    }
	    }
	}

       // Nettoyer le renderer
	SDL_RenderClear(rend);
	
	// Dessiner la texture de fond
	// SDL_RenderCopy(rend, textureFond, NULL, NULL);
	
	// Affiche le personnage AVEC POSITION MAIS NE MARCHE PAS
	SDL_RenderCopy(rend, texturePersonnage, NULL, &positionPersonnage);
	
	// Affiche le personnage SANS POSITION
	//SDL_RenderCopy(rend, texturePersonnage, NULL, NULL);


	// Mettre à jour l'écran
	SDL_RenderPresent(rend);
	SDL_Delay(1000/FPS);
    }
    // Nettoyage
    //SDL_DestroyTexture(textureFond);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
