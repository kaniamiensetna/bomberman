#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/affichage.h"

#include "../include/menu.h"

void initMenu(StructMenu *menu)
{
    menu->numeroFenetre = 1;

    menu->positionCurseurY = 0;
    menu->positionCurseurX = 0;

    menu->paramPartie[0] = 2; // 0 = Ordinateur / 1 = Vide / 2 = Humain
    menu->paramPartie[1] = 0;

    for(int i = 2; i < 4; i++)
    {
        menu->paramPartie[i] = 1;
    }


    strcpy(menu->entreeTexte, "AAAAA");
    strcpy(menu->profilSelectionne.nom, "Humai");
}

/******************** MENU 1 : SELECTION PROFIL **********************/

void afficherMenuSelectionProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu)
{
    int nbTotalProfils;
    CompteJoueur tabComptes[NBR_MAX_COMPTES];

    nbTotalProfils = chargerComptes(tabComptes);

    // Si l'utilisateur se déplace dans le menu
    if(clavier->toucheBas && menu->positionCurseurY != nbTotalProfils)
        menu->positionCurseurY++;
    if(clavier->toucheHaut && menu->positionCurseurY != 0)
        menu->positionCurseurY--;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("SELECTION DU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 65, 70, affichage->renderer);

    if(chargerComptes(&tabComptes) != -1) //ON VERIFIE QUE LE FICHIER DES PROFILS NEST PAS VIDE
    {
        for(int i=0; i<nbTotalProfils; i++)
        {
            if(menu->positionCurseurY == i)
                afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 260, 170 + 50 * i, affichage->renderer);
            else
                afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 260, 170 + 50 * i, affichage->renderer);
        }

        if(menu->positionCurseurY == nbTotalProfils )
            afficherTexte("Nouveau profil", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 190, 170 + 50 * nbTotalProfils, affichage->renderer);
        else
            afficherTexte("Nouveau profil", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 190, 170 + 50 * nbTotalProfils, affichage->renderer);
    }
    else
    {
        afficherTexte("APPUYER SUR ENTREE POUR ", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 150,250, affichage->renderer);
        afficherTexte("CREER UN NOUVEAU PROFIL", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 150,280, affichage->renderer);
    }

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        if(menu->positionCurseurY != nbTotalProfils && nbTotalProfils != -1)
        {

            menu->profilSelectionne = tabComptes[menu->positionCurseurY];
            menu->numeroFenetre = 3;
        }
        else if(menu->positionCurseurY == nbTotalProfils || nbTotalProfils == -1)
            menu->numeroFenetre = 2;
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        menu->numeroFenetre = 0;
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;
}

/******************** MENU 2 : CREATION PROFIL **********************/

void afficherMenuCreationProfil(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu)
{
    char caractereTmp;

    // Coordonnées initiales des éléments du menu
    SDL_Rect caseLettre = {130, 210, 65, 60};
    SDL_Rect lettre = {caseLettre.x + 5, caseLettre.y - 5, 0, 0};
    SDL_Rect flecheHaute = {caseLettre.x + 5, caseLettre.y - 30, 40, 30};
    SDL_Rect flecheBasse = {caseLettre.x + 5, caseLettre.y + caseLettre.h, 40, 30};

    // Si l'utilisateur se déplace dans le menu
    if(clavier->toucheDroite && menu->positionCurseurX != 4)
        menu->positionCurseurX++;
    if(clavier->toucheGauche && menu->positionCurseurX != 0)
        menu->positionCurseurX--;
    if(clavier->toucheHaut && menu->entreeTexte[menu->positionCurseurX] != 'A')
        menu->entreeTexte[menu->positionCurseurX]--;
    if(clavier->toucheBas && menu->entreeTexte[menu->positionCurseurX] != 'Z')
        menu->entreeTexte[menu->positionCurseurX]++;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("NOUVEAU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 110, 70, affichage->renderer);

    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);
    for(int i=0; i<5; i++)
    {
        // Cases contenant les lettres du nom
        SDL_RenderDrawRect(affichage->renderer, &caseLettre);

        // Nom
        caractereTmp=menu->entreeTexte[i];
        afficherTexte(&caractereTmp, 55, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, lettre.x, lettre.y, affichage->renderer);

        // Flèches
        if(i == menu->positionCurseurX)
        {
            if(menu->entreeTexte[menu->positionCurseurX] != 'A')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheHaute, NULL, &flecheHaute);
            if(menu->entreeTexte[menu->positionCurseurX] != 'Z')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheBasse, NULL, &flecheBasse);
        }

        // Passer aux coordonnées de la prochaine case
        caseLettre.x += caseLettre.w;
        lettre.x += caseLettre.w;
        flecheHaute.x += caseLettre.w;
        flecheBasse.x += caseLettre.w;
    }

    afficherTexte("ENTREE pour continuer", 24, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 150, 380, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        strcpy(jeu->listeDesJoueurs[0].nom, menu->entreeTexte);
        enregistrerNouveauCompte(menu->entreeTexte);
        menu->numeroFenetre = 1;

    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 1;
    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;


}

