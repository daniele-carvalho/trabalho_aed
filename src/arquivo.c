#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../includes/arquivo.h"
#include "../includes/musica.h"
#include "../includes/playlist.h"
#include "../includes/faixa.h"

/* Propósito: remove espaços em branco do início e fim de uma string.
 * Pré-condição: string válida.
 * Pós-condição: string sem espaços extras*/
void trim(char *str){
    if (str == NULL) return;
    char *inicio = str;
    char *fim = str + strlen(str) - 1;    
    while(isspace((unsigned char)*inicio)) inicio++; //remove espacos do inicio

    if(inicio != str){
        memmove(str, inicio, strlen(inicio) + 1);
    }
    
    fim = str + strlen(str) - 1;
    while(fim > str && isspace((unsigned char)*fim)) fim--; //remove espacos do fim
    *(fim + 1) = '\0';
}

/* Propósito: carregar e executar operações em lote de um arquivo texto.
 * Pré-condição: arquivo existe e está no formato correto (txt).
 * Pós-condição: operações executadas conforme arquivo*/
void carregarArquivo(char *nomeArquivo){
    FILE *arquivo;
    char linha[300];
    int numLinha = 0;
    int operacoesRealizadas = 0;

    arquivo = fopen(nomeArquivo, "r");
    
    if(arquivo == NULL){
        printf("Erro: arquivo '%s'\n", nomeArquivo);
        return;
    }

    printf("\n              PROCESSANDO ARQUIVO '%s'                \n", nomeArquivo);

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        numLinha++;
        
        linha[strcspn(linha, "\n")] = '\0';

        if(strlen(linha) == 0) continue; //pula linhas vazias
        
        char *token = strtok(linha, ";");
        if(token == NULL) continue;
        
        trim(token);
        
        // comando m
        if(strcmp(token, "M") == 0){
            char *codigoStr = strtok(NULL, ";");
            char *titulo = strtok(NULL, ";");
            char *artista = strtok(NULL, ";");
            char *anoStr = strtok(NULL, ";");
            
            if(codigoStr && titulo && artista && anoStr){
                trim(codigoStr);
                trim(titulo);
                trim(artista);
                trim(anoStr);
                
                int codigo = atoi(codigoStr);
                int ano = atoi(anoStr);
                
                printf("Linha %d: cadastrando música '%s' (Cod: %d)...\n", 
                       numLinha, titulo, codigo);
                cadastrarMusica(codigo, titulo, artista, ano);
                operacoesRealizadas++;
            }else{
                printf("Linha %d: formato inválido para cadastro de música\n", numLinha);
            }
        }
        // comando p
        else if(strcmp(token, "P") == 0){
            char *codigoStr = strtok(NULL, ";");
            char *titulo = strtok(NULL, ";");
            
            if(codigoStr && titulo){
                trim(codigoStr);
                trim(titulo);
                
                int codigo = atoi(codigoStr);
                
                printf("Linha %d: criando playlist '%s' (Cod: %d)...\n", 
                       numLinha, titulo, codigo);
                criarPlaylist(codigo, titulo);
                operacoesRealizadas++;
            }else{
                printf("Linha %d: formato inválido para criação de playlist\n", numLinha);
            }
        }else if(strcmp(token, "I") == 0){
            char *posicao = strtok(NULL, ";");
            char *playlistStr = strtok(NULL, ";");
            char *musicaStr = strtok(NULL, ";");
            
            if(posicao && playlistStr && musicaStr){
                trim(posicao);
                trim(playlistStr);
                trim(musicaStr);
                
                int codPlaylist = atoi(playlistStr);
                int codMusica = atoi(musicaStr);
                
                if(strcmp(posicao, "I") == 0){
                    printf("Linha %d: Inserindo música %d no início da playlist %d...\n", 
                           numLinha, codMusica, codPlaylist);
                    adicionarMusicaInicio(codPlaylist, codMusica);
                }else if(strcmp(posicao, "F") == 0){
                    printf("Linha %d: Inserindo música %d no fim da playlist %d...\n", 
                           numLinha, codMusica, codPlaylist);
                    adicionarMusicaFim(codPlaylist, codMusica);
                }else{
                    printf("Linha %d: Posicao inválida (use I ou F)\n", numLinha);
                }
                operacoesRealizadas++;
            }else{
                printf("Linha %d: Formato inválido para inserção na playlist\n", numLinha);
            }
        }else if(strcmp(token, "R") == 0){
            char *playlistStr = strtok(NULL, ";");
            char *musicaStr = strtok(NULL, ";");
            
            if(playlistStr && musicaStr){
                trim(playlistStr);
                trim(musicaStr);
                
                int codPlaylist = atoi(playlistStr);
                int codMusica = atoi(musicaStr);
                
                printf("Linha %d: Removendo música %d da playlist %d...\n", 
                       numLinha, codMusica, codPlaylist);
                removerMusicaPlaylist(codPlaylist, codMusica);
                operacoesRealizadas++;
            }else{
                printf("Linha %d: Formato inválido para remoção da playlist\n", numLinha);
            }
        }else{
            printf("Linha %d: Comando desconhecido '%s'\n", numLinha, token);
        }
    }
    
    fclose(arquivo);
    
    printf("\n              PROCESSAMENTO CONCLUIDO             \n");
    printf("Total de linhas processadas: %d\n", numLinha);
    printf("Operacoes realizadas: %d\n", operacoesRealizadas);
}