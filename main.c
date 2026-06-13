#include <stdio.h>
#include <string.h>
#include "musica.h"

int main(){
    int menu;

    do
    {
        printf("\nSeja bem vindo.\n");
        printf("01 - Cadastrar uma nova música\n");
        printf("02 - Imprimir dados da música\n");
        printf("03 - Listar acervo\n");
        printf("04 - Criar uma playlist\n");
        printf("05 - Adicionar uma musica do acervo no final da playlist\n");
        printf("06 - Adicionar uma musica do acervo no inicio da playlist\n");
        printf("07 - Remover uma música da playlist\n");
        printf("08 - Imprimir as músicas de uma playlist\n");
        printf("09 - Imprimir a lista de playlist\n");
        printf("10 - Carregar arquivo\n");
        printf("11 - Imprimir nós livres\n");
        printf("00 - Fechar menu\n");

        scanf("%d", &menu);
        scanf("%*c");

        switch (menu)
        {
        case 1:
        {
            int codigo, ano;
            char titulo[51], artista[51];

            printf("Digite o código da música.\n");
            scanf("%d", &codigo);
            scanf("%*c");

            printf("Digite o titulo da música.\n");
            fgets(titulo, sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';

            printf("Digite o artista.\n");
            fgets(artista, sizeof(artista), stdin);
            artista[strcspn(artista, "\n")] = '\0';

            printf("Digite o ano de lançamento da música.\n");
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
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            printf("\nPressione ENTER para continuar...");
            getchar();
            break;

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