#include <stdio.h>
#include <stdlib.h>


#include "include/affichage.h"



void initAffichage(StructAffichage *affichage, char nomFenetre[])
{


    SDL_Init(SDL_INIT_VIDEO); //Initialise le système de gestion de rendu
    IMG_Init(IMG_INIT_PNG);

    affichage->window = SDL_CreateWindow(nomFenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN); //Création d'une fenêtre
    SDL_SetWindowIcon(affichage->window, IMG_Load("icon.png"));
    SDL_RaiseWindow(affichage->window); //Permet de placer la fenêtre au dessus de toutes les autres



    affichage->renderer = SDL_CreateRenderer(affichage->window, -1, SDL_RENDERER_ACCELERATED); //Renderer, permettant de de "dessiner" dans la fenêtre

    chargementTextures(&affichage->structTextures, affichage->renderer);
}



void chargementTextures(StructTextures *structTextures, SDL_Renderer *renderer)
{

    SDL_Surface *surface = NULL;

    surface = IMG_Load("perso2.png");
    // printf(" yoo : %s \n",SDL_GetError());
    (*structTextures).texturePerso = SDL_CreateTextureFromSurface(renderer, surface);


    surface = IMG_Load("mur.bmp"); //On charge une image sans la surface
    (*structTextures).textureMur = SDL_CreateTextureFromSurface(renderer, surface); //On met la surface dans une texture

    surface = IMG_Load("mur2.png");
    (*structTextures).textureMur2 = SDL_CreateTextureFromSurface(renderer, surface);

    surface = IMG_Load("bomb.png");
    (*structTextures).textureBomb = SDL_CreateTextureFromSurface(renderer, surface);

    surface = IMG_Load("explosion.png");
    (*structTextures).textureExplosion = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface); //On peut donc "détruire" la surface

}

/******************************************************************************/
/*****************************AFFICHAGE DU JEU*********************************/
/******************************************************************************/

void afficherJeu(StructAffichage *affichage, StructJeu *jeu)
{



    /**
    Dans la SDL, il existe une structure spécialement faite pour stocker un rectangle qui s'appelle SDL_Rect.
    Celle-ci comporte quatre champs, tous des int, que vous connaissez déjà :
    x et y : la position du rectangle par rapport au repère de votre fenêtre
    w : la largeur du rectangle
    h : la hauteur du rectangle
    **/


    /**
    Nous avons vu comment afficher une texture dans son ensemble. Cependant, il peut arriver que l'on ait besoin d'afficher
    seulement un rectangle particulier de la texture. Pour cela, il nous faut voir le prototype de la fonction SDL_RenderCopy. Le voici :

    int SDL_RenderCopy(SDL_Renderer*   renderer  ,  SDL_Texture*    texture  ,  const SDL_Rect* srcrect  ,  const SDL_Rect* dstrect);

    La fonction renvoie 0 si tout s'est bien passé et une valeur négative sinon. On peut donc procéder à l'éternelle
    vérification du bon fonctionnement de la fonction mais je vous laisse le soin de la faire (cf chapitre précédent si vous n'y arrivez pas).

    Ce qui nous intéresse, ce sont les arguments srcrect et dstrect qui correspondent respectivement à la source et à la destination de
    la texture.
    - La source est le rectangle au sein même de la texture que l'on veut afficher. Attention, la position du rectangle correspond à un repère
    relatif à l'angle en haut à gauche de la texture et non du renderer. Si l'on envoie NULL, ce rectangle recouvrira la texture entière.
    - La destination est un rectangle relatif au renderer dans lequel on va afficher la source. En fait, c'est comme si l'on considérait
    que la source était une nouvelle texture de la taille du rectangle source et qu'on l'affichait à la position indiquée par la destination.
    - Il n'est pas nécessaire de s'inquiéter de la taille de la destination car l'image mère est redimensionnée pour corresponpondre au carré
    acceuillant l'image.
    **/



    SDL_Rect rectBriqueJeu = {0, 0, 30, 30}; //Déclaration du rectangle 30*30 qui dessine tout

    SDL_Rect rectDecoupeImage;



    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255); //afffichage fond
    SDL_RenderClear(affichage->renderer);



    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j<20; j++)
        {

            rectBriqueJeu.x = i*30;
            rectBriqueJeu.y = j*30;

            if(jeu->mapJeu[j][i] == 1)  //inversion de j et i pour faciliter la lecture la matrice map
            {
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.textureMur, NULL, &rectBriqueJeu);
            }
            if(jeu->mapJeu[j][i] == 2)
            {
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.textureMur2, NULL, &rectBriqueJeu);
            }
            if(jeu->mapJeu[j][i] == 3)
            {
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.textureBomb, NULL, &rectBriqueJeu);
            }
            if(jeu->mapJeu[j][i] == 4)
            {
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.textureExplosion, NULL, &rectBriqueJeu);
            }


        }
    }

    for(int i = 0; i < jeu->nbrDeJoueurs; i++)
    {
        rectBriqueJeu.x = jeu->listeDesJoueurs[i].coordonnes.x;
        rectBriqueJeu.y = jeu->listeDesJoueurs[i].coordonnes.y;

        rectDecoupeImage.x = jeu->listeDesJoueurs[i].coordonnesSprite.x; //Coordonnée de la portion à découper dans le sprite
        rectDecoupeImage.y = jeu->listeDesJoueurs[i].coordonnesSprite.y; //Coordonnée de la portion à découper dans le sprite

        rectDecoupeImage.w = 35; //Hauteur de la portion à découper dans le sprite
        rectDecoupeImage.h = 50; //Largeur de la portion à découper dans le sprite

        SDL_RenderCopy(affichage->renderer, affichage->structTextures.texturePerso, &rectDecoupeImage, &rectBriqueJeu);

    }


    SDL_RenderPresent(affichage->renderer); //Affichage du renderer
}