/***** MENU 3 : MENU PRINCIPAL ************************************/

void afficherMenuPrincipal(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu)
{
    char chaineBienvenue[40] = "";

    // Si l'utilisateur se déplace dans le menu
    if(clavier->toucheBas && menu->positionCurseurY != 2)
        menu->positionCurseurY++;
    if(clavier->toucheHaut && menu->positionCurseurY != 0)
        menu->positionCurseurY--;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer

    strcat(chaineBienvenue,"Bienvenue ");
    strcat(chaineBienvenue, menu->profilSelectionne.nom);
    afficherTexte(chaineBienvenue, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 150, 50, affichage->renderer);

    if(menu->positionCurseurY == 0)
        afficherTexte("STATISTIQUES", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 180, 150, affichage->renderer);
    else
        afficherTexte("STATISTIQUES", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 180, 150, affichage->renderer);

    if(menu->positionCurseurY == 1)
        afficherTexte("LANCER UNE PARTIE", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 130, 200, affichage->renderer);
    else
        afficherTexte("LANCER UNE PARTIE", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 130, 200, affichage->renderer);

    if(menu->positionCurseurY == 2)
        afficherTexte("Quitter", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 240, 250, affichage->renderer);
    else
        afficherTexte("Quitter", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 240, 250, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        switch(menu->positionCurseurY)
        {
        case 0:
            menu->numeroFenetre = 4; //Statistiques
            break;
        case 1:
            menu->numeroFenetre = 5; //Lancer une partie
            break;
        case 2:
            menu->numeroFenetre = -1; //Quitter
            break;
        }
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 1;
    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = 0;


}

/******************** MENU 4 : MENU STATISTIQUES **********************/
void afficherMenuStatistiques(StructAffichage *affichage, StructTouchesClavier *clavier, StructMenu *menu)
{
    CompteJoueur tabComptes[NBR_MAX_COMPTES];
    int nbrDeComptes;
    int tailleMaxListeAAfficher;
    char chaine[15];
    int nbrMaxVictoire;
    CompteJoueur c = {"lambda", 0, -1};
    int i = 0;

    nbrDeComptes = chargerComptes(tabComptes);

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);


    afficherTexte("Statistiques", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 180, 50, affichage->renderer);

    if(nbrDeComptes < 10)
        tailleMaxListeAAfficher = nbrDeComptes;
    else
        tailleMaxListeAAfficher = 10;


    for(int cmpt = 0; cmpt < tailleMaxListeAAfficher; cmpt++)
    {
        for(int j = cmpt+1; j<nbrDeComptes ; j++)
        {
            nbrMaxVictoire = 0;
            if(tabComptes[j].nbrVictoires > nbrMaxVictoire)
            {
                nbrMaxVictoire = tabComptes[j].nbrVictoires;
                c = tabComptes[cmpt];
                tabComptes[cmpt] = tabComptes[j];
                tabComptes[j] = c;
                j = nbrDeComptes;
            }

        }
    }

    while(i < tailleMaxListeAAfficher)
    {

        sprintf(chaine, "Victoires : %d", tabComptes[i].nbrVictoires);
        afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 80, (i+1)*50 + 80, affichage->renderer);

        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 250, (i+1)*50 + 80, affichage->renderer);
        sprintf(chaine, "Defaites : %d", tabComptes[i].nbrDefaites);

        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 400, (i+1)*50 + 80, affichage->renderer);
        i++;
    }

    afficherTexte("Appuyer sur Echap pour returner au menu", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 90, i*50 + 250, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 3;
    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = 0;

}


/******************** MENU 5 : MENU DE PARAMETRAGE DE LA PARTIE **********************/

