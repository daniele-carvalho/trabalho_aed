### TRABALHO ALGORITMO E ESTRUTURA DE DADOS.
## Implementação de lista encadeada em arquivo binário.

## Identificação:

- Disciplina: Algoritmo e Estrutura de Dados.
- Alunas: Daniele Neri de Carvalho e Maria Gabriela Valiati.
- Entrega: 15/06/2026.

## Descrição do projeto: 

- Sistema de gerenciamento de playlists de músicas usando listas encadeadas e arquivo binário.

## Como compilar e executar

- Compilação direta: 

<p>gcc src/main.c src/musica.c src/playlist.c src/faixa.c src/arquivo.c</p>

---
- Executar 

<p>Linux: ./a.out</p>
<p>Windows: ./a.exe</p>

---

## Como utilizar as funções do menu:

- 01 - Cadastrar nova música.
<p>Função que faz o cadastro completo de uma música no acervo, digitar todos os dados pedidos, tais como código, título, artista e ano.</p>

- 02 - Imprimir dados da música.
<p>Função que imprime os dados da música desejada, basta escrever o código da música.</p>

- 03 - Listar acervo.
<p>Função que imprime todas as músicas cadastradas no acervo. Não precisa de entrada além da escolha no menu. </p>

- 04 - Criar uma playlist.
<p>Função que cria uma nova playlist. Basta digitar o código da playlist e o título. </p>

- 05 - Adicionar uma música do acervo no final da playlist.
<p>Função que adiciona música existente no acervo no final da playlist existente desejada. Basta digitar o código da playlist e o código da música. </p>

- 06 - Adicionar uma música do acervo no início da playlist.
<p>Função que adiciona uma música existente do acervo no início da playlist existente desejada. Basta digitar o código da playlist e o código da música.</p>

- 07 - Remover uma música da playlist.
<p>Função que remove uma música de uma playlist existente. Basta digitar o código da música e o código da playlist. </p>

- 08 - Imprimir as músicas de uma playlist.
<p>Função que imprime as músicas da playlist escolhida. Digitar o código da playlist. </p>

- 09 - Imprimir a lista de playlists.
<p>Função que imprime a lista de playlists existentes. Não é necessário colocar nenhuma entrada.</p>

- 10 - Carregar arquivo.
<p>Função que a partir da entrada de um arquivo "txt" existente e com operações definidas, carrega as operações em lote desse arquivo.</p>

- 11 - Imprimir nós livres.
<p>Função que acha nós livres a partir de músicas removidas.</p>
        
- 0 - Fechar menu.
<p>Quando o usuário digita 0, o programa finaliza.</p>

## Arquivos binários do programa.

- musica.bin
- playlist.bin
- faixa.bin