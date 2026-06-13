#ifndef MUSICA_H
#define MUSICA_H

typedef struct{
    int cabeca;
    int topo;
    int nolivre;
} estruturaMusica;

typedef struct{
    int codigo;
    char titulo[51];
    char artista[51];
    int ano;
    int prox;
} Musica;

void cadastrarMusica(int codigo, char titulo[], char artista[], int ano);
int buscarMusica(int codigo);
void imprimeDadosMusica(int codigo);
void listarAcervo();

#endif