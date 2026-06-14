#ifndef FAIXA_H
#define FAIXA_H

#include "playlist.h"
#include "musica.h"

typedef struct
{
    int topo;
    int livres;
} CabecalhoFaixa;

typedef struct
{
    int codigoMusica;
    int prox;
} Faixa;

void adicionarMusicaInicio(int codPlaylist, int codMusica);
void adicionarMusicaFim(int codPlaylist, int codMusica);
void removerMusicaPlaylist(int codPlaylist, int codMusica);
void imprimirNosLivres();

#endif