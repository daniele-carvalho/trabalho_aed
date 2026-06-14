#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arquivo.h"
#include "musica.h"
#include "playlist.h"
#include "faixa.h"

/*
 * Propósito: Remove espaços em branco do início e fim de uma string
 * Pré-condição: String válida
 * Pós-condição: String sem espaços extras
 */
void trim(char *str)
{
    if (str == NULL) return;
    
    char *inicio = str;
    char *fim = str + strlen(str) - 1;
    
    // Remove espaços do início
    while(isspace((unsigned char)*inicio)) inicio++;
    
    if(inicio != str)
    {
        memmove(str, inicio, strlen(inicio) + 1);
    }
    
    // Remove espaços do fim
    fim = str + strlen(str) - 1;
    while(fim > str && isspace((unsigned char)*fim)) fim--;
    
    *(fim + 1) = '\0';
}

/*
 * Propósito: Carrega e executa operações em lote de um arquivo texto
 * Pré-condição: Arquivo existe e está no formato correto
 * Pós-condição: Operações executadas conforme arquivo
 */
void carregarArquivo(char *nomeArquivo)
{
    FILE *arquivo;
    char linha[300];
    int numLinha = 0;
    int operacoesRealizadas = 0;

    arquivo = fopen(nomeArquivo, "r");
    
    if(arquivo == NULL)
    {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'\n", nomeArquivo);
        return;
    }

    printf("\n===== PROCESSANDO ARQUIVO '%s' =====\n", nomeArquivo);

    while(fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        numLinha++;
        
        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = '\0';
        
        // Pular linhas vazias
        if(strlen(linha) == 0) continue;
        
        char *token = strtok(linha, ";");
        if(token == NULL) continue;
        
        trim(token);
        
        // Comando M: Cadastrar música
        if(strcmp(token, "M") == 0)
        {
            char *codigoStr = strtok(NULL, ";");
            char *titulo = strtok(NULL, ";");
            char *artista = strtok(NULL, ";");
            char *anoStr = strtok(NULL, ";");
            
            if(codigoStr && titulo && artista && anoStr)
            {
                trim(codigoStr);
                trim(titulo);
                trim(artista);
                trim(anoStr);
                
                int codigo = atoi(codigoStr);
                int ano = atoi(anoStr);
                
                printf("Linha %d: Cadastrando musica '%s' (Cod: %d)...\n", 
                       numLinha, titulo, codigo);
                cadastrarMusica(codigo, titulo, artista, ano);
                operacoesRealizadas++;
            }
            else
            {
                printf("Linha %d: Formato invalido para cadastro de musica\n", numLinha);
            }
        }
        // Comando P: Criar playlist
        else if(strcmp(token, "P") == 0)
        {
            char *codigoStr = strtok(NULL, ";");
            char *titulo = strtok(NULL, ";");
            
            if(codigoStr && titulo)
            {
                trim(codigoStr);
                trim(titulo);
                
                int codigo = atoi(codigoStr);
                
                printf("Linha %d: Criando playlist '%s' (Cod: %d)...\n", 
                       numLinha, titulo, codigo);
                criarPlaylist(codigo, titulo);
                operacoesRealizadas++;
            }
            else
            {
                printf("Linha %d: Formato invalido para criacao de playlist\n", numLinha);
            }
        }
        // Comando I: Inserir música na playlist
        else if(strcmp(token, "I") == 0)
        {
            char *posicao = strtok(NULL, ";");
            char *playlistStr = strtok(NULL, ";");
            char *musicaStr = strtok(NULL, ";");
            
            if(posicao && playlistStr && musicaStr)
            {
                trim(posicao);
                trim(playlistStr);
                trim(musicaStr);
                
                int codPlaylist = atoi(playlistStr);
                int codMusica = atoi(musicaStr);
                
                if(strcmp(posicao, "I") == 0)
                {
                    printf("Linha %d: Inserindo musica %d no inicio da playlist %d...\n", 
                           numLinha, codMusica, codPlaylist);
                    adicionarMusicaInicio(codPlaylist, codMusica);
                }
                else if(strcmp(posicao, "F") == 0)
                {
                    printf("Linha %d: Inserindo musica %d no fim da playlist %d...\n", 
                           numLinha, codMusica, codPlaylist);
                    adicionarMusicaFim(codPlaylist, codMusica);
                }
                else
                {
                    printf("Linha %d: Posicao invalida (use I ou F)\n", numLinha);
                }
                operacoesRealizadas++;
            }
            else
            {
                printf("Linha %d: Formato invalido para insercao na playlist\n", numLinha);
            }
        }
        // Comando R: Remover música da playlist
        else if(strcmp(token, "R") == 0)
        {
            char *playlistStr = strtok(NULL, ";");
            char *musicaStr = strtok(NULL, ";");
            
            if(playlistStr && musicaStr)
            {
                trim(playlistStr);
                trim(musicaStr);
                
                int codPlaylist = atoi(playlistStr);
                int codMusica = atoi(musicaStr);
                
                printf("Linha %d: Removendo musica %d da playlist %d...\n", 
                       numLinha, codMusica, codPlaylist);
                removerMusicaPlaylist(codPlaylist, codMusica);
                operacoesRealizadas++;
            }
            else
            {
                printf("Linha %d: Formato invalido para remocao da playlist\n", numLinha);
            }
        }
        else
        {
            printf("Linha %d: Comando desconhecido '%s'\n", numLinha, token);
        }
    }
    
    fclose(arquivo);
    
    printf("\n===== PROCESSAMENTO CONCLUIDO =====\n");
    printf("Total de linhas processadas: %d\n", numLinha);
    printf("Operacoes realizadas: %d\n", operacoesRealizadas);
}