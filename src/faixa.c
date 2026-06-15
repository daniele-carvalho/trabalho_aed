#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/faixa.h"
#include "../includes/playlist.h"
#include "../includes/musica.h"

/*Objetivo: obter posição livre no arquivo de faixas.
 Pré condição: arquivo aberto em modo leitura/escrita e cabeçalho inicializado
 Pós condição: atualiza lista de nós livres ou incrementa topo na memória */
int obterPosicaoLivre(FILE *arq, CabecalhoFaixa *cab)
{
    int pos;
    Faixa f;

    if(cab->livres == -1){
        pos = cab->topo;
        cab->topo++;
        return pos;
    }

    pos = cab->livres;

    fseek(arq,sizeof(CabecalhoFaixa) + pos * sizeof(Faixa),SEEK_SET);

    fread(&f, sizeof(Faixa), 1, arq);

    cab->livres = f.prox;

    return pos;
}

void adicionarMusicaInicio(int codPlaylist, int codMusica){
    FILE *fFaixa;
    FILE *fPlay;

    CabecalhoFaixa cab;
    Playlist p;
    Faixa nova, atual;
    int posPlaylist;
    int posNova;
    int encontrou = 0;

    if(buscarMusica(codMusica) == -1){
        printf("Musica nao encontrada no acervo.\n");
        return;
    }

    posPlaylist = buscarPlaylist(codPlaylist);

    if(posPlaylist == -1){
        printf("Playlist nao encontrada.\n");
        return;
    }

    fFaixa = fopen("faixas.bin","r+b");

    if(fFaixa == NULL){
        fFaixa = fopen("faixas.bin","w+b");

        cab.topo = 0;
        cab.livres = -1;

        fwrite(&cab,sizeof(CabecalhoFaixa),1,fFaixa);
    }else{
        fread(&cab,sizeof(CabecalhoFaixa),1,fFaixa);
    }

    fPlay = fopen("playlist.bin","r+b");

    fseek(fPlay,sizeof(estruturaPlaylist) + posPlaylist * sizeof(Playlist),SEEK_SET);

    fread(&p,sizeof(Playlist),1,fPlay);

    // Verificar se a música já está na playlist
    int posAtual = p.inicioFaixas;
    while(posAtual != -1 && !encontrou){
        fseek(fFaixa,sizeof(CabecalhoFaixa) + posAtual * sizeof(Faixa),SEEK_SET);
        fread(&atual, sizeof(Faixa), 1, fFaixa);
        
        if(atual.codigoMusica == codMusica){
            encontrou = 1;
        }
        posAtual = atual.prox;
    }

    if(encontrou){
        printf("Musica ja existe na playlist.\n");
        fclose(fPlay);
        fclose(fFaixa);
        return;
    }

    posNova = obterPosicaoLivre(fFaixa, &cab);

    nova.codigoMusica = codMusica;
    nova.prox = p.inicioFaixas;

    fseek(fFaixa,sizeof(CabecalhoFaixa) + posNova * sizeof(Faixa),SEEK_SET);
    fwrite(&nova,sizeof(Faixa),1,fFaixa);

    p.inicioFaixas = posNova;

    if(p.fimFaixas == -1)
        p.fimFaixas = posNova;

    fseek(fPlay,sizeof(estruturaPlaylist) + posPlaylist * sizeof(Playlist),SEEK_SET);
    fwrite(&p,sizeof(Playlist),1,fPlay);

    fseek(fFaixa,0,SEEK_SET);
    fwrite(&cab,sizeof(CabecalhoFaixa),1,fFaixa);

    fclose(fPlay);
    fclose(fFaixa);

    printf("Musica inserida no inicio da playlist.\n");
}

/*Objetivo: adicionar uma música no fim de uma playlist..
 Pré condição: adicionar códigos inteiros maiores que 0.
 Pós condição: música adicionada na playlist. */
