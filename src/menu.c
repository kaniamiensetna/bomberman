#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/menu.h"
#include "../include/affichage.h"
#include "../include/clavier.h"


void initMenu(StructMenu *menu)
{
    // Initialiser le système de numérotation de fenêtres
    menu->numeroFenetre = 0;
    menu->dernierNumeroFenetre = menu->numeroFenetre;
}

void afficherMenu(StructMenu *menu, StructJeu *jeu, StructClavier *clavier, StructAffichage *affichage, StructAudio *audio)
{
    // S'il y'a changement de fenêtre
    if(menu->numeroFenetre != menu->dernierNumeroFenetre)
    {
        // Réinitialiser les curseurs
        menu->positionCurseurY = 0;
        menu->positionCurseurX = 0;

        // Cas spécifiques
        if(menu->numeroFenetre == 3)
            // Placer le curseur sur le bouton jouer
            menu->positionCurseurY = 1;
        else if(menu->numeroFenetre == 2)
        {
            // Afficher un champ texte vide
            menu->tabNomDuJoueur[0] = 'A';
            menu->tabNomDuJoueur[1] = '\0';
        }
        else if(menu->numeroFenetre == 5)
        {
            // Afficher les paramètres par défaut d'une partie (0 = Ordinateur / 1 = Vide / 2 = Humain)
            menu->paramPartie[0] = 2;
            menu->paramPartie[1] = 0;
            menu->paramPartie[2] = 1;
            menu->paramPartie[3] = 1;
        }

        // Actualiser la numérotation de la dernière fenêtre visitée
        menu->dernierNumeroFenetre = menu->numeroFenetre;
    }

    // Afficher le menu
    switch(menu->numeroFenetre)
    {
    case 0:
        // Menu d'accueil
        afficherMenuAccueil(affichage, clavier, menu, audio);
        break;
    case 1:
        // Menu demandant de sélectionner un profil et/ou d'en créer un
        afficherMenuSelectionProfil(affichage, clavier, menu, audio);
        break;
    case 2:
        // Menu de création de profil
        afficherMenuCreationProfil(affichage, clavier, menu, audio);
        break;
    case 3:
        // Menu principal (Choix entre 'Jouer', 'Statistiques' et 'Quitter')
        afficherMenuPrincipal(affichage, clavier, menu, audio);
        break;
    case 4:
        // Menu affichant les statistiques
        afficherMenuStatistiques(affichage, clavier, menu);
        break;
    case 5:
        // Menu permettant de paramétrer sa partie avant de la lancer
        afficherMenuParametragePartie(affichage, clavier, jeu, menu, audio);
        break;
    case 6:
        jeu->etat = LANCEMENT;
        break;
    case 7:
        //Menu pause affiché par dessus le jeu
        afficherMenuPause(affichage, clavier, jeu, menu, audio);
        break;
    case -1:
        clavier->toucheQuitter = 1;
        break;
    }
}


/****************************************************************************/
/***************************** MENU 0 : ACCUEIL *****************************/
/****************************************************************************/

void afficherMenuAccueil(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio)
{
    int couleurTexteClignotant = (SDL_GetTicks() / 6) % 255; // Passe de 0 à 255 en incrémentant toutes les 6 ms
    SDL_Color color = {couleurTexteClignotant, couleurTexteClignotant, couleurTexteClignotant, 0};

    SDL_Rect rectBombe;
    rectBombe.w = 250;
    rectBombe.h = 250;
    rectBombe.x = WIDTH / 2 - rectBombe.w / 2;
    rectBombe.y = 150;

    SDL_Rect rectEtincelle;
    rectEtincelle.w = 60;
    rectEtincelle.h = 60;
    rectEtincelle.x = 297;
    rectEtincelle.y = 132;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("* BOMBERMAN *", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.bombe, NULL, &rectBombe);
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.etincelle, NULL, &rectEtincelle);
    afficherTexte("ENTREE pour demarrer", 20, color, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 450, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
        menu->numeroFenetre = 1;
    }
}


/****************************************************************************************/
/***************************** MENU 1 : SELECTION DE PROFIL *****************************/
/****************************************************************************************/

