#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "constantes.h"
#include "tour.h"
#include "menu.h"
#include "ressources.h"

int main() {
    SDL_Window * fenetre;
    SDL_Renderer * rend;
    SDL_Event event;


    initialiser_sdl();

    // Initialisation de SDL_image pour le support des images PNG
    initialiser_sdl_img();

    // Création de la fenêtre
    fenetre = creation_fenetre();

    // Création du renderer
    rend = creation_renderer(&fenetre);
<<<<<<< HEAD

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
=======
    SDL_RenderSetLogicalSize(rend, TAILLE_L, TAILLE_H);

    init_ressources(rend);
        
    t_son * son_essai = recuperer_son("essai");
    SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(NULL, 0, &(son_essai->spec), NULL, 0);
    uint8_t * buffer_essai_mix =  SDL_calloc(1, son_essai->length);
    SDL_MixAudioFormat(buffer_essai_mix, son_essai->buffer, son_essai->spec.format, son_essai->length, SDL_MIX_MAXVOLUME/20);
    SDL_PauseAudioDevice(audio_device, 0);

    t_bouton btn_titre = { recuperer_texture("menu_titre"),
            {TAILLE_L * (0.5 - 0.238),
                TAILLE_H * (0.5 - 0.42),
                TAILLE_L * 0.476,
                TAILLE_H * 0.417},
            action_nulle,
            "TITRE"
        };
    t_bouton btn_parametres = { recuperer_texture("bouton_paramètres"),
            {TAILLE_L * 0.012,
                TAILLE_H * 0.021,
                TAILLE_L * 0.059,
                TAILLE_H * 0.104},
            action_parametres,
            "PARAMÈTRES"
        };
    t_bouton btn_volume = { recuperer_texture("bouton_volume_off"),
            {TAILLE_L * 0.095,
                TAILLE_H * 0.010,
                TAILLE_L * 0.065,
                TAILLE_H * 0.125},
            action_volume,
            "VOLUME"
        };
    t_bouton btn_personnages = { recuperer_texture("bouton_personnages"),
            {TAILLE_L * 0.761,
                TAILLE_H * 0.010,
                TAILLE_L * 0.065,
                TAILLE_H * 0.125},
            action_personnages,
            "PERSONNAGES"
        };
    t_bouton btn_fullscreen = { recuperer_texture("bouton_fullscreen_on"),
            {TAILLE_L * 0.892,
                TAILLE_H * 0.017,
                TAILLE_L * 0.065,
                TAILLE_H * 0.114},
            action_fullscreen,
            "FULLSCREEN"
        };
    t_bouton btn_menu = { recuperer_texture("bouton_menu"),
            {TAILLE_L * 0.012,
                TAILLE_H * 0.87,
                TAILLE_L * 0.065,
                TAILLE_H * 0.114},
            action_home,
            "HOME"
        };
    t_bouton btn_jouer = { recuperer_texture("bouton_jouer"),
            {TAILLE_L * 0.67,
                TAILLE_H * 0.7,
                TAILLE_L * 0.201,
                TAILLE_H * 0.154},
            action_jouer,
            "PLAY"
        };
    t_bouton btn_quitter = { recuperer_texture("bouton_quitter"),
            {TAILLE_L * 0.12,
                TAILLE_H * 0.7,
                TAILLE_L * 0.201,
                TAILLE_H * 0.154},
            action_quitter,
            "QUIT"
        };

    t_bouton * menus[4][10] = {
        {&btn_fullscreen, &btn_volume, &btn_jouer, &btn_quitter, &btn_parametres, &btn_personnages, &btn_titre, NULL}, // menu principal
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_parametres, &btn_personnages, NULL}, // menu serveur
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_personnages, NULL}, // menu paramètres
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_parametres, NULL} // menu personnages
    };
    SDL_Texture * fonds_menus[4] = {
        recuperer_texture("menu_fond"),
        recuperer_texture("menu_serveur"),
        recuperer_texture("menu_paramètres"),
        recuperer_texture("menu_personnages")
    };

    t_etat etat = {
        FAUX,
        FAUX,
        PAGE_MENU,
        menus[PAGE_MENU],
        fenetre
    };

    int i_btn;
    t_bouton * btn;
    while (!etat.doit_quitter) {
>>>>>>> 103bedee7b5811b630f5d9fa6378a498434d22ec
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    etat.doit_quitter = VRAI;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        SDL_Point pointeur = {event.button.x, event.button.y};
                        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++) {
                            if (SDL_PointInRect(&pointeur, &btn->rect)) {
                                btn->action(&etat);
                                printf("Bouton %s cliqué\n", btn->nom);
                            }
                        }
                    }
<<<<<<< HEAD

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
                        //bouton_enfonce = 0;
                        //play_btn.y = TAILLE_H * 0.7;
                    }
=======
>>>>>>> 103bedee7b5811b630f5d9fa6378a498434d22ec
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            etat.doit_quitter = VRAI;
                            break;
                        case SDL_SCANCODE_A:
                            boucle_jeu(rend);
                            break;
                        case SDL_SCANCODE_E:
                            SDL_QueueAudio(audio_device, buffer_essai_mix, son_essai->length);
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }
        
        // Mettre à jour l'écran
        SDL_RenderClear(rend);
<<<<<<< HEAD
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
                play_btn.y += 2;
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



=======
        SDL_RenderCopy(rend, fonds_menus[etat.i_menu], NULL, NULL);
        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++)
            SDL_RenderCopy(rend, btn->texture, NULL, &btn->rect);
>>>>>>> 103bedee7b5811b630f5d9fa6378a498434d22ec
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / FPS);
    }
//
    // Nettoyage

    detruire_ressources();

    SDL_CloseAudioDevice(audio_device);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
