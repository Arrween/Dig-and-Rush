#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "tour.h"
#include "menu.h"


#define TITRE_FENETRE "Dig & Rush"
#define TAILLE_L 840
#define TAILLE_H 480
#define FPS 60

enum { PAGE_MENU, PAGE_MENU_SERVEUR, PAGE_MENU_PARAMETRES, PAGE_MENU_PERSONNAGES };


int main() {
    SDL_Window * fenetre;
    SDL_Renderer * rend;
    SDL_Event event;



    //INITIALISATION ET DÉCLARATION DES SURFACES

    //BOUTONS
    SDL_Surface *bouton_parametre_sprite;
    SDL_Surface *bouton_volume_on_sprite;
    SDL_Surface *bouton_volume_off_sprite;
    SDL_Surface *bouton_personnages_sprite;
    SDL_Surface *bouton_fullscreen_sprite;
    SDL_Surface *bouton_not_fullscreen_sprite;
    SDL_Surface *bouton_home_sprite;
    SDL_Surface *bouton_play_sprite;
    SDL_Surface *bouton_quit_sprite;

    /******************************************************************************************************************/

    //INITIALISATION ET DÉCLARATION DES TEXTURES 
    SDL_Texture * texture_fond ; // Texture pour l'image de fond
    SDL_Texture *texture_logo; 

    //MENUS
    SDL_Texture *texture_menu_serveur; // Nouvelle texture pour le fond du menu du serveur
    SDL_Texture *texture_menu_parametre; // Nouvelle texture pour le fond du menu des paramètres
    SDL_Texture *texture_menu_personnages;

    /******************************************************************************************************************/

    //INITIALISATION ET DÉCLARATION DES TEXTURES À PARTIR DES SURFACES

    //BOUTONS
    SDL_Texture *texture_bouton_parametre_sprite;
    SDL_Texture *texture_bouton_volume_on_sprite;
    SDL_Texture *texture_bouton_volume_off_sprite;
    SDL_Texture *texture_bouton_personnages_sprite;
    SDL_Texture *texture_bouton_fullscreen; // Charger la texture du bouton plein écran
    SDL_Texture *texture_bouton_not_fullscreen; // Charger la texture du bouton plein écran
    SDL_Texture *texture_bouton_home; // Charger la texture du bouton home
    SDL_Texture *texture_bouton_play_sprite; // Charger la texture du bouton play
    SDL_Texture *texture_bouton_quit_sprite; // Charger la texture du bouton quit


    /******************************************************************************************************************/

    //INITIALISATION ET DÉCLARATION DU RECTANGLE LOGO
    SDL_Rect logo = {TAILLE_L * (0.5 - 0.238), TAILLE_H / 2 - TAILLE_H * 0.42, TAILLE_L * 0.476, TAILLE_H * 0.417};


    //INITIALISATION ET DÉCLARATION DES RECTANGLES BOUTONS
    SDL_Rect parametres_btn = {TAILLE_L * 0.012, TAILLE_H * 0.021, 50, 50}; // Coordonnées et dimensions du bouton "Paramètres"
    SDL_Rect volume_btn_on = {TAILLE_L * 0.095, TAILLE_H * 0.010, 55, 60}; // Coordonnées et dimensions du bouton "Volume"
    SDL_Rect volume_btn_off = {TAILLE_L * 0.095, TAILLE_H * 0.010, 55, 60}; // Coordonnées et dimensions du bouton "Volume Off"
    SDL_Rect personnages_btn = {TAILLE_L * 0.761, TAILLE_H * 0.010, 55, 60}; // Coordonnées et dimensions du bouton "Personnages"
    SDL_Rect fullscreen_btn = {TAILLE_L * 0.892, TAILLE_H * 0.017, 55, 55};
    SDL_Rect not_fullscreen_btn = {TAILLE_L * 0.892, TAILLE_H * 0.017, 55, 55};
    SDL_Rect home_btn = {TAILLE_L * 0.012, TAILLE_H * 0.87, 55, 55};
    SDL_Rect play_btn = {TAILLE_L * 0.67, TAILLE_H * 0.7, 169, 74};
    SDL_Rect quit_btn = {TAILLE_L * 0.120, TAILLE_H * 0.7, 169, 74};


    /*****************************************************************************************************************/




    //INITIALISATION ET DÉCLARATION DES VARIABLES
    int fin = 0;
    int page = PAGE_MENU;
    
    // Ajout d'une variable pour stocker l'état du son (activé ou désactivé)
    int son_actif = 1; // Initialisation à "on"
    int fullscreen = 0; // 0 pour le mode fenêtré, 1 pour le mode plein écran
    int bouton_enfonce = 0; 




    initialiser_sdl();

    // Initialisation de SDL_image pour le support des images PNG
    initialiser_sdl_img();

    // Création de la fenêtre
    fenetre = creation_fenetre();

    // Création du renderer
    rend = creation_renderer(&fenetre);

    //SURFACE

    bouton_parametre_sprite = chargement_surface("ressources/Menu/Boutons/bouton_parametre.png", &rend, &fenetre);
    bouton_volume_on_sprite = chargement_surface("ressources/Menu/Boutons/bouton_volume_on.png", &rend, &fenetre);
    bouton_volume_off_sprite = chargement_surface("ressources/Menu/Boutons/bouton_volume_off.png", &rend, &fenetre);
    bouton_personnages_sprite = chargement_surface("ressources/Menu/Boutons/bouton_personnages.png", &rend, &fenetre);
    bouton_fullscreen_sprite = chargement_surface("ressources/Menu/Boutons/bouton_fullscreen.png", &rend, &fenetre);
    bouton_not_fullscreen_sprite = chargement_surface("ressources/Menu/Boutons/bouton_not_fullscreen.png", &rend, &fenetre);
    bouton_home_sprite = chargement_surface("ressources/Menu/Boutons/bouton_home.png", &rend, &fenetre);
    bouton_play_sprite = chargement_surface("ressources/Menu/Boutons/bouton_play.png", &rend, &fenetre);
    bouton_quit_sprite = chargement_surface("ressources/Menu/Boutons/bouton_quit.png", &rend, &fenetre);

    //TEXTURE À PARTIR D'UNE SURFACE
    texture_bouton_parametre_sprite = chargement_texture_from_surface(rend, bouton_parametre_sprite, fenetre);
    texture_bouton_volume_on_sprite = chargement_texture_from_surface(rend, bouton_volume_on_sprite, fenetre);
    texture_bouton_volume_off_sprite = chargement_texture_from_surface(rend, bouton_volume_off_sprite, fenetre);
    texture_bouton_personnages_sprite = chargement_texture_from_surface(rend, bouton_personnages_sprite, fenetre);
    texture_bouton_fullscreen = chargement_texture_from_surface(rend, bouton_fullscreen_sprite, fenetre);
    texture_bouton_not_fullscreen = chargement_texture_from_surface(rend, bouton_not_fullscreen_sprite, fenetre);
    texture_bouton_home = chargement_texture_from_surface(rend, bouton_home_sprite, fenetre);
    texture_bouton_play_sprite = chargement_texture_from_surface(rend, bouton_play_sprite, fenetre);
    texture_bouton_quit_sprite = chargement_texture_from_surface(rend, bouton_quit_sprite, fenetre);

    //TEXTURE
    // Chargement de l'image PNG pour le fond dans une texture
    texture_fond = chargement_image("ressources/Menu/Background_Menu/Fond_Menu.png", &rend, &fenetre);
    texture_logo = chargement_image("ressources/Menu/Nom_du_Jeu/logo.png", &rend, &fenetre);
    texture_menu_serveur = chargement_image("ressources/Menu/Background_Menu/choix_serveur.png", &rend, &fenetre);
    texture_menu_parametre = chargement_image("ressources/Menu/Background_Menu/menu_parametre.png", &rend, &fenetre);
    texture_menu_personnages = chargement_image("ressources/Menu/Background_Menu/menu_personnages.png", &rend, &fenetre);

    

  
  
  
    while (!fin) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    fin = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (page == PAGE_MENU) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            // Si l'utilisateur clique sur le bouton pour basculer entre le mode plein écran et le mode fenêtré
                            if (event.button.x >= fullscreen_btn.x && event.button.x <= fullscreen_btn.x + fullscreen_btn.w &&
                                event.button.y >= fullscreen_btn.y && event.button.y <= fullscreen_btn.y + fullscreen_btn.h) {
                                fullscreen = !fullscreen; // Inverser le mode plein écran
                                SDL_SetWindowFullscreen(fenetre, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); // Activer ou désactiver le plein écran
                            }

                            else if (event.button.x >= play_btn.x && event.button.x <= play_btn.x + play_btn.w &&
                                event.button.y >= play_btn.y && event.button.y <= play_btn.y + play_btn.h) {
                                printf("Bouton PLAY cliqué.\n");
                                bouton_enfonce = 1; // Marquer le bouton "Play" comme enfoncé
                                page = PAGE_MENU_SERVEUR;
                            } 
                            else if (event.button.x >= quit_btn.x && event.button.x <= quit_btn.x + quit_btn.w &&
                                       event.button.y >= quit_btn.y && event.button.y <= quit_btn.y + quit_btn.h) {
                                printf("Bouton QUIT cliqué.\n");
                                fin = 1;
                            }
                            else if (event.button.x >= volume_btn_on.x && event.button.x <= volume_btn_on.x + volume_btn_on.w &&
                                       event.button.y >= volume_btn_on.y && event.button.y <= volume_btn_on.y + volume_btn_on.h) {
                                printf("Bouton VOLUME cliqué.\n");
                                // Inversion de l'état du son
                                son_actif = !son_actif;
                            }
                            else if (event.button.x >= parametres_btn.x && event.button.x <= parametres_btn.x + parametres_btn.w &&
                                       event.button.y >= parametres_btn.y && event.button.y <= parametres_btn.y + parametres_btn.h) {
                                        printf("Bouton PARAMETRES cliqué.\n");
                                        page = PAGE_MENU_PARAMETRES;
                            }
                            else if (event.button.x >= personnages_btn.x && event.button.x <= personnages_btn.x + personnages_btn.w &&
                                       event.button.y >= personnages_btn.y && event.button.y <= personnages_btn.y + personnages_btn.h) {
                                        printf("Bouton PERSONNAGE cliqué.\n");
                                        page = PAGE_MENU_PERSONNAGES;
                            }
                        }
                    }

                    else if (page == PAGE_MENU_SERVEUR){
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            // Si l'utilisateur clique sur le bouton pour basculer entre le mode plein écran et le mode fenêtré
                            if (event.button.x >= fullscreen_btn.x && event.button.x <= fullscreen_btn.x + fullscreen_btn.w &&
                                event.button.y >= fullscreen_btn.y && event.button.y <= fullscreen_btn.y + fullscreen_btn.h) {
                                fullscreen = !fullscreen; // Inverser le mode plein écran
                                SDL_SetWindowFullscreen(fenetre, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); // Activer ou désactiver le plein écran
                            }
                            else if (event.button.x >= volume_btn_on.x && event.button.x <= volume_btn_on.x + volume_btn_on.w &&
                                        event.button.y >= volume_btn_on.y && event.button.y <= volume_btn_on.y + volume_btn_on.h) {
                                    printf("Bouton VOLUME cliqué.\n");
                                    // Inversion de l'état du son
                                    son_actif = !son_actif;
                                }
                            else if (event.button.x >= parametres_btn.x && event.button.x <= parametres_btn.x + parametres_btn.w &&
                                    event.button.y >= parametres_btn.y && event.button.y <= parametres_btn.y + parametres_btn.h) {
                                        printf("Bouton PARAMETRES cliqué.\n");
                                        page = PAGE_MENU_PARAMETRES;
                                }
                            else if (event.button.x >= home_btn.x && event.button.x <= home_btn.x + home_btn.w &&
                                    event.button.y >= home_btn.y && event.button.y <= home_btn.y + home_btn.h) {
                                        printf("Bouton HOME cliqué.\n");
                                        page = PAGE_MENU;
                                }
                            else if (event.button.x >= personnages_btn.x && event.button.x <= personnages_btn.x + personnages_btn.w &&
                                    event.button.y >= personnages_btn.y && event.button.y <= personnages_btn.y + personnages_btn.h) {
                                        printf("Bouton PERSONNAGE cliqué.\n");
                                        page = PAGE_MENU_PERSONNAGES;
                                }

                        }                        

                    }

                    else if (page == PAGE_MENU_PARAMETRES) {
                        if(event.button.button == SDL_BUTTON_LEFT){
                            // Si l'utilisateur clique sur le bouton pour basculer entre le mode plein écran et le mode fenêtré
                            if (event.button.x >= fullscreen_btn.x && event.button.x <= fullscreen_btn.x + fullscreen_btn.w &&
                                event.button.y >= fullscreen_btn.y && event.button.y <= fullscreen_btn.y + fullscreen_btn.h) {
                                fullscreen = !fullscreen; // Inverser le mode plein écran
                                SDL_SetWindowFullscreen(fenetre, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); // Activer ou désactiver le plein écran
                            }
                            else if (event.button.x >= volume_btn_on.x && event.button.x <= volume_btn_on.x + volume_btn_on.w &&
                                       event.button.y >= volume_btn_on.y && event.button.y <= volume_btn_on.y + volume_btn_on.h) {
                                printf("Bouton VOLUME cliqué.\n");
                                // Inversion de l'état du son
                                son_actif = !son_actif;
                            }

                            else if (event.button.x >= home_btn.x && event.button.x <= home_btn.x + home_btn.w &&
                                    event.button.y >= home_btn.y && event.button.y <= home_btn.y + home_btn.h) {
                                        printf("Bouton HOME cliqué.\n");
                                        page = PAGE_MENU;
                                }
                            else if (event.button.x >= personnages_btn.x && event.button.x <= personnages_btn.x + personnages_btn.w &&
                                    event.button.y >= personnages_btn.y && event.button.y <= personnages_btn.y + personnages_btn.h) {
                                        printf("Bouton PERSONNAGE cliqué.\n");
                                        page = PAGE_MENU_PERSONNAGES;
                                }
                            }
                    }

                    else if (page == PAGE_MENU_PERSONNAGES) {
                        if(event.button.button == SDL_BUTTON_LEFT){
                            // Si l'utilisateur clique sur le bouton pour basculer entre le mode plein écran et le mode fenêtré
                            if (event.button.x >= fullscreen_btn.x && event.button.x <= fullscreen_btn.x + fullscreen_btn.w &&
                                event.button.y >= fullscreen_btn.y && event.button.y <= fullscreen_btn.y + fullscreen_btn.h) {
                                fullscreen = !fullscreen; // Inverser le mode plein écran
                                SDL_SetWindowFullscreen(fenetre, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); // Activer ou désactiver le plein écran
                            }
                            else if (event.button.x >= volume_btn_on.x && event.button.x <= volume_btn_on.x + volume_btn_on.w &&
                                    event.button.y >= volume_btn_on.y && event.button.y <= volume_btn_on.y + volume_btn_on.h) {
                                printf("Bouton VOLUME cliqué.\n");
                                // Inversion de l'état du son
                                son_actif = !son_actif;
                            }
                            else if (event.button.x >= parametres_btn.x && event.button.x <= parametres_btn.x + parametres_btn.w &&
                                        event.button.y >= parametres_btn.y && event.button.y <= parametres_btn.y + parametres_btn.h) {
                                    printf("Bouton PARAMETRES cliqué.\n");
                                    page = PAGE_MENU_PARAMETRES;
                            }
                            else if (event.button.x >= home_btn.x && event.button.x <= home_btn.x + home_btn.w &&
                                    event.button.y >= home_btn.y && event.button.y <= home_btn.y + home_btn.h) {
                                        printf("Bouton HOME cliqué.\n");
                                        page = PAGE_MENU;
                            }

                        }


                    break;
                
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        //Remettre à 0 l'état enfoncé du bouton
                        bouton_enfonce = 0;
                        play_btn.y = TAILLE_H * 0.7;
                    }
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
                    break;
                }
            }
        }

        SDL_RenderClear(rend);
        if (page == PAGE_MENU) {


            SDL_RenderCopy(rend, texture_fond, NULL, NULL);
            SDL_RenderCopy(rend, texture_logo, NULL, &logo);



            if (fullscreen) {

                SDL_RenderCopy(rend, texture_bouton_not_fullscreen, NULL, &not_fullscreen_btn);
            
            } else {

                // Afficher le bouton plein écran
                SDL_RenderCopy(rend, texture_bouton_fullscreen, NULL, &fullscreen_btn);
            }


            SDL_RenderCopy(rend, texture_bouton_parametre_sprite, NULL, &parametres_btn);



            // Affichage du bouton "Volume" en fonction de son état
            if (son_actif) {
                SDL_RenderCopy(rend, texture_bouton_volume_on_sprite, NULL, &volume_btn_on);
            } else {
                // Afficher le bouton "Volume" désactivé si vous avez une texture pour cela
                SDL_RenderCopy(rend, texture_bouton_volume_off_sprite, NULL, &volume_btn_off);
            }

            SDL_RenderCopy(rend, texture_bouton_personnages_sprite, NULL, &personnages_btn);
            
            if(bouton_enfonce) {
                play_btn.y *= 20;
            }
            SDL_RenderCopy(rend, texture_bouton_play_sprite, NULL, &play_btn);

            SDL_RenderCopy(rend, texture_bouton_quit_sprite, NULL, &quit_btn);



        } else if (page == PAGE_MENU_SERVEUR) {
            
            // Afficher le fond du menu serveur
            SDL_RenderCopy(rend, texture_menu_serveur, NULL, NULL);
            SDL_RenderCopy(rend, texture_bouton_parametre_sprite, NULL, &parametres_btn);




            if (fullscreen) {
                
                SDL_RenderCopy(rend, texture_bouton_not_fullscreen, NULL, &not_fullscreen_btn);
            
            } else {

                // Afficher le bouton plein écran
                SDL_RenderCopy(rend, texture_bouton_fullscreen, NULL, &fullscreen_btn);
            }

            // Affichage du bouton "Volume" en fonction de son état
            if (son_actif) {
                SDL_RenderCopy(rend, texture_bouton_volume_on_sprite, NULL, &volume_btn_on);
            } else {
                // Afficher le bouton "Volume" désactivé si vous avez une texture pour cela
                SDL_RenderCopy(rend, texture_bouton_volume_off_sprite, NULL, &volume_btn_off);
            }

            SDL_RenderCopy(rend, texture_bouton_personnages_sprite, NULL, &personnages_btn);
            SDL_RenderCopy(rend, texture_bouton_home, NULL, &home_btn);


        } else if( page == PAGE_MENU_PARAMETRES ) { 
            //Afficher le fond du menu paramètre
            SDL_RenderCopy(rend, texture_menu_parametre, NULL, NULL);
            SDL_RenderCopy(rend, texture_bouton_parametre_sprite, NULL, &parametres_btn);
            SDL_RenderCopy(rend, texture_bouton_home, NULL, &home_btn);


            if (fullscreen) {
                
                SDL_RenderCopy(rend, texture_bouton_not_fullscreen, NULL, &not_fullscreen_btn);
            
            } else {

                // Afficher le bouton plein écran
                SDL_RenderCopy(rend, texture_bouton_fullscreen, NULL, &fullscreen_btn);
            }

            // Affichage du bouton "Volume" en fonction de son état
            if (son_actif) {
                SDL_RenderCopy(rend, texture_bouton_volume_on_sprite, NULL, &volume_btn_on);
            } else {
                // Afficher le bouton "Volume" désactivé si vous avez une texture pour cela
                SDL_RenderCopy(rend, texture_bouton_volume_off_sprite, NULL, &volume_btn_off);
            }

            SDL_RenderCopy(rend, texture_bouton_personnages_sprite, NULL, &personnages_btn);
            SDL_RenderCopy(rend, texture_bouton_home, NULL, &home_btn);

        }

        else if (page == PAGE_MENU_PERSONNAGES) {

            // Afficher le fond du menu personnages

            SDL_RenderCopy(rend, texture_menu_personnages, NULL, NULL);
            SDL_RenderCopy(rend, texture_bouton_parametre_sprite, NULL, &parametres_btn);
            SDL_RenderCopy(rend, texture_bouton_home, NULL, &home_btn);

            if (fullscreen) {

                SDL_RenderCopy(rend, texture_bouton_not_fullscreen, NULL, &not_fullscreen_btn);

            } else {
                // Afficher le bouton plein écran
                SDL_RenderCopy(rend, texture_bouton_fullscreen, NULL, &fullscreen_btn);
            }
            // Affichage du bouton "Volume" en fonction de son état
            if (son_actif) {
                SDL_RenderCopy(rend, texture_bouton_volume_on_sprite, NULL, &volume_btn_on);
            } else {
                // Afficher le bouton "Volume" désactivé si vous avez une texture pour cela
                SDL_RenderCopy(rend, texture_bouton_volume_off_sprite, NULL, &volume_btn_off);
            }

            //SDL_RenderCopy(rend, texture_bouton_personnages_sprite, NULL, &personnages_btn);
            SDL_RenderCopy(rend, texture_bouton_home, NULL, &home_btn);

        }



        SDL_RenderPresent(rend);
        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyTexture(texture_fond);
    SDL_DestroyTexture(texture_logo);
    SDL_DestroyTexture(texture_menu_serveur);
    SDL_DestroyTexture(texture_menu_personnages);
    SDL_DestroyTexture(texture_menu_parametre);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
