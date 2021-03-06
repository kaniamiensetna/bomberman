#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL_mixer.h>


typedef enum {
    MUSIQUE, SON_MENU_TOUCHE_DIRECTIONNELLE, SON_MENU_TOUCHE_VALIDER, SON_POSE_BOMBE, SON_EXPLOSION_BOMBE, SON_MORT_PERSONNAGE, SON_VICTOIRE, SON_DEFAITE
} SonSelectionne;

typedef struct {
    Mix_Music  *musiqueDuJeu;
    Mix_Chunk *sonMenuToucheDirectionnelle;
    Mix_Chunk *sonMenuToucheValider;
    Mix_Chunk *sonPoseBombe;
    Mix_Chunk *sonExplosionBombe;
    Mix_Chunk *sonMortPersonnage;
    Mix_Chunk *sonVictoire;
    Mix_Chunk *sonDefaite;
} StructAudio;

void initAudio(StructAudio *audio);
void chargerAudio(StructAudio *audio);
void lireAudio(StructAudio *audio, SonSelectionne sonSelectionne);


#endif // AUDIO_H