void afficherMenuSelectionProfil(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio)
{
    CompteJoueur tabComptes[NBR_MAX_COMPTES];
    int nbTotalProfils = chargerComptes(tabComptes);

    // Si l'utilisateur se déplace dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != nbTotalProfils)
    {
        menu->positionCurseurY++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0)
    {
        menu->positionCurseurY--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("SELECTION DU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    if(chargerComptes(tabComptes) != -1)
    {
        for(int i=0; i<nbTotalProfils; i++)
        {
            if(menu->positionCurseurY == i)
                afficherTexte(tabComptes[i].nom, 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * i, affichage->renderer);
            else
                afficherTexte(tabComptes[i].nom, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * i, affichage->renderer);
        }

        if(menu->positionCurseurY == nbTotalProfils)
            afficherTexte("Nouveau profil", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * nbTotalProfils, affichage->renderer);
        else
            afficherTexte("Nouveau profil", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170 + 50 * nbTotalProfils, affichage->renderer);
    }
    else
    {
        afficherTexte("APPUYER SUR ENTREE POUR ", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1,250, affichage->renderer);
        afficherTexte("CREER UN NOUVEAU PROFIL", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1,280, affichage->renderer);
    }

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
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


/***************************************************************************************/
/***************************** MENU 2 : CREATION DE PROFIL *****************************/
/***************************************************************************************/

void afficherMenuCreationProfil(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio)
{
    char caractereActuel;
    int nbMaxLettres = 8;

    SDL_Rect caseLettre;
    caseLettre.w = 55;
    caseLettre.h = 50;
    caseLettre.x = WIDTH / 2 - (caseLettre.w / 2) * nbMaxLettres;
    caseLettre.y = 170;

    SDL_Rect lettre = {caseLettre.x + 5, caseLettre.y - 5, 0, 0};
    SDL_Rect flecheHauteNoire = {caseLettre.x + 5, caseLettre.y - 30, 40, 30};
    SDL_Rect flecheBasseNoire = {caseLettre.x + 5, caseLettre.y + caseLettre.h, 40, 30};

    // Si l'utilisateur appuie sur une touche : Effectuer l'action associée
    if(cycleToucheClavierRealise(&clavier->toucheDroite, clavier) && menu->positionCurseurX != nbMaxLettres - 1)
    {
        // Si la dernière lettre est sélectionnée : Ajouter une nouvelle lettre
        if(menu->tabNomDuJoueur[menu->positionCurseurX + 1] == '\0')
        {
            menu->tabNomDuJoueur[menu->positionCurseurX + 1] = 'A';
            menu->tabNomDuJoueur[menu->positionCurseurX + 2] = '\0';
        }

        // Effectuer le déplacement
        menu->positionCurseurX++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheGauche, clavier) && menu->positionCurseurX != 0)
    {
        menu->positionCurseurX--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->tabNomDuJoueur[menu->positionCurseurX] != 'A')
    {
        menu->tabNomDuJoueur[menu->positionCurseurX]--; // Décrémente le caractère ASCII de 1
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->tabNomDuJoueur[menu->positionCurseurX] != 'Z')
    {
        menu->tabNomDuJoueur[menu->positionCurseurX]++; // Incrémente le caractère ASCII de 1
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheSupprimer, clavier) &&  menu->positionCurseurX != 0)
    {
        // Si la dernière lettre est sélectionnée : La supprimer
        if(menu->tabNomDuJoueur[menu->positionCurseurX + 1] == '\0')
        {
            menu->tabNomDuJoueur[menu->positionCurseurX] = '\0';
            menu->positionCurseurX--;
            lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
        }
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Définir la couleur utilisée pour les opérations de dessin
    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);

    // Copier les éléments du menu dans le renderer
    afficherTexte("NOUVEAU PROFIL", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    for(int i=0; i<nbMaxLettres; i++)
    {
        // Cases contenant les lettres du nom
        SDL_RenderDrawRect(affichage->renderer, &caseLettre);

        // Nom
        if(i < strlen(menu->tabNomDuJoueur))
        {
            caractereActuel = menu->tabNomDuJoueur[i];
            afficherTexte(&caractereActuel, 45, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, lettre.x, lettre.y, affichage->renderer);
        }

        // Flèches
        if(i == menu->positionCurseurX)
        {
            if(menu->tabNomDuJoueur[menu->positionCurseurX] != 'A')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheHauteNoire, NULL, &flecheHauteNoire);
            if(menu->tabNomDuJoueur[menu->positionCurseurX] != 'Z')
                SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheBasseNoire, NULL, &flecheBasseNoire);
        }

        // Passer aux coordonnées de la prochaine case
        caseLettre.x += caseLettre.w;
        lettre.x += caseLettre.w;
        flecheHauteNoire.x += caseLettre.w;
        flecheBasseNoire.x += caseLettre.w;
    }

    afficherTexte("ENTREE pour continuer", 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 310, affichage->renderer);

    if(menu->tabNomDuJoueur[menu->positionCurseurX + 1] == '\0')
    {
        // Si le curseur ne se trouve ni sur le premier si sur le dernier emplacement texte
        if(0 < menu->positionCurseurX && menu->positionCurseurX < nbMaxLettres - 1)
            afficherTexte("DROITE pour ajouter une lettre | RETOUR ARRIERE pour supprimer une lettre", 14, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 345, affichage->renderer);
        // Si le curseur ne se trouve pas sur le dernier emplacement texte
        else if(menu->positionCurseurX < nbMaxLettres - 1)
            afficherTexte("DROITE pour ajouter une lettre", 14, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 345, affichage->renderer);
        // Si le curseur ne se trouve pas sur le premier emplacement texte
        else if(0 < menu->positionCurseurX)
            afficherTexte("RETOUR ARRIERE pour supprimer une lettre", 14, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 345, affichage->renderer);
    }

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
        enregistrerNouveauCompte(menu->tabNomDuJoueur);
        menu->numeroFenetre = 1;
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        menu->numeroFenetre = 1;
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;
}


