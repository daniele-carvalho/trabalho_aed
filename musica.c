#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "musica.h"

void cadastrarMusica(int codigo, char titulo[], char artista[], int ano){
    FILE *fmusica;
    estruturaMusica cab;
    Musica m;

    fmusica = fopen("musica.bin", "r+b");

    if (fmusica == NULL){
        fmusica = fopen("musica.bin", "w+b");

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

    m.prox = cab.cabeca;

    fseek(
        fmusica,
        sizeof(estruturaMusica) + cab.topo * sizeof(Musica),
        SEEK_SET);

    fwrite(&m, sizeof(Musica), 1, fmusica);

    cab.cabeca = cab.topo;
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