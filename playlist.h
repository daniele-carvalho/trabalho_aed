#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "musica.h"
#include "faixa.h"

typedef struct
{
    int cabeca;
    int topo;
} estruturaPlaylist;

typedef struct
{
    int codigo;
    char titulo[51];

    int inicioFaixas;
    int fimFaixas;

    int prox;
} Playlist;

void criarPlaylist(int codigo, char titulo[]);
int buscarPlaylist(int codigo);
void listarPlaylists();
void imprimirPlaylist(int codigo);

#endif