void adicionarMusicaFim(int codPlaylist, int codMusica){
    FILE *fFaixa;
    FILE *fPlay;
    CabecalhoFaixa cab;
    Playlist p;
    Faixa nova, atual;
    int posPlaylist;
    int posNova;
    int encontrou = 0;

    if(buscarMusica(codMusica) == -1){
        printf("Musica nao encontrada no acervo.\n");
        return;
    }
    posPlaylist = buscarPlaylist(codPlaylist);

    if(posPlaylist == -1){
        printf("Playlist nao encontrada.\n");
        return;
    }
    fFaixa = fopen("faixas.bin","r+b");

    if(fFaixa == NULL){
        fFaixa = fopen("faixas.bin","w+b");

        cab.topo = 0;
        cab.livres = -1;

        fwrite(&cab,sizeof(CabecalhoFaixa),1,fFaixa);
    }else{
        fread(&cab,sizeof(CabecalhoFaixa),1,fFaixa);
    }
    fPlay = fopen("playlist.bin","r+b");

    fseek(fPlay,sizeof(estruturaPlaylist) + posPlaylist * sizeof(Playlist),SEEK_SET);
    fread(&p,sizeof(Playlist),1,fPlay);

    // Verificar se a música já está na playlist
    int posAtual = p.inicioFaixas;
    while(posAtual != -1 && !encontrou){
        fseek(fFaixa,
              sizeof(CabecalhoFaixa) + posAtual * sizeof(Faixa),SEEK_SET);
        fread(&atual, sizeof(Faixa), 1, fFaixa);
        
        if(atual.codigoMusica == codMusica){
            encontrou = 1;
        }
        posAtual = atual.prox;
    }

    if(encontrou){
        printf("Musica ja existe na playlist.\n");
        fclose(fPlay);
        fclose(fFaixa);
        return;
    }

    posNova = obterPosicaoLivre(fFaixa, &cab);

    nova.codigoMusica = codMusica;
    nova.prox = -1;

    fseek(fFaixa,sizeof(CabecalhoFaixa) + posNova * sizeof(Faixa),SEEK_SET);
    fwrite(&nova,sizeof(Faixa),1,fFaixa);

    if(p.inicioFaixas == -1){
        p.inicioFaixas = posNova;
        p.fimFaixas = posNova;
    }else{
        fseek(fFaixa,sizeof(CabecalhoFaixa) + p.fimFaixas * sizeof(Faixa),SEEK_SET);
        fread(&nova,sizeof(Faixa),1,fFaixa);

        nova.prox = posNova;

        fseek(fFaixa,sizeof(CabecalhoFaixa) + p.fimFaixas * sizeof(Faixa),SEEK_SET);
        fwrite(&nova,sizeof(Faixa),1,fFaixa);

        p.fimFaixas = posNova;
    }

    fseek(fPlay,sizeof(estruturaPlaylist) + posPlaylist * sizeof(Playlist),SEEK_SET);
    fwrite(&p,sizeof(Playlist),1,fPlay);

    fseek(fFaixa,0,SEEK_SET);
    fwrite(&cab,sizeof(CabecalhoFaixa),1,fFaixa);

    fclose(fPlay);
    fclose(fFaixa);

    printf("Musica inserida no fim da playlist.\n");
}

/*Objetivo: remover uma música de uma playlist específica.
 Pré condição: código de playlist e de música maior que 0.
 Pós condição: remove a música da playlist.*/
