#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/playlist.h"
#include "../includes/faixa.h"
#include "../includes/musica.h"

/*
 * Propósito: Imprime todas as músicas de uma playlist específica
 * Pré-condição: Código da playlist válido
 * Pós-condição: Título da playlist e músicas impressos na tela
 */
void imprimirPlaylist(int codigo)
{
    FILE *arqPlay;
    FILE *arqFaixa;
    estruturaPlaylist cabPlay;
    Playlist p;
    int posPlay, posFaixa;
    int contador = 1;

    arqPlay = fopen("playlist.bin", "rb");

    if(arqPlay == NULL)
    {
        printf("Arquivo de playlists nao encontrado.\n");
        return;
    }

    fread(&cabPlay, sizeof(estruturaPlaylist), 1, arqPlay);

    posPlay = cabPlay.cabeca;

    while(posPlay != -1)
    {
        fseek(arqPlay,
              sizeof(estruturaPlaylist) + posPlay * sizeof(Playlist),
              SEEK_SET);

        fread(&p, sizeof(Playlist), 1, arqPlay);

        if(p.codigo == codigo)
        {
            printf("\n========================================\n");
            printf("PLAYLIST: %s (Codigo: %d)\n", p.titulo, p.codigo);
            printf("========================================\n");

            if(p.inicioFaixas == -1)
            {
                printf("Playlist vazia.\n");
            }
            else
            {
                arqFaixa = fopen("faixas.bin", "rb");
                
                if(arqFaixa == NULL)
                {
                    printf("Erro ao abrir arquivo de faixas.\n");
                    fclose(arqPlay);
                    return;
                }

                // Pular cabeçalho
                fseek(arqFaixa, sizeof(CabecalhoFaixa), SEEK_SET);

                posFaixa = p.inicioFaixas;
                contador = 1;

                while(posFaixa != -1)
                {
                    Faixa f;
                    
                    fseek(arqFaixa,
                          posFaixa * sizeof(Faixa),
                          SEEK_CUR);
                    
                    fread(&f, sizeof(Faixa), 1, arqFaixa);

                    // Buscar dados da música
                    int posMusica = buscarMusica(f.codigoMusica);
                    
                    if(posMusica != -1)
                    {
                        FILE *arqMus = fopen("musica.bin", "rb");
                        if(arqMus != NULL)
                        {
                            estruturaMusica cabMus;
                            Musica m;
                            
                            fread(&cabMus, sizeof(estruturaMusica), 1, arqMus);
                            
                            fseek(arqMus,
                                  sizeof(estruturaMusica) + posMusica * sizeof(Musica),
                                  SEEK_SET);
                            fread(&m, sizeof(Musica), 1, arqMus);
                            
                            printf("%d. %s - %s (Ano: %d)\n", 
                                   contador++, m.titulo, m.artista, m.ano);
                            
                            fclose(arqMus);
                        }
                    }
                    else
                    {
                        printf("%d. Musica com codigo %d (nao encontrada no acervo)\n", 
                               contador++, f.codigoMusica);
                    }

                    posFaixa = f.prox;
                    
                    // Voltar para posição correta para continuar
                    fseek(arqFaixa, sizeof(CabecalhoFaixa), SEEK_SET);
                }
                
                fclose(arqFaixa);
            }

            fclose(arqPlay);
            return;
        }

        posPlay = p.prox;
    }

    printf("Playlist com codigo %d nao encontrada.\n", codigo);
    fclose(arqPlay);
}

void criarPlaylist(int codigo, char titulo[])
{
    FILE *fplaylist;
    estruturaPlaylist cab;
    Playlist p, aux;
    int novaPos, atual;

    fplaylist = fopen("playlist.bin", "r+b");

    if (fplaylist == NULL)
    {
        fplaylist = fopen("playlist.bin", "w+b");

        cab.cabeca = -1;
        cab.topo = 0;

        fwrite(&cab, sizeof(estruturaPlaylist), 1, fplaylist);
        fflush(fplaylist);
    }
    else
    {
        fread(&cab, sizeof(estruturaPlaylist), 1, fplaylist);
    }

    if (buscarPlaylist(codigo) != -1)
    {
        printf("Playlist ja cadastrada.\n");
        fclose(fplaylist);
        return;
    }

    p.codigo = codigo;
    strcpy(p.titulo, titulo);

    p.inicioFaixas = -1;
    p.fimFaixas = -1;
    p.prox = -1;

    novaPos = cab.topo;

    fseek(
        fplaylist,
        sizeof(estruturaPlaylist) + novaPos * sizeof(Playlist),
        SEEK_SET);

    fwrite(&p, sizeof(Playlist), 1, fplaylist);

    if (cab.cabeca == -1)
    {
        cab.cabeca = novaPos;
    }
    else
    {
        atual = cab.cabeca;

        while (1)
        {
            fseek(
                fplaylist,
                sizeof(estruturaPlaylist) + atual * sizeof(Playlist),
                SEEK_SET);

            fread(&aux, sizeof(Playlist), 1, fplaylist);

            if (aux.prox == -1)
                break;

            atual = aux.prox;
        }

        aux.prox = novaPos;

        fseek(
            fplaylist,
            sizeof(estruturaPlaylist) + atual * sizeof(Playlist),
            SEEK_SET);

        fwrite(&aux, sizeof(Playlist), 1, fplaylist);
    }

    cab.topo++;

    fseek(fplaylist, 0, SEEK_SET);
    fwrite(&cab, sizeof(estruturaPlaylist), 1, fplaylist);

    fclose(fplaylist);
    printf("Playlist criada com sucesso!\n");
}

int buscarPlaylist(int codigo)
{
    FILE *arq;
    estruturaPlaylist cab;
    Playlist p;
    int pos;

    arq = fopen("playlist.bin", "rb");

    if (arq == NULL)
        return -1;

    fread(&cab, sizeof(estruturaPlaylist), 1, arq);

    pos = cab.cabeca;

    while (pos != -1)
    {
        fseek(arq,
              sizeof(estruturaPlaylist) + pos * sizeof(Playlist),
              SEEK_SET);

        fread(&p, sizeof(Playlist), 1, arq);

        if (p.codigo == codigo)
        {
            fclose(arq);
            return pos;
        }

        pos = p.prox;
    }

    fclose(arq);

    return -1;
}

void listarPlaylists()
{
    FILE *arq;
    estruturaPlaylist cab;
    Playlist p;
    int pos;

    arq = fopen("playlist.bin", "rb");

    if (arq == NULL)
    {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    fread(&cab, sizeof(estruturaPlaylist), 1, arq);

    if (cab.cabeca == -1)
    {
        printf("Nenhuma playlist cadastrada.\n");
        fclose(arq);
        return;
    }

    printf("\n===== PLAYLISTS =====\n");

    pos = cab.cabeca;

    while (pos != -1)
    {
        fseek(arq,
              sizeof(estruturaPlaylist) + pos * sizeof(Playlist),
              SEEK_SET);

        fread(&p, sizeof(Playlist), 1, arq);

        printf("Codigo: %d\n", p.codigo);
        printf("Titulo: %s\n", p.titulo);
        printf("-------------------\n");

        pos = p.prox;
    }

    fclose(arq);
}