void afficherMenuParametragePartie(StructAffichage *affichage, StructTouchesClavier *clavier, StructJeu *jeu, StructMenu *menu)
{

    SDL_Rect contourSelection = {130, 130, 300, 50};
    SDL_Rect flecheGauche = {80, 135, 40, 40};
    SDL_Rect flecheDroite = {130 + 300 + 10, 135, 40, 40};

    // Si l'utilisateur se déplace dans le menu

    if(clavier->toucheHaut && menu->positionCurseurY != 0)
    {
        menu->positionCurseurY--;
    }

    if(clavier->toucheBas && menu->positionCurseurY != 3)
    {
        menu->positionCurseurY++;
    }

    if(clavier->toucheGauche && menu->positionCurseurY != 0 && menu->paramPartie[menu->positionCurseurY] != 0 )
    {
        menu->paramPartie[menu->positionCurseurY]--;
    }
    if(clavier->toucheDroite && menu->positionCurseurY != 0 && menu->paramPartie[menu->positionCurseurY] != 2 && menu->positionCurseurY != 2 && menu->positionCurseurY != 3 )
    {
        menu->paramPartie[menu->positionCurseurY]++;
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("PARAMETRAGE PARTIE", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 105, 30, affichage->renderer);

    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);


    // Dessin des cases contenant les joueurs et remplissage PAR DEFAULT de celle-ci
    for(int i = 0; i < 4; i++)
    {
        SDL_RenderDrawRect(affichage->renderer, &contourSelection);

        if(i == 0)
            afficherTexte(menu->profilSelectionne.nom, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, contourSelection.x + 90, contourSelection.y + 5, affichage->renderer);
        else
        {
            if(menu->paramPartie[i] == 0)
                afficherTexte("Ordinateur", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, contourSelection.x + 60, contourSelection.y + 5, affichage->renderer);
            else if(menu->paramPartie[i] == 1)
                afficherTexte("Vide", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, contourSelection.x + 100, contourSelection.y + 5, affichage->renderer);
            else if(menu->paramPartie[i] == 2)
                afficherTexte("Humain", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, contourSelection.x + 100, contourSelection.y + 5, affichage->renderer);
        }

        contourSelection.y += 80;
    }


    flecheGauche.y = flecheGauche.y + 80*menu->positionCurseurY;
    flecheDroite.y = flecheDroite.y + 80*menu->positionCurseurY;

    // Dessin flèches
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheGauche, NULL, &flecheGauche);
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheDroite, NULL, &flecheDroite);




    afficherTexte("ENTREE pour lancer la partie", 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, 140, 490, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);


    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        initLeJeuUneDeuxiemeFois(jeu, menu);
        menu->numeroFenetre = 6;
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
    {
        menu->numeroFenetre = 1;

    }
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;


}





/*************************************************************/
/*************FONCTIONS POUR SIMPLIFIER LE CODE **************/
/*************************************************************/


void enregistrerNouveauCompte(char nomCompte[])
{

    FILE *fic;
    CompteJoueur c;

    strcpy(c.nom, nomCompte);
    c.nbrVictoires = 0;
    c.nbrDefaites = 0;

    fic = fopen (CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "a");
    if(fic == NULL)
    {
        printf("[ECRITURE] Impossible d'accéder à la liste des joueurs\n");
    }
    else
    {
        fwrite(&c, sizeof(c), 1, fic);
        printf("Ecriture réussit !");
    }
    fclose(fic);
}



int chargerComptes(CompteJoueur *tabComptes)
{

    CompteJoueur compte;
    int nbrDeComptes = 0;

    FILE *fic;
    fic = fopen (CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "r");
    if(fic == NULL)
    {
        printf("Impossible d'accéder à la liste des joueurs\n");
        return -1;
    }
    else
    {

        while(fread(&compte, sizeof(compte), 1, fic) && !feof(fic))
        {
            tabComptes[nbrDeComptes] = compte;
            nbrDeComptes++;
        }

        printf("Lecture réussit ! Il y a  %d profils enregistrés !\n", nbrDeComptes);
        return nbrDeComptes;
    }
    fclose(fic);
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

    // Créer le rectangle qui contiendra les coordonnées et dimensions de la texture
    SDL_Rect rectTexture = {positionX, positionY, textureW, textureH};

    // Afficher le texte
    SDL_RenderCopy(renderer, texture, NULL, &rectTexture);

    // Libérer de la RAM
    SDL_FreeSurface(surface);
    TTF_CloseFont(policeEcriture);
    SDL_DestroyTexture(texture);
}

void initLeJeuUneDeuxiemeFois(StructJeu *jeu, StructMenu *menu)
{
    int nbrJoueursHumains = 0;
    int nbrJoueursIA = 0;

    CompteJoueur cDefault = {"Test", 0, 0};

    for(int i = 0; i < 4; i ++)
    {
        if(menu->paramPartie[i] == 0)
            nbrJoueursIA++;
        else if (menu->paramPartie[i] == 2)
            nbrJoueursHumains++;
    }


    for(int i = 0; i < nbrJoueursHumains; i++)
    {
        if(i == 0)
            jeu->listeDesJoueurs[i].compte = menu->profilSelectionne;
        else
            jeu->listeDesJoueurs[i].compte = cDefault;

        jeu->listeDesJoueurs[i].humainOuIA = 0;
    }

    for(int i = nbrJoueursHumains; i < nbrJoueursHumains + nbrJoueursIA; i++)
        jeu->listeDesJoueurs[i].humainOuIA = 1;

    jeu->nbrDeJoueurs = nbrJoueursHumains + nbrJoueursIA;

}