void removerMusicaPlaylist(int codPlaylist, int codMusica){
    FILE *fFaixa;
    FILE *fPlay;
    CabecalhoFaixa cab;
    Playlist p;
    Faixa atual, anterior;
    int posPlaylist;
    int posAtual;
    int posAnterior = -1;
    int encontrou = 0;

    posPlaylist = buscarPlaylist(codPlaylist);

    if(posPlaylist == -1){
        printf("Playlist nao encontrada.\n");
        return;
    }

    fFaixa = fopen("faixas.bin","r+b");
    if(fFaixa == NULL){
        printf("Arquivo de faixas nao encontrado.\n");
        return;
    }

    fread(&cab, sizeof(CabecalhoFaixa), 1, fFaixa);

    fPlay = fopen("playlist.bin","r+b");

    fseek(fPlay,sizeof(estruturaPlaylist) + posPlaylist * sizeof(Playlist),SEEK_SET);
    fread(&p, sizeof(Playlist), 1, fPlay);

    if(p.inicioFaixas == -1){
        printf("Playlist vazia.\n");
        fclose(fPlay);
        fclose(fFaixa);
        return;
    }

    // Buscar a música na playlist
    posAtual = p.inicioFaixas;

    while(posAtual != -1 && !encontrou){
        fseek(fFaixa,sizeof(CabecalhoFaixa) + posAtual * sizeof(Faixa),SEEK_SET);
        fread(&atual, sizeof(Faixa), 1, fFaixa);
        
        if(atual.codigoMusica == codMusica){
            encontrou = 1;
        }else{
            posAnterior = posAtual;
            posAtual = atual.prox;
        }
    }

    if(!encontrou){
        printf("Musica nao encontrada na playlist.\n");
        fclose(fPlay);
        fclose(fFaixa);
        return;
    }
    // Remover o nó da lista
    if(posAnterior == -1){
        // Remover do início
        p.inicioFaixas = atual.prox;
        if(p.inicioFaixas == -1){
            p.fimFaixas = -1;
        }
    }else{
        // Remover do meio ou fim
        fseek(fFaixa,sizeof(CabecalhoFaixa) + posAnterior * sizeof(Faixa),SEEK_SET);
        fread(&anterior, sizeof(Faixa), 1, fFaixa);
        
        anterior.prox = atual.prox;
        
        fseek(fFaixa,sizeof(CabecalhoFaixa) + posAnterior * sizeof(Faixa),SEEK_SET);
        fwrite(&anterior, sizeof(Faixa), 1, fFaixa);
        
        if(p.fimFaixas == posAtual){
            p.fimFaixas = posAnterior;
        }
    }

    // Adicionar nó removido à lista de livres
    atual.prox = cab.livres;
    
    fseek(fFaixa,sizeof(CabecalhoFaixa) + posAtual * sizeof(Faixa),SEEK_SET);
    fwrite(&atual, sizeof(Faixa), 1, fFaixa);
    
    cab.livres = posAtual;

    // Atualizar playlist
    fseek(fPlay,sizeof(estruturaPlaylist) + posPlaylist * sizeof(Playlist),SEEK_SET);
    fwrite(&p, sizeof(Playlist), 1, fPlay);

    // Atualizar cabeçalho das faixas
    fseek(fFaixa, 0, SEEK_SET);
    fwrite(&cab, sizeof(CabecalhoFaixa), 1, fFaixa);

    fclose(fPlay);
    fclose(fFaixa);

    printf("Musica removida da playlist com sucesso.\n");
}

/*Propósito: Imprime os nós livres do arquivo de faixas.
 * Pré-condição: Nenhuma.
 * Pós-condição: Posições livres impressas na tela*/
void imprimirNosLivres(){
    FILE *arq;
    CabecalhoFaixa cab;
    Faixa f;
    int pos;
    int count = 0;
    arq = fopen("faixas.bin","rb");

    if(arq == NULL){
        printf("Arquivo de faixas nao existe.\n");
        return;
    }

    fread(&cab,sizeof(CabecalhoFaixa),1,arq);

    pos = cab.livres;

    printf("\n===== NOS LIVRES =====\n");

    if(pos == -1){
        printf("Nenhum no livre disponivel.\n");
    }else{
        while(pos != -1){
            printf("Posicao: %d\n", pos);
            count++;

            fseek(arq,sizeof(CabecalhoFaixa) + pos * sizeof(Faixa),SEEK_SET);
            fread(&f, sizeof(Faixa), 1, arq);

            pos = f.prox;
        }
        printf("Total de nos livres: %d\n", count);
    }

    fclose(arq);
}