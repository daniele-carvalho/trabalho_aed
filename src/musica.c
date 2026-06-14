#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/musica.h"

void cadastrarMusica(int codigo, char titulo[], char artista[], int ano){
    FILE *fmusica; 
    estruturaMusica cab; 
    Musica m, aux; //aux para percorrer a lista.
    int novaPos, atual; 

    fmusica = fopen("musica.bin", "r+b"); //modo leitura e escrita

    if (fmusica == NULL){
        fmusica = fopen("musica.bin", "w+b"); //modo escrita
        cab.cabeca = -1;
        cab.topo = 0;
        cab.nolivre = -1;

        fwrite(&cab, sizeof(estruturaMusica), 1, fmusica);
        fflush(fmusica);
    } else {
        fread(&cab, sizeof(estruturaMusica), 1, fmusica);
    }

    if (buscarMusica(codigo) != -1){
        printf("Musica ja cadastrada.\n");
        fclose(fmusica);
        return;
    }

    m.codigo = codigo;
    strcpy(m.titulo, titulo);
    strcpy(m.artista, artista);
    m.ano = ano;
    m.prox = -1;

    novaPos = cab.topo;

    // grava a nova música no final do arquivo 
    fseek(fmusica, sizeof(estruturaMusica) + novaPos * sizeof(Musica),SEEK_SET);
    fwrite(&m, sizeof(Musica), 1, fmusica);

    /* lista vazia */
    if (cab.cabeca == -1){
        cab.cabeca = novaPos;
    }else{
        atual = cab.cabeca;

        while (1){
            fseek(fmusica,sizeof(estruturaMusica) + atual * sizeof(Musica),SEEK_SET);
            fread(&aux, sizeof(Musica), 1, fmusica);

            if (aux.prox == -1)
                break;

            atual = aux.prox;
        }

        aux.prox = novaPos;

        fseek(fmusica,sizeof(estruturaMusica) + atual * sizeof(Musica),SEEK_SET);
        fwrite(&aux, sizeof(Musica), 1, fmusica);
    }

    cab.topo++;

    fseek(fmusica, 0, SEEK_SET);
    fwrite(&cab, sizeof(estruturaMusica), 1, fmusica);
    fclose(fmusica);
}

int buscarMusica(int codigo){
    FILE *arq;
    estruturaMusica cab;
    Musica m;
    int pos;

    arq = fopen("musica.bin", "rb");

    if (arq == NULL)
        return -1;

    fread(&cab, sizeof(estruturaMusica), 1, arq);

    pos = cab.cabeca;

    while (pos != -1){
        fseek(arq,sizeof(estruturaMusica) + pos * sizeof(Musica),SEEK_SET);

        fread(&m, sizeof(Musica), 1, arq);

        if (m.codigo == codigo){
            fclose(arq);
            return pos;
        }

        pos = m.prox;
    }
    fclose(arq);
    return -1;
}

void imprimeDadosMusica(int codigo){
    FILE *arq;
    Musica m;
    int pos;

    pos = buscarMusica(codigo);

    if (pos == -1){
        printf("Música não encontrada.\n");
        return;
    }

    arq = fopen("musica.bin", "rb");

    if (arq == NULL){
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fseek(arq,sizeof(estruturaMusica) + pos * sizeof(Musica),SEEK_SET);
    fread(&m, sizeof(Musica), 1, arq);

    printf("\n");
    printf("Código : %d\n", m.codigo);
    printf("Título : %s\n", m.titulo);
    printf("Artista: %s\n", m.artista);
    printf("Ano: %d\n", m.ano);

    fclose(arq);
}

void listarAcervo(){
    FILE *arq;
    estruturaMusica cab;
    Musica m;
    int pos;

    arq = fopen("musica.bin", "rb");

    if (arq == NULL){
        printf("Arquivo nao encontrado.\n");
        return;
    }

    fread(&cab, sizeof(estruturaMusica), 1, arq);

    if (cab.cabeca == -1){
        printf("Acervo vazio.\n");
        fclose(arq);
        return;
    }

    printf("\n===== ACERVO DE MUSICAS =====\n");

    pos = cab.cabeca;

    while (pos != -1){
        fseek(arq,sizeof(estruturaMusica) + pos * sizeof(Musica),SEEK_SET);
        fread(&m, sizeof(Musica), 1, arq);

        printf("\nCodigo : %d\n", m.codigo);
        printf("Titulo : %s\n", m.titulo);
        printf("Artista: %s\n", m.artista);
        printf("Ano    : %d\n", m.ano);
        printf("----------------------------\n");

        pos = m.prox;
    }

    fclose(arq);
}