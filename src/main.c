#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "constantes.h"
#include "entite.h"
#include "tour.h"
#include "menu.h"
#include "ressources.h"
#include "selection_personnages.h"

#define MAX_PERSONNAGES 3 // Nombre maximum de personnages que vous souhaitez charger
#define N 1 // Taille maximale du nom de fichier pour les textures des personnages


Persos personnages; // Assurez-vous que MAX_PERSONNAGES est défini correctement
char filename[N]; // Assurez-vous que N est défini correctement

t_entite* matt;
SDL_Rect mattRect = { 35, 33, 17, 17 }; // x, y, largeur, hauteur

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
    SDL_Surface * surface_txt_chargement = TTF_RenderText_Solid(police, "Chargement...", couleur_txt_chargement);
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

    jouer_audio(0, "musique_menu", -1);

    int i_btn;
    t_bouton * btn;
    int mattSelectionne = 0; // Matt n'est pas sélectionné initialement

    matt = creer_entite_depuis_spritesheet("matt", 35, 33.5, 17, 17, VRAI);
    
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
                        }                      
                        
                        for (i_btn = 0; (btn = menus[etat.i_menu][i_btn]); i_btn++) {
                            if (SDL_PointInRect(&pointeur, &btn->rect)) {
                                btn->action(&etat);
                                printf("Bouton %s cliqué\n", btn->nom);
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
                            jouer_audio(0, "coq", 0);
                            etat.doit_quitter = boucle_jeu(rend);
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
        if (!mattSelectionne) {
            // Sauvegarder la couleur actuelle du dessin
            printf("MAtt");
            SDL_Color couleurOriginale;
            SDL_GetRenderDrawColor(rend, &couleurOriginale.r, &couleurOriginale.g, &couleurOriginale.b, &couleurOriginale.a);
            
            // Changer la couleur du dessin en rouge
            SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
            
            // Dessiner un contour autour de Matt
            SDL_RenderDrawRect(rend, &mattRect);
            
            // Rétablir la couleur originale du dessin
            SDL_SetRenderDrawColor(rend, couleurOriginale.r, couleurOriginale.g, couleurOriginale.b, couleurOriginale.a);

            // Afficher Matt avec un effet de sélection
            // Vous pouvez implémenter cette fonctionnalité selon vos besoins
            afficher_personnage_selectionne(rend, matt, mattSelectionne);
        } else {
            // Afficher Matt sans effet de sélection
            afficher_personnage_selectionne(rend, matt, mattSelectionne);
        }
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