/******************************************************************************/
/***************************** MENU 3 : PRINCIPAL *****************************/
/******************************************************************************/

void afficherMenuPrincipal(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu, StructAudio *audio)
{
    char chaineBienvenue[40] = "BIENVENUE ";
    strcat(chaineBienvenue, menu->profilSelectionne.nom);

    // Si l'utilisateur se déplace dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 2){
        menu->positionCurseurY++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte(chaineBienvenue, 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    if(menu->positionCurseurY == 0)
        afficherTexte("Statistiques", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170, affichage->renderer);
    else
        afficherTexte("Statistiques", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 170, affichage->renderer);

    if(menu->positionCurseurY == 1)
        afficherTexte("Lancer une partie", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 220, affichage->renderer);
    else
        afficherTexte("Lancer une partie", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 220, affichage->renderer);

    if(menu->positionCurseurY == 2)
        afficherTexte("Quitter", 30, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 270, affichage->renderer);
    else
        afficherTexte("Quitter", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 270, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
        switch(menu->positionCurseurY)
        {
        case 0:
            menu->numeroFenetre = 4; //Statistiques
            break;
        case 1:
            menu->numeroFenetre = 5; // Lancer une partie
            break;
        case 2:
            menu->numeroFenetre = -1; // Quitter
            break;
        }
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        menu->numeroFenetre = 1;
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = 0;
}


/*********************************************************************************/
/***************************** MENU 4 : STATISTIQUES *****************************/
/*********************************************************************************/

void afficherMenuStatistiques(StructAffichage *affichage, StructClavier *clavier, StructMenu *menu)
{
    CompteJoueur tabComptes[NBR_MAX_COMPTES];
    int nbrDeComptes = chargerComptes(tabComptes);
    int tailleMaxListeAAfficher;
    char chaine[15];
    int nbrMaxVictoire;
    CompteJoueur compteTmp;
    int i = 0;
    int indiceJoueurAvecMaxVictoire;

    // Déterminer la taille du classement
    if(nbrDeComptes < 10)
        tailleMaxListeAAfficher = nbrDeComptes;
    else
        tailleMaxListeAAfficher = 10;

    // Organiser les comptes par nombre de victoires décroissant
    for(int cmpt=0; cmpt<tailleMaxListeAAfficher; cmpt++)
    {
        nbrMaxVictoire = 0;
        indiceJoueurAvecMaxVictoire = 0;

        for(int j=cmpt; j<nbrDeComptes; j++)
        {
            if(tabComptes[j].nbrVictoires >= nbrMaxVictoire)
            {
                nbrMaxVictoire = tabComptes[j].nbrVictoires;
                indiceJoueurAvecMaxVictoire = j;
            }
        }
        compteTmp = tabComptes[cmpt];
        tabComptes[cmpt] = tabComptes[indiceJoueurAvecMaxVictoire];
        tabComptes[indiceJoueurAvecMaxVictoire] = compteTmp;
    }

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Copier les éléments du menu dans le renderer
    afficherTexte("STATISTIQUES", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    while(i < tailleMaxListeAAfficher)
    {
        afficherTexte(tabComptes[i].nom, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 80, (i+1)*50 + 120, affichage->renderer);

        sprintf(chaine, "Victoires : %d", tabComptes[i].nbrVictoires);
        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 250, (i+1)*50 + 120, affichage->renderer);

        sprintf(chaine, "Defaites : %d", tabComptes[i].nbrDefaites);
        afficherTexte(chaine, 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, 400, (i+1)*50 + 120, affichage->renderer);

        i++;
    }

    afficherTexte("ECHAP pour retourner au menu", 20, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, i*50 + 270, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        menu->numeroFenetre = 3;
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = 0;
}


/*********************************************************************************************/
/***************************** MENU 5 : PARAMETRAGE DE LA PARTIE *****************************/
/*********************************************************************************************/

void afficherMenuParametragePartie(StructAffichage *affichage, StructClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio)
{
    SDL_Rect contourSelection;
    contourSelection.w = 300;
    contourSelection.h = 50;
    contourSelection.x = WIDTH / 2 - contourSelection.w / 2;
    contourSelection.y = 170;

    SDL_Rect flecheGaucheNoire;
    flecheGaucheNoire.w = 40;
    flecheGaucheNoire.h = 40;
    flecheGaucheNoire.x = contourSelection.x - flecheGaucheNoire.w - 10;
    flecheGaucheNoire.y = contourSelection.y + 5;

    SDL_Rect flecheDroiteNoire;
    flecheDroiteNoire.w = 40;
    flecheDroiteNoire.h = 40;
    flecheDroiteNoire.x = contourSelection.x + contourSelection.w + 10;
    flecheDroiteNoire.y = contourSelection.y + 5;

    int nbJoueursHumainsSelectionnes = 0;

    // Déterminer le nombre de joueurs humains déjà sélectionnés
    for(int i=0; i<4; i++)
    {
        if(menu->paramPartie[i] == 2)
            nbJoueursHumainsSelectionnes++;
    }

    // Si l'utilisateur se déplace dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0){
        menu->positionCurseurY--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 3){
        menu->positionCurseurY++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheGauche, clavier))
    {
        // Sauter le choix 'Humain' si 2 sont déja selectionnés
        if(menu->paramPartie[menu->positionCurseurY] == 0 && nbJoueursHumainsSelectionnes == 2)
            menu->paramPartie[menu->positionCurseurY] = 1;
        else
            menu->paramPartie[menu->positionCurseurY]--;

        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheDroite, clavier))
    {
        // Sauter le choix 'Humain' si 2 sont déja selectionnés
        if(menu->paramPartie[menu->positionCurseurY] == 1 && nbJoueursHumainsSelectionnes == 2)
            menu->paramPartie[menu->positionCurseurY] = 0;
        else
            menu->paramPartie[menu->positionCurseurY]++;

        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Passer au premier/dernier choix si le joueur a effectué un tour complet de choix
    if(menu->paramPartie[menu->positionCurseurY] > 2)
        menu->paramPartie[menu->positionCurseurY] = 0;
    else if(menu->paramPartie[menu->positionCurseurY] < 0)
        menu->paramPartie[menu->positionCurseurY] = 2;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderClear(affichage->renderer);

    // Définir la couleur utilisée pour les opérations de dessin
    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);

    // Copier les éléments du menu dans le renderer
    afficherTexte("PARAMETRAGE PARTIE", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 70, affichage->renderer);

    for(int i = 0; i < 4; i++)
    {
        // Cases contenant le type de joueur
        SDL_RenderDrawRect(affichage->renderer, &contourSelection);

        // Type de joueur
        if(i == 0)
            afficherTexte(menu->profilSelectionne.nom, 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
        else
        {
            if(menu->paramPartie[i] == 0)
                afficherTexte("Ordinateur", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
            else if(menu->paramPartie[i] == 1)
                afficherTexte("Vide", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
            else if(menu->paramPartie[i] == 2)
                afficherTexte("Humain", 30, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, contourSelection.y + 5, affichage->renderer);
        }

        // Passer aux coordonnées de la prochaine case
        contourSelection.y += 80;
    }

    // Flèche gauche
    flecheGaucheNoire.y = flecheGaucheNoire.y + 80 * menu->positionCurseurY;
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheGaucheNoire, NULL, &flecheGaucheNoire);

    // Flèche droite
    flecheDroiteNoire.y = flecheDroiteNoire.y + 80 * menu->positionCurseurY;
    SDL_RenderCopy(affichage->renderer, affichage->structTextures.flecheDroiteNoire, NULL, &flecheDroiteNoire);

    afficherTexte("ENTREE pour lancer la partie", 20, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT, -1, 510, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
        appliquerParametragePartie(jeu, menu);
        menu->numeroFenetre = 6;
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        menu->numeroFenetre = 3;
    else if(clavier->toucheQuitter)
        menu->numeroFenetre = -1;
}


/**************************************************************************/
/***************************** MENU 7 : PAUSE *****************************/
/**************************************************************************/

void afficherMenuPause(StructAffichage *affichage, StructClavier *clavier, StructJeu *jeu, StructMenu *menu, StructAudio *audio)
{
    SDL_Rect rectFond;
    rectFond.w = 400;
    rectFond.h = 400;
    rectFond.x = WIDTH / 2 - rectFond.w / 2;
    rectFond.y = 100;

    // Afficher le fond
    SDL_SetRenderDrawColor(affichage->renderer, 110, 120, 150, 255);
    SDL_RenderFillRect(affichage->renderer, &rectFond);

    // Afficher les contours
    SDL_SetRenderDrawColor(affichage->renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(affichage->renderer, &rectFond);

    // Si l'utilisateur se déplace dans le menu
    if(cycleToucheClavierRealise(&clavier->toucheHaut, clavier) && menu->positionCurseurY != 0)
    {
        menu->positionCurseurY--;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }
    else if(cycleToucheClavierRealise(&clavier->toucheBas, clavier) && menu->positionCurseurY != 1)
    {
        menu->positionCurseurY++;
        lireAudio(audio, SON_MENU_TOUCHE_DIRECTIONNELLE);
    }

    // Copier les éléments du menu dans le renderer
    afficherTexte("* PAUSE *", 40, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 150, affichage->renderer);

    if(menu->positionCurseurY == 0)
        afficherTexte("Redemarrer la partie", 25, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 275, affichage->renderer);
    else
        afficherTexte("Redemarrer la partie", 25, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 275, affichage->renderer);

    if(menu->positionCurseurY == 1)
        afficherTexte("Quitter la partie", 25, affichage->structCouleur.noir, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 325, affichage->renderer);
    else
        afficherTexte("Quitter la partie", 25, affichage->structCouleur.blanc, CHEMIN_POLICE_ECRITURE_MONTSERRAT_BOLD, -1, 325, affichage->renderer);

    // Afficher le renderer
    SDL_RenderPresent(affichage->renderer);

    // Déterminer la prochaine fenêtre à afficher
    if(cycleToucheClavierRealise(&clavier->toucheAction, clavier))
    {
        lireAudio(audio, SON_MENU_TOUCHE_VALIDER);
        switch(menu->positionCurseurY)
        {
        case 0:
            jeu->etat = LANCEMENT;
            break;
        case 1:
            jeu->etat = OFF;
            menu->numeroFenetre = 3;
            break;
        }
    }
    else if(cycleToucheClavierRealise(&clavier->toucheArriere, clavier))
        jeu->etat = ON;
}


/*********************************************************************************************/
/***************************** FONCTIONS POUR SIMPLIFIER LE CODE *****************************/
/*********************************************************************************************/

void enregistrerNouveauCompte(char nomCompte[])
{
    CompteJoueur nouveauCompte;
    FILE *fic;

    // Créer le nouveau compte
    strcpy(nouveauCompte.nom, nomCompte);
    nouveauCompte.nbrVictoires = 0;
    nouveauCompte.nbrDefaites = 0;

    // Enregistrer le nouveau compte
    fic = fopen (CHEMIN_D_ACCES_FICHIER_COMPTES_JOUEURS, "a");

    if(fic == NULL)
        printf("[ECRITURE] Impossible d'accéder à la liste des joueurs\n");
    else
    {
        fwrite(&nouveauCompte, sizeof(nouveauCompte), 1, fic);
        fclose(fic);
    }
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
        fclose(fic);
        return nbrDeComptes;
    }
}

void appliquerParametragePartie(StructJeu *jeu, StructMenu *menu)
{
    int nbrJoueursHumains = 0;
    int nbrJoueursIA = 0;

    CompteJoueur compteTemporaire = {"Test", 0, 0}; // Compte temporaire pour les joueurs "invités" qui ne possèdent pas de compte

    // Déterminer le nombre de joueurs dans la partie
    for(int i=0; i<4; i++)
    {
        if(menu->paramPartie[i] == 0)
            nbrJoueursIA++;
        else if (menu->paramPartie[i] == 2)
            nbrJoueursHumains++;
    }
    jeu->nbrDeJoueurs = nbrJoueursHumains + nbrJoueursIA;

    // Traiter les joueurs humains
    for(int i=0; i<=nbrJoueursHumains; i++)
    {
        // Lier chacun des joueurs humain à un numéro
        jeu->listeDesJoueurs[i].humainOuIA = 0;

        // Lier chacun des joueurs humain à un compte
        if(i == 0)
            jeu->listeDesJoueurs[i].compte = menu->profilSelectionne;
        else
            jeu->listeDesJoueurs[i].compte = compteTemporaire;
    }

    // Traiter les IA
    for(int i = nbrJoueursHumains; i < jeu->nbrDeJoueurs; i++)
        // Lier chacune des IA à un numéro
        jeu->listeDesJoueurs[i].humainOuIA = 1;
}
