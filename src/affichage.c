#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/affichage.h"


void initAffichage(StructAffichage *affichage, char nomFenetre[])
{
    // Initialiser les bibliothèques de rendu
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // Créer une fenêtre
    affichage->window = SDL_CreateWindow(nomFenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_SetWindowIcon(affichage->window, IMG_Load("assets/img/icon.png"));
    SDL_RaiseWindow(affichage->window);

    // Créer un renderer
    affichage->renderer = SDL_CreateRenderer(affichage->window, -1, SDL_RENDERER_ACCELERATED);

    // Charger les textures et couleurs
    chargerTextures(&affichage->structTextures, affichage->renderer);
    chargerCouleurs(&affichage->structCouleur);
}

void chargerTextures(StructTextures *structTextures, SDL_Renderer *renderer)
{
    SDL_Surface *surfaceTmp = NULL; // Créer une surface temporaire

    surfaceTmp = IMG_Load("assets/img/feuille_sprite.png"); // Charger une image dans la surface temporaire
    (*structTextures).feuilleSprites = SDL_CreateTextureFromSurface(renderer, surfaceTmp); // Créer une texture à partir de la surface temporaire

    surfaceTmp = IMG_Load("assets/img/sol_jeu.jpg");
    (*structTextures).solJeu = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/mur_indestructible.png");
    (*structTextures).murIndestructible = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/mur_destructible.png");
    (*structTextures).murDestructible = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/bombe.png");
    (*structTextures).bombe = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/etincelle.gif");
    (*structTextures).etincelle = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/explosion.png");
    (*structTextures).explosion = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_basse_noire.png");
    (*structTextures).flecheBasseNoire = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_haute_noire.png");
    (*structTextures).flecheHauteNoire = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_gauche_noire.png");
    (*structTextures).flecheGaucheNoire = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    surfaceTmp = IMG_Load("assets/img/fleche_droite_noire.png");
    (*structTextures).flecheDroiteNoire = SDL_CreateTextureFromSurface(renderer, surfaceTmp);

    SDL_FreeSurface(surfaceTmp); // Détruire la surface temporaire
}

void chargerCouleurs(StructCouleur *structCouleur)
{
    // Blanc
    structCouleur->blanc.r = 255;
    structCouleur->blanc.g = 255;
    structCouleur->blanc.b = 255;

    // Noir
    structCouleur->noir.r = 0;
    structCouleur->noir.g = 0;
    structCouleur->noir.b = 0;
}


/************************************************************************************/
/***************************** AFFICHAGE D'ELEMENTS *********************************/
/************************************************************************************/

void afficherJeu(StructAffichage *affichage, StructJeu *jeu)
{
    SDL_Rect caseMap = {0, 0, 30, 30}; // Case utilisée pour remplir la map
    SDL_Rect spriteInitial = {15, 13, 35, 50};  // Coordonnées du sprite initial dans la feuille de sprites
    SDL_Rect spriteDecoupe; // Coordonnées du sprite à afficher
    int nbTotalFrames = 9; // Nombre de sprites sur une même ligne de la feuille de sprites
    int dureeParFrame = 100;
    int frame;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Afficher les cases
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j<20; j++)
        {
            caseMap.x = i*30;
            caseMap.y = j*30;


            SDL_RenderCopy(affichage->renderer, affichage->structTextures.solJeu, NULL, &caseMap); //En dessous de toutes les cases on dessine le sol

            if(jeu->mapJeu[i][j] == 1)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.murIndestructible, NULL, &caseMap);
            if(jeu->mapJeu[i][j] == 2)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.murDestructible, NULL, &caseMap);
            if(jeu->mapJeu[i][j] == 3)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.bombe, NULL, &caseMap);
            if(jeu->mapJeu[i][j] == 4)
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.explosion, NULL, &caseMap);
        }
    }

    // Afficher les joueurs
    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {
        if(jeu->listeDesJoueurs[i].enVie == 1)
        {
            spriteDecoupe=spriteInitial;

            // Déterminer les coordonnées X du nouveau sprite
            if(jeu->listeDesJoueurs[i].deplacement == 1)
                frame = (SDL_GetTicks() / dureeParFrame) % nbTotalFrames; // Passer à la prochaine frame toutes les 'dureeParFrame' ms
            else
                frame = 0;
            spriteDecoupe.x += frame * 64;

            // Déterminer les coordonnées Y du nouveau sprite
            if(jeu->listeDesJoueurs[i].direction == HAUT)
                spriteDecoupe.y += 0;
            if(jeu->listeDesJoueurs[i].direction == GAUCHE)
                spriteDecoupe.y += 64;
            if(jeu->listeDesJoueurs[i].direction == BAS)
                spriteDecoupe.y += 2*64;
            if(jeu->listeDesJoueurs[i].direction == DROITE)
                spriteDecoupe.y += 3*64;

            // Déterminer la couleur du sprite
            switch(i)
            {
            case 0:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 255, 255, 255);
                break;
            case 1:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 0,100,255); //BLEU
                break;
            case 2:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 0, 255, 100); //VERT
                break;
            case 3:
                SDL_SetTextureColorMod(affichage->structTextures.feuilleSprites, 255, 70, 0); //ROUGE
                break;
            }

            // Déterminer la case sur laquelle se trouve le joueur
            caseMap.x = jeu->listeDesJoueurs[i].coordonnes.x;
            caseMap.y = jeu->listeDesJoueurs[i].coordonnes.y;

            // Copier le sprite mis à jour dans le renderer
            SDL_RenderCopy(affichage->renderer, affichage->structTextures.feuilleSprites, &spriteDecoupe, &caseMap);
        }
    }

    // Afficher le message de victoire / défaite en fin de partie
    if(jeu->animation.victoire)
    {
        afficherTexte("VICTOIRE !", 80, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 180, affichage->renderer);
        afficherTexte("Appuyez sur ENTREE", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 280, affichage->renderer);
    }
    else if(jeu->animation.defaite)
    {
        afficherTexte("DEFAITE !", 80, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 180, affichage->renderer);
        afficherTexte("Appuyez sur ENTREE", 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 280, affichage->renderer);
    }

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);
}

void afficherTexte(char texte[], int tailleTexte, SDL_Color couleurTexte, char cheminPoliceEcriture[], int positionX, int positionY, SDL_Renderer *renderer)
{
    int textureW, textureH;

    // Charger le fichier comportant la police d'écriture
    TTF_Font *policeEcriture = TTF_OpenFont(cheminPoliceEcriture, tailleTexte);

    // Ecrire le texte dans une surface
    SDL_Surface *surface = TTF_RenderText_Blended(policeEcriture, texte, couleurTexte);

    // Créer une texture à partir de la surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Déterminer les dimensions de la texture
    SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);

    // Déterminer les coordonnées de la texture si aucune n'ont été indiquées
    if(positionX == -1)
        positionX = (WIDTH / 2) - (textureW / 2);
    if(positionY == -1)
        positionY = (HEIGHT / 2) - (textureH / 2);

    // Créer le rectangle qui contiendra les coordonnées et dimensions de la texture
    SDL_Rect rectTexture = {positionX, positionY, textureW, textureH};

    // Copier le texte dans le renderer
    SDL_RenderCopy(renderer, texture, NULL, &rectTexture);

    // Libérer de la RAM
    SDL_FreeSurface(surface);
    TTF_CloseFont(policeEcriture);
    SDL_DestroyTexture(texture);
}
