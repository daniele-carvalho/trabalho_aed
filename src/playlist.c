#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/playlist.h"
#include "../includes/faixa.h"
#include "../includes/musica.h"

/* Objetivo: imprime todas as músicas de uma playlist específica.
 * Pré-condição: código da playlist válido.
 * Pós-condição: título da playlist e músicas impressos na tela*/
void imprimirPlaylist(int codigo){
    FILE *arqPlay, *arqFaixa;
    estruturaPlaylist cabPlay;
    Playlist p;
    int posPlay, posFaixa, contador=1;

    arqPlay = fopen("playlist.bin", "rb");

    if(arqPlay == NULL){
        printf("Erro: arquivo.\n");
        return;
    }

    fread(&cabPlay, sizeof(estruturaPlaylist), 1, arqPlay);
    posPlay = cabPlay.cabeca;

    while(posPlay != -1){
        fseek(arqPlay, sizeof(estruturaPlaylist) + posPlay * sizeof(Playlist),SEEK_SET);
        fread(&p, sizeof(Playlist), 1, arqPlay);

        if(p.codigo == codigo){
            printf("        PLAYLIST: %s (Codigo: %d)       \n", p.titulo, p.codigo);
            printf("----------------------------------------\n");

            if(p.inicioFaixas == -1){
                printf("Playlist vazia.\n");
            }else{
                arqFaixa = fopen("faixas.bin", "rb");
                if(arqFaixa == NULL){
                    printf("Erro: arquivo.\n");
                    fclose(arqPlay);
                    return;
                }
                
                fseek(arqFaixa, sizeof(CabecalhoFaixa), SEEK_SET);
                posFaixa = p.inicioFaixas;

                while(posFaixa != -1){
                    Faixa f;
                    fseek(arqFaixa,posFaixa * sizeof(Faixa),SEEK_CUR);
                    fread(&f, sizeof(Faixa), 1, arqFaixa);

                    int posMusica = buscarMusica(f.codigoMusica);
                    if(posMusica != -1){
                        FILE *arqMus = fopen("musica.bin", "rb");
                        if(arqMus != NULL){
                            estruturaMusica cabMus;
                            Musica m;
                            fread(&cabMus, sizeof(estruturaMusica), 1, arqMus);
                            fseek(arqMus,sizeof(estruturaMusica) + posMusica * sizeof(Musica),SEEK_SET);
                            fread(&m, sizeof(Musica), 1, arqMus);
                            printf("%d. %s - %s (Ano: %d)\n", 
                                   contador++, m.titulo, m.artista, m.ano);
                            fclose(arqMus);
                        }
                    }else{
                        printf("%d. Música com codigo %d não encontrada no acervo.\n", contador++, f.codigoMusica);
                    }
                    posFaixa = f.prox;
                    fseek(arqFaixa, sizeof(CabecalhoFaixa), SEEK_SET);
                }
                fclose(arqFaixa);
            }
            fclose(arqPlay);
            return;
        }
        posPlay = p.prox;
    }
    printf("Playlist com codigo %d não encontrada.\n", codigo);
    fclose(arqPlay);
}

/* Objetivo: criar nova playlist.
 * Pré-condição: código da playlist>0 e tamanho do titulo<=50.
 * Pós-condição: playlist criada.*/
void criarPlaylist(int codigo, char titulo[]){
    FILE *fplaylist;
    estruturaPlaylist cab;
    Playlist p, aux;
    int novaPos, atual, anterior;

    fplaylist = fopen("playlist.bin", "r+b");
    if (fplaylist == NULL){
        fplaylist = fopen("playlist.bin", "w+b");
        cab.cabeca = -1;
        cab.topo = 0;
        fwrite(&cab, sizeof(estruturaPlaylist), 1, fplaylist);
        fflush(fplaylist);
    }else{
        fread(&cab, sizeof(estruturaPlaylist), 1, fplaylist);
    }

    if (buscarPlaylist(codigo) != -1){
        printf("Código de playlist já existe. Não é possível cadastrar.\n");
        fclose(fplaylist);
        return;
    }

    p.codigo = codigo;
    strcpy(p.titulo, titulo);

    p.inicioFaixas = -1;
    p.fimFaixas = -1;
    p.prox = -1;

    novaPos = cab.topo;

    fseek(fplaylist, sizeof(estruturaPlaylist) + novaPos * sizeof(Playlist), SEEK_SET);
    fwrite(&p, sizeof(Playlist), 1, fplaylist);

    if (cab.cabeca == -1){
        cab.cabeca = novaPos;
    }else{
        atual = cab.cabeca;
        anterior = -1;
        
        // Percorre até encontrar o último elemento (prox == -1)
        while (atual != -1){
            anterior = atual;
            fseek(fplaylist, sizeof(estruturaPlaylist) + atual * sizeof(Playlist), SEEK_SET);
            fread(&aux, sizeof(Playlist), 1, fplaylist);
            atual = aux.prox;
        }
        
        // 'anterior' contém a posição do último elemento
        // 'aux' contém o último elemento da lista
        aux.prox = novaPos;
        
        // Escreve o último elemento atualizado de volta no arquivo
        fseek(fplaylist, sizeof(estruturaPlaylist) + anterior * sizeof(Playlist), SEEK_SET);
        fwrite(&aux, sizeof(Playlist), 1, fplaylist);
    }

    cab.topo++;

    fseek(fplaylist, 0, SEEK_SET);
    fwrite(&cab, sizeof(estruturaPlaylist), 1, fplaylist);

    fclose(fplaylist);
    printf("Playlist criada com sucesso!\n");
}

int buscarPlaylist(int codigo){
    FILE *arq;
    estruturaPlaylist cab;
    Playlist p;
    int pos;

    arq = fopen("playlist.bin", "rb");
    if (arq == NULL)
        return -1;

    fread(&cab, sizeof(estruturaPlaylist), 1, arq);

    pos = cab.cabeca;

    while (pos != -1){
        fseek(arq, sizeof(estruturaPlaylist) + pos * sizeof(Playlist), SEEK_SET);
        fread(&p, sizeof(Playlist), 1, arq);

        if (p.codigo == codigo){
            fclose(arq);
            return pos;
        }

        pos = p.prox;
    }

    fclose(arq);

    return -1;
}

void listarPlaylists(){
    FILE *arq;
    estruturaPlaylist cab;
    Playlist p;
    int pos;

    arq = fopen("playlist.bin", "rb");

    if (arq == NULL){
        printf("Erro: arquivo.\n");
        return;
    }

    fread(&cab, sizeof(estruturaPlaylist), 1, arq);

    if (cab.cabeca == -1){
        printf("Nenhuma playlist cadastrada.\n");
        fclose(arq);
        return;
    }

    printf("\n              PLAYLISTS               \n");

    pos = cab.cabeca;

    while (pos != -1){
        fseek(arq,sizeof(estruturaPlaylist) + pos * sizeof(Playlist),SEEK_SET);
        fread(&p, sizeof(Playlist), 1, arq);

        printf("Código: %d\n", p.codigo);
        printf("Título: %s\n", p.titulo);
        printf("-------------------\n");

        pos = p.prox;
    }

    fclose(arq);
}