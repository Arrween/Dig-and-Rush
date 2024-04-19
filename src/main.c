#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "tour.h"
#include "texte.h"
#include "constantes.h"
#include "menu.h"
#include "ressources.h"


SDL_Rect controlsRect = { (TAILLE_L - 400) / 2, (TAILLE_H +15) / 2, 230, 75 };


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

    initialiser_sdl_ttf();

    // écran de chargement
    SDL_Color couleur_txt_chargement = {255,255,255,255};
    TTF_Font * police = TTF_OpenFont("ressources/Polices/linux_libertine.ttf", 50);
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
    initialiser_sdl_mixer();
    Mix_AllocateChannels(N_CANAUX);
    // réglage du volume, -1 pour tous les canaux
    Mix_Volume(-1, VOLUME_SONS_INI);
    Mix_VolumeMusic(VOLUME_MUSIQUE_INI);

    init_ressources(rend);
        
    t_bouton btn_titre = { recuperer_texture("menu_titre"),
            {TAILLE_L * (0.5 - 0.238),
                TAILLE_H * (0.5 - 0.42),
                TAILLE_L * 0.476,
                TAILLE_H * 0.417},
            (void(*)(t_etat*)) action_nulle,
            "TITRE"
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
    t_bouton btn_option = { recuperer_texture("options"),
            {TAILLE_L * 0.24,
                TAILLE_H * 0.35,
                TAILLE_L * 0.35,
                TAILLE_H * 0.35},
            action_parametres,
            "OPTIONS"
        };
    // SDL_Rect mattRect = { (TAILLE_L - 165) / 2, (TAILLE_H - 195) / 2, 80, 100 };
    t_bouton btn_perso_matt = { recuperer_texture("matt"),
            {(TAILLE_L - 165) / 2,
                (TAILLE_H - 215) / 2,
                80,
                100},
            action_perso_matt,
            "PERSO_MATT"
        };
    t_bouton btn_perso_jack = { recuperer_texture("jack"),
            {(TAILLE_L + 30) / 2,
                (TAILLE_H - 215) / 2,
                80,
                100},
            action_perso_jack,
            "PERSO_JACK"
        };
    t_bouton btn_perso_yohan = { recuperer_texture("yohan"),
            {(TAILLE_L - 165) / 2,
                (TAILLE_H ) / 2,
                80,
                100},
            action_perso_yohan,
            "PERSO_YOHAN"
        };
    t_bouton btn_perso_ania = { recuperer_texture("ania"),
            {(TAILLE_L + 30) / 2,
                (TAILLE_H ) / 2,
                80,
                100},
            action_perso_ania,
            "PERSO_ANIA"
        };



    t_bouton * menus[3][14] = {
        {&btn_fullscreen, &btn_volume, &btn_continue , &btn_quitter, &btn_parametres, &btn_personnages, &btn_titre, NULL}, // menu principal
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_jouer, &btn_parametres, &btn_titre_perso, &btn_back, 
                &btn_perso_matt, &btn_perso_jack, &btn_perso_yohan, &btn_perso_ania, NULL},// menu personnages
        {&btn_fullscreen, &btn_volume, &btn_menu, &btn_personnages, &btn_option, NULL}, // menu paramètres
    };

    SDL_Texture * fonds_menus[3] = {
        recuperer_texture("menu_fond"),
        recuperer_texture("menu_personnages"),
        recuperer_texture("menu_paramètres"),

    };

    SDL_Rect rect_perso_selectionne = {TAILLE_L * (.25 + .75/2), TAILLE_H * .175, TAILLE_L * .25, TAILLE_H * .5};
    SDL_Rect rect_barre_vie = {TAILLE_L * .15, TAILLE_H * .35, TAILLE_L * .2, TAILLE_H * .15};
    SDL_Rect rect_barre_energie = {TAILLE_L * .15, TAILLE_H * .5, TAILLE_L * .2, TAILLE_H * .15};
    SDL_Rect rect_contour = {(TAILLE_L - 220) / 2, (TAILLE_H - 280) / 2, 220, 280};
    SDL_Texture * tex_perso_selectionne = NULL;
    SDL_Texture * tex_barre_vie = NULL;
    SDL_Texture * tex_barre_energie = NULL;
    SDL_Texture * tex_contour = recuperer_texture("koba");
    t_texte * texte_perso_selectionne = creer_texte("police_defaut", 255, 0, 0, 255, TAILLE_L/2 + 215, TAILLE_H/2 - 215, 200, 50);
    t_texte * texte_avertissement = creer_texte("police_defaut", 255, 255, 255, 255, btn_jouer.rect.x - 10,
                                                                                     btn_jouer.rect.y + btn_jouer.rect.h + 5,
                                                                                     btn_jouer.rect.w + 5,
                                                                                     20);

    t_etat etat = {
        FAUX,
        FAUX,
        PAGE_MENU,
        menus[PAGE_MENU],
        fenetre,
        "",
        tex_perso_selectionne,
        texte_perso_selectionne,
        tex_barre_vie,
        tex_barre_energie,
        texte_avertissement,
        rend,
    };

    jouer_audio(0, "musique_menu", -1);

    int i_btn;
    t_bouton * btn;

    while (!etat.doit_quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    etat.doit_quitter = VRAI;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        SDL_Point pointeur = {event.button.x, event.button.y};

                        if (etat.i_menu == PAGE_MENU_PARAMETRES) {
                            if (SDL_PointInRect(&pointeur, &controlsRect)) {
                                SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); // Couleur rouge
                                SDL_RenderDrawRect(rend, &controlsRect); // Dessiner le rectangle

                                // Charger et afficher la texture du nouveau fond
                                SDL_Texture* nouveauFond = recuperer_texture("menu_commandes");
                                SDL_Rect destRect = {0, 0, TAILLE_L, TAILLE_H}; // Rectangle de destination pour le rendu
                                SDL_RenderCopy(rend, nouveauFond, NULL, &destRect);
                                SDL_RenderPresent(rend); // Mettre à jour le rendu
                                SDL_Delay(5000); // Attendre 5 secondes (5000 millisecondes)
                                printf("Controls sélectionné\n");
                                
                            }
                        }

                        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]) && !SDL_PointInRect(&pointeur, &btn->rect); i_btn++);
                        if (btn) {
                            btn->action(&etat);
                            printf("Bouton %s cliqué\n", btn->nom);
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
                            etat.doit_quitter = boucle_jeu(rend, "matt");
                            break;
                        case SDL_SCANCODE_S:
                            etat.doit_quitter = boucle_jeu(rend, "jack");
                            break;
                        case SDL_SCANCODE_D:
                            etat.doit_quitter = boucle_jeu(rend, "yohan");
                            break;
                        case SDL_SCANCODE_W:
                            etat.doit_quitter = boucle_jeu(rend, "ania");
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
            SDL_RenderCopy(rend, etat.tex_perso_selectionne, NULL, &rect_perso_selectionne);
            SDL_RenderCopy(rend, etat.tex_barre_vie, NULL, &rect_barre_vie);
            SDL_RenderCopy(rend, etat.tex_barre_energie, NULL, &rect_barre_energie);
            afficher_texte(rend, etat.texte_perso_selectionne);
            SDL_RenderCopy(rend, tex_contour, NULL, &rect_contour);
            afficher_texte(rend, etat.texte_avertissement);
        }
        
        // Afficher les autres boutons du menu
        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++)
            SDL_RenderCopy(rend, btn->texture, NULL, &btn->rect);

        SDL_RenderPresent(rend);
        SDL_Delay(1000 / FPS);
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
