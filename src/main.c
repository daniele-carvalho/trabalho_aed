#include <stdio.h>
#include <string.h>
#include "../includes/musica.h"
#include "../includes/playlist.h"
#include "../includes/faixa.h"
#include "../includes/arquivo.h"

int main(){
    int menu;

    do
    {
        printf("\nSeja bem vindo. Escolha uma opção: \n");
        printf("01 - Cadastrar uma nova música.\n");
        printf("02 - Imprimir dados da música.\n");
        printf("03 - Listar acervo.\n");
        printf("04 - Criar uma playlist.\n");
        printf("05 - Adicionar uma musica do acervo no final da playlist.\n");
        printf("06 - Adicionar uma musica do acervo no inicio da playlist.\n");
        printf("07 - Remover uma música da playlist.\n");
        printf("08 - Imprimir as músicas de uma playlist.\n");
        printf("09 - Imprimir a lista de playlist.\n");
        printf("10 - Carregar arquivo.\n");
        printf("11 - Imprimir nós livres.\n");
        printf("0 - Fechar menu.\n");

        scanf("%d", &menu);
        scanf("%*c");

        switch (menu)
        {
        case 1:
        {
            int codigo, ano;
            char titulo[51], artista[51];

            printf("Código: \n");
            scanf("%d", &codigo);
            scanf("%*c");

            printf("Título: \n");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';

            printf("Artista: \n");
            fgets(artista, sizeof(artista), stdin);
            artista[strcspn(artista, "\n")] = '\0';

            printf("Ano: \n");
            scanf("%d", &ano);
            scanf("%*c");

            cadastrarMusica(codigo, titulo, artista, ano);

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }

        case 2:
        {
            int codigo;

            printf("Digite o código da música.\n");
            scanf("%d", &codigo);
            scanf("%*c");

            imprimeDadosMusica(codigo);

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }

        case 3:
        {
            listarAcervo();
        
            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }
        
        case 4:
        {
            int codigo;
            char titulo[51];

            printf("Digite o codigo da playlist.\n");
            scanf("%d", &codigo);
            scanf("%*c");

            printf("Digite o titulo da playlist.\n");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';

            criarPlaylist(codigo, titulo);

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }
        case 5:
        {
            int codPlaylist, codMusica;

            printf("Digite o codigo da playlist.\n");
            scanf("%d", &codPlaylist);

            printf("Digite o codigo da musica.\n");
            scanf("%d", &codMusica);
            scanf("%*c");

            adicionarMusicaFim(codPlaylist, codMusica);

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }

        case 6:
        {
            int codPlaylist, codMusica;

            printf("Digite o codigo da playlist.\n");
            scanf("%d", &codPlaylist);

            printf("Digite o codigo da musica.\n");
            scanf("%d", &codMusica);
            scanf("%*c");

            adicionarMusicaInicio(codPlaylist, codMusica);

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }

        case 7:
        {
            int codPlaylist, codMusica;

            printf("Digite o codigo da playlist.\n");
            scanf("%d", &codPlaylist);

            printf("Digite o codigo da musica.\n");
            scanf("%d", &codMusica);
            scanf("%*c");

            removerMusicaPlaylist(codPlaylist, codMusica);

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }
        case 8:
        { 
            int codigo;

            printf("Digite o codigo da playlist.\n");
            scanf("%d", &codigo);
            scanf("%*c");

            imprimirPlaylist(codigo);

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }
        case 9:        
        {
            listarPlaylists();

            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }
        case 10:
        {
            char nomeArquivo[50];
            
            printf("Digite o nome do arquivo de operacoes: ");
            fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
            nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';
            
            carregarArquivo(nomeArquivo);
            
            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }

        case 11:
        {
            imprimirNosLivres();
            
            printf("\nPressione ENTER para continuar...");
            getchar();
            break;
        }
        case 0:
            printf("Encerrando...\n");
            break;

        default:
            printf("Opção inválida.\n");

            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (menu != 0);

    return 0;
}