#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "tour.h"
#include "constantes.h"
#include "selection_personnages.h"
#include "entite.h"
#include "menu.h"
#include "ressources.h"


Persos personnages; // Assurez-vous que MAX_PERSONNAGES est défini correctement

t_entite* matt;
t_entite* jack;
t_entite* yohan;
t_entite* ania;




SDL_Rect mattRect = { (TAILLE_L - 165) / 2, (TAILLE_H - 195) / 2, 80, 100 };
SDL_Rect jackRect = { (TAILLE_L + 30) / 2, (TAILLE_H - 195) / 2, 80, 100 };
SDL_Rect yohanRect = { (TAILLE_L - 165) / 2, (TAILLE_H + 15) / 2, 80, 100 };
SDL_Rect aniaRect = { (TAILLE_L + 30) / 2, (TAILLE_H + 15) / 2, 80, 100 };



int main() {

    printf("début du main\n");

    SDL_Window * fenetre;
    SDL_Renderer * rend;
    SDL_Event event;


    initialiser_sdl();

    // Création de la fenêtre
    fenetre = creation_fenetre();

    // Création du renderer
    rend = creation_renderer(&fenetre);
    SDL_RenderSetLogicalSize(rend, TAILLE_L, TAILLE_H);

    TTF_Init();

    // écran de chargement
    SDL_Color couleur_txt_chargement = {255,255,255,255};
    TTF_Font * police = TTF_OpenFont("ressources/Menu/Police/font1.ttf", 50);
    SDL_Surface * surface_txt_chargement = TTF_RenderText_Solid(police, "Loading...", couleur_txt_chargement);
    SDL_Rect dst_txt_chargement = {TAILLE_L/2 - 100, TAILLE_H/2 - 40, 200, 80};
    SDL_Texture * tex_txt_chargement = SDL_CreateTextureFromSurface(rend, surface_txt_chargement);;
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderFillRect(rend, NULL);
    SDL_RenderCopy(rend, tex_txt_chargement, NULL, &dst_txt_chargement);
    SDL_RenderPresent(rend);

    // Initialisation de SDL_image pour le support des images PNG
    initialiser_sdl_img();

    // initialiser l’audio
    if (Mix_OpenAudio(FREQ_AUDIO, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, TAILLE_TAMPON_AUDIO) < 0) {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    Mix_AllocateChannels(N_CANAUX);
    // réglage du volume, -1 pour tous les canaux
    Mix_Volume(-1, MIX_MAX_VOLUME * FACTEUR_VOLUME_SONS_INI);
    Mix_VolumeMusic(MIX_MAX_VOLUME * FACTEUR_VOLUME_MUSIQUE_INI);

    init_ressources(rend);
        
    t_bouton btn_titre = { recuperer_texture("menu_titre"),
            {TAILLE_L * (0.5 - 0.238),
                TAILLE_H * (0.5 - 0.42),
                TAILLE_L * 0.476,
                TAILLE_H * 0.417},
            (void(*)(t_etat*)) action_nulle,
            "TITRE"
        };

    t_bouton btn_barre_de_vie_pleine = { recuperer_texture("barre_de_vie_pleine"),
        {TAILLE_L * 0.15,
            TAILLE_H * 0.25,
            TAILLE_L * 0.20,
            TAILLE_H * 0.15},
        (void(*)(t_etat*)) action_nulle,
        "PDV PLEINE"
    };
    t_bouton btn_defense_pleine = { recuperer_texture("defense_pleine"),
        {TAILLE_L * 0.15,
            TAILLE_H * 0.40,
            TAILLE_L * 0.20,
            TAILLE_H * 0.15},
        (void(*)(t_etat*)) action_nulle,
        "DEFENSE PLEINE"
    };
    t_bouton btn_energie_pleine = { recuperer_texture("energie_pleine"),
        {TAILLE_L * 0.15,
            TAILLE_H * 0.55,
            TAILLE_L * 0.20,
            TAILLE_H * 0.15},
        (void(*)(t_etat*)) action_nulle,
        "ENERGIE PLEINE"
    };
    t_bouton btn_energie_semi = { recuperer_texture("energie_semi"),
        {TAILLE_L * 0.15,
            TAILLE_H * 0.65,
            TAILLE_L * 0.20,
            TAILLE_H * 0.15},
        (void(*)(t_etat*)) action_nulle,
        "ENERGIE SEMI"
    };
    t_bouton btn_titre_perso = { recuperer_texture("titre_personnages"),
        {TAILLE_L * (0.5 - 0.238),
            TAILLE_H * (0.35 - 0.42),
            TAILLE_L * 0.476,
            TAILLE_H * 0.417},
        (void(*)(t_etat*)) action_nulle,
        "TITRE_PERSO"
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
    t_bouton btn_continue = { recuperer_texture("bouton_continue"),
            {TAILLE_L * 0.67,
                TAILLE_H * 0.7,
                TAILLE_L * 0.201,
                TAILLE_H * 0.154},
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
    t_bouton btn_back = { recuperer_texture("back"),
            {TAILLE_L * 0.12,
                TAILLE_H * 0.7,
                TAILLE_L * 0.201,
                TAILLE_H * 0.154},
            action_retour,
            "RETOUR"
        };
    t_bouton btn_quitter = { recuperer_texture("bouton_quitter"),
            {TAILLE_L * 0.12,
                TAILLE_H * 0.7,
                TAILLE_L * 0.201,
                TAILLE_H * 0.154},
            action_quitter,
            "QUIT"
        };


t_bouton * menus[3][10] = {
    {&btn_fullscreen, &btn_volume, &btn_continue , &btn_quitter, &btn_parametres, &btn_personnages, &btn_titre, NULL}, // menu principal
    {&btn_fullscreen, &btn_volume, &btn_menu, &btn_jouer, &btn_parametres, &btn_titre_perso, &btn_back, NULL}, // menu serveur
    {&btn_fullscreen, &btn_volume, &btn_menu, &btn_personnages, NULL}, // menu paramètres
    //{&btn_fullscreen, &btn_volume, &btn_menu, &btn_jouer, &btn_parametres, &btn_titre_perso, &btn_back, NULL} // menu personnages

};

    SDL_Texture * fonds_menus[3] = {
        recuperer_texture("menu_fond"),
        recuperer_texture("menu_personnages"),
        recuperer_texture("menu_paramètres"),
        //recuperer_texture("menu_personnages")

    };

    t_etat etat = {
        FAUX,
        FAUX,
        PAGE_MENU,
        menus[PAGE_MENU],
        fenetre
    };

    jouer_audio(0, "musique_menu", -1);

    int i_btn;
    int pause = 0;
    t_bouton * btn;

    matt = creer_entite_depuis_spritesheet("matt", 75, 17.5, 50, 50, VRAI);
    jack = creer_entite_depuis_spritesheet("jack", 75, 17.5, 50, 50, VRAI);
    yohan = creer_entite_depuis_spritesheet("yohan", 75, 17.5, 50, 50, VRAI);
    ania = creer_entite_depuis_spritesheet("ania", 75, 17.5, 50, 50, VRAI);



    int mattSelectionne = 0; // Matt n'est pas sélectionné initialement
    int jackSelectionne = 0; 
    int yohanSelectionne = 0; 
    int aniaSelectionne = 0; 




    
    // Afficher les personnages uniquement si le menu actuel est celui des personnages
    if (etat.i_menu == PAGE_MENU_PERSONNAGES) {
        afficher_images(rend, personnages);
    }

    while (!etat.doit_quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    etat.doit_quitter = VRAI;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        SDL_Point pointeur = {event.button.x, event.button.y};
                        // Vérifier si le clic se trouve à l'intérieur du rectangle de Matt
                        if (SDL_PointInRect(&pointeur, &mattRect)) {
                            // Gérer l'action lorsque Matt est cliqué
                            mattSelectionne = !mattSelectionne; // Inverser l'état de sélection de Matt
                            jackSelectionne = 0; // Désélectionner Jack
                            yohanSelectionne = 0; // Désélectionner Yohan
                            aniaSelectionne = 0; // Désélectionner Ania
                            printf("Personnage sélectionné : Matt\n");

                        }
                        else if (SDL_PointInRect(&pointeur, &jackRect)) {
                            jackSelectionne = !jackSelectionne; 
                            mattSelectionne = 0;
                            yohanSelectionne = 0;
                            aniaSelectionne = 0;


                            printf("Personnage sélectionné : Jack\n");

                        }  
                        else if (SDL_PointInRect(&pointeur, &yohanRect)) {
                            yohanSelectionne = !yohanSelectionne;
                            mattSelectionne = 0;
                            jackSelectionne = 0;
                            aniaSelectionne = 0;

                            printf("Personnage sélectionné : Yohan\n");

                        }                                        
                        else if (SDL_PointInRect(&pointeur, &aniaRect)) {
                            aniaSelectionne = !aniaSelectionne;
                            mattSelectionne = 0;
                            jackSelectionne = 0;
                            yohanSelectionne = 0;
                            printf("Personnage sélectionné : Ania\n");

                        }                          
                        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++) {
                            if (SDL_PointInRect(&pointeur, &btn->rect)) {
                                btn->action(&etat);
                                printf("Bouton %s cliqué\n", btn->nom);
                        
                                // Si le bouton "Play" est cliqué, exécutez l'action spécifique à ce bouton
                                /*if (btn == &btn_jouer) {
                                    jouer_audio(0, "coq", 0);
                                    etat.doit_quitter = boucle_jeu(rend);
                                }*/
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            etat.doit_quitter = VRAI;
                            break;
                        case SDL_SCANCODE_A:
                            selectionner_personnage("matt");
                            jouer_audio(0, "coq", 0);
                            etat.doit_quitter = boucle_jeu(rend);
                            break;
                        case SDL_SCANCODE_S:
                            selectionner_personnage("jack");
                            jouer_audio(0, "coq", 0);
                            etat.doit_quitter = boucle_jeu(rend);
                            break;
                        case SDL_SCANCODE_D:
                            selectionner_personnage("yohan");
                            jouer_audio(0, "coq", 0);
                            etat.doit_quitter = boucle_jeu(rend);
                            break;
                        case SDL_SCANCODE_W:
                            selectionner_personnage("ania");
                            jouer_audio(0, "coq", 0);
                            etat.doit_quitter = boucle_jeu(rend);
                            break;
                        case SDLK_SPACE:
                            pause = !pause;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }
        

        // Mettre à jour l'écran
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, fonds_menus[etat.i_menu], NULL, NULL);


        if (etat.i_menu == PAGE_MENU_PERSONNAGES) {
        afficher_persos(rend);
        afficher_images(rend, personnages);
        // Afficher Matt en fonction de son état de sélection
        if (mattSelectionne) {   
            selectionner_personnage("matt");
 
          // Afficher Matt avec un effet de sélection
            afficher_entite(rend, matt);

            SDL_Color couleur_txt_matt = {0,0,0,255};
            TTF_Font * font = TTF_OpenFont("ressources/Menu/Police/font1.ttf", 50);
            SDL_Surface * surface_txt_chargement = TTF_RenderText_Solid(font, "IDLUSEN", couleur_txt_matt);
            SDL_Rect dst_txt_chargement = {TAILLE_L/2 + 215, TAILLE_H/2 - 215, 200, 50};
            SDL_Texture * tex_txt_chargement = SDL_CreateTextureFromSurface(rend, surface_txt_chargement);;
            SDL_RenderCopy(rend, tex_txt_chargement, NULL, &dst_txt_chargement);

            SDL_RenderCopy(rend, btn_barre_de_vie_pleine.texture, NULL, &btn_barre_de_vie_pleine.rect);
            SDL_RenderCopy(rend, btn_defense_pleine.texture, NULL, &btn_defense_pleine.rect);
            SDL_RenderCopy(rend, btn_energie_pleine.texture, NULL, &btn_energie_pleine.rect);



        }
        if (jackSelectionne) { 
            selectionner_personnage("jack");
   
            afficher_entite(rend, jack);

            SDL_Color couleur_txt_jack = {0,0,0,255};
            TTF_Font * font2 = TTF_OpenFont("ressources/Menu/Police/font1.ttf", 50);
            SDL_Surface * surface_txt_chargement = TTF_RenderText_Solid(font2, "STENFRESH", couleur_txt_jack);
            SDL_Rect dst_txt_chargement = {TAILLE_L/2 + 215, TAILLE_H/2 - 215, 200, 50};
            SDL_Texture * tex_txt_chargement = SDL_CreateTextureFromSurface(rend, surface_txt_chargement);;
            SDL_RenderCopy(rend, tex_txt_chargement, NULL, &dst_txt_chargement);

            SDL_RenderCopy(rend, btn_barre_de_vie_pleine.texture, NULL, &btn_barre_de_vie_pleine.rect);
            SDL_RenderCopy(rend, btn_defense_pleine.texture, NULL, &btn_defense_pleine.rect);
            SDL_RenderCopy(rend, btn_energie_pleine.texture, NULL, &btn_energie_pleine.rect);


        }
        
        if (yohanSelectionne) {  
            selectionner_personnage("yohan");
  
            afficher_entite(rend, yohan);

            SDL_Color couleur_txt_yohan = {0,0,0,255};
            TTF_Font * font3 = TTF_OpenFont("ressources/Menu/Police/font1.ttf", 50);
            SDL_Surface * surface_txt_chargement = TTF_RenderText_Solid(font3, "YO HAN", couleur_txt_yohan);
            SDL_Rect dst_txt_chargement = {TAILLE_L/2 + 215, TAILLE_H/2 - 215, 200, 50};
            SDL_Texture * tex_txt_chargement = SDL_CreateTextureFromSurface(rend, surface_txt_chargement);;
            SDL_RenderCopy(rend, tex_txt_chargement, NULL, &dst_txt_chargement);

            SDL_RenderCopy(rend, btn_barre_de_vie_pleine.texture, NULL, &btn_barre_de_vie_pleine.rect);
            SDL_RenderCopy(rend, btn_defense_pleine.texture, NULL, &btn_defense_pleine.rect);
            SDL_RenderCopy(rend, btn_energie_pleine.texture, NULL, &btn_energie_pleine.rect);


        }
        if (aniaSelectionne) {  
            selectionner_personnage("ania");
  
            afficher_entite(rend, ania);

            SDL_Color couleur_txt_ania = {0,0,0,255};
            TTF_Font * font4 = TTF_OpenFont("ressources/Menu/Police/font1.ttf", 50);
            SDL_Surface * surface_txt_chargement = TTF_RenderText_Solid(font4, "Arrween", couleur_txt_ania);
            SDL_Rect dst_txt_chargement = {TAILLE_L/2 + 215, TAILLE_H/2 - 215, 200, 50};
            SDL_Texture * tex_txt_chargement = SDL_CreateTextureFromSurface(rend, surface_txt_chargement);;
            SDL_RenderCopy(rend, tex_txt_chargement, NULL, &dst_txt_chargement);

            SDL_RenderCopy(rend, btn_barre_de_vie_pleine.texture, NULL, &btn_barre_de_vie_pleine.rect);
            SDL_RenderCopy(rend, btn_defense_pleine.texture, NULL, &btn_defense_pleine.rect);
            SDL_RenderCopy(rend, btn_energie_pleine.texture, NULL, &btn_energie_pleine.rect);


        }
        
    }
    
    // Afficher les autres boutons du menu
    for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++)
        SDL_RenderCopy(rend, btn->texture, NULL, &btn->rect);

    SDL_RenderPresent(rend);
    SDL_Delay(1000 / FPS);

    if (pause) {
        printf("Pause\n");
        // Afficher un message de pause ou un écran de pause
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // Fond blanc pour la pause
        SDL_RenderClear(rend);
        // Afficher un message de pause, par exemple :
        // SDL_RenderCopy(renderer, textureMessagePause, NULL, &destRectMessagePause);
        SDL_RenderPresent(rend);
    }
}

//
    // Nettoyage

    detruire_ressources();

    SDL_FreeSurface(surface_txt_chargement);
    SDL_DestroyTexture(tex_txt_chargement);

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(fenetre);
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio();
    exit(EXIT_SUCCESS);
}
