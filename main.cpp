#include <stdio.h>
#include "lab.h"
#include <stdlib.h>
#include <string.h>

// struct para o produto com as informações
struct Produto {
    int quant;
    float preco;
    char name[100];
} produto;



// função default de início
void inicio() {
    // As funções abaixo estão presumidamente no lab.h
    setFullScreen(true);
    setColor(MY_COLOR_WHITE, MY_COLOR_DARK_BLUE);
    system("cls");
    showRectAt(40, 10, 30, 10);
    gotoxy(45, 15);
    printf("      STOCKSAGE \n");
    gotoxy(10, 22);
    system("pause");
}



// função para abrir um ficheiro e armazenar os dados da struct Produto
void armazenarProduto(struct Produto p) {
    FILE* file = NULL;
    errno_t err;  // Variável para armazenar o código de erro

    err = fopen_s(&file, "produtos.txt", "a");

    // Verificar se houve erro ao abrir o .txt
    if (err != 0 || file == NULL) {
        printf("Erro ao abrir o ficheiro! Código de erro: %d\n", err);
        return;
    }

    // Escreve no .txt
    fprintf(file, "Produto: %s\n", p.name);
    fprintf(file, "Quantidade: %d\n", p.quant);
    fprintf(file, "Preço: %.2f €\n", p.preco);
    fprintf(file, "---------------------------\n");

    // Fechar o file .txt
    fclose(file);
    printf("Dados armazenados com sucesso!\n");
}



// função para adicionar produto
void addProduto() {
    // Inicializar os valores da struct
    produto.quant = 0;
    produto.preco = 0;

    printf("Indique o nome do produto que deseja adicionar:\n");
    scanf_s("%s", produto.name, (unsigned)_countof(produto.name));

    printf("Indique a sua quantidade atual em stock:\n");
    scanf_s("%d", &produto.quant);

    printf("Indique o seu preço:\n(Escrever o valor com vírgula.Ex:1,5)\n");
    scanf_s("%f", &produto.preco);

    armazenarProduto(produto);
}



//funcao de limpar o .txt 
void limparStock() {
    FILE* file = NULL;
    errno_t err;

    err = fopen_s(&file, "produtos.txt", "w");

    // Verificar se houve erro ao abrir o .txt
    if (err != 0 || file == NULL) {
        printf("Erro ao abrir o ficheiro! Código de erro: %d\n", err);
        return;
    }

    // Fechar o ficheiro imediatamente após a abertura em modo "w" apaga o conteúdo
    fclose(file);
    printf("Stock limpo com sucesso!\n");
}



// função para ver o stock total
void verStock() {
    FILE* file = NULL;
    errno_t err;
    char linha[200];  // Buffer para armazenar cada linha do ficheiro

    err = fopen_s(&file, "produtos.txt", "r");

    if (err != 0 || file == NULL) {
        printf("Erro ao abrir o ficheiro! Código de erro: %d\n", err);
        return;
    }

    printf("---- Stock Atual ----\n");
    while (fgets(linha, sizeof(linha), file) != NULL) {
        printf("%s", linha);
    }

    fclose(file);
    printf("Stock aberto com sucesso!\n");
}



//função para procurar no stock
void procurarStock() {
    char procura[100];  // Variável para o nome do produto que será procurado
    FILE* file = NULL;
    errno_t err;
    char linha[200];    // Buffer para armazenar cada linha do ficheiro
    int encontrado = 0; // Variável para marcar se encontrou o produto

    printf("Digite o nome do produto que deseja procurar: ");
    scanf_s("%s", procura, (unsigned)_countof(procura));

    err = fopen_s(&file, "produtos.txt", "r");

    if (err != 0 || file == NULL) {
        printf("Erro ao abrir o ficheiro! Código de erro: %d\n", err);
        return;
    }

    // Percorre o ficheiro linha por linha
    while (fgets(linha, sizeof(linha), file) != NULL) {
        // Verifica se a linha contém o nome do produto procurado
        if (strstr(linha, procura) != NULL) {
            printf("%s", linha);  // Imprime a linha que contém o produto
            encontrado = 1;       // Marca que o produto foi encontrado
            // Continua a imprimir as linhas seguintes, que serão a quantidade e o preço
            for (int i = 0; i < 2; i++) {
                if (fgets(linha, sizeof(linha), file) != NULL) {
                    printf("%s", linha);
                }
            }
            break;
        }
    }

    if (!encontrado) {
        printf("Produto '%s' não encontrado no stock.\n", procura);
    }

    fclose(file);
}



//funçao para remover um item do stock
void removerStock() {
    char produtoParaRemover[100];  // Nome do produto a ser removido
    FILE* fileOriginal = NULL;
    FILE* fileTemp = NULL;
    errno_t errOriginal, errTemp;
    char linha[200];   // Buffer para armazenar cada linha do arquivo
    int encontrado = 0; // Flag para indicar se o produto foi encontrado

    // Solicitar o nome do produto a remover
    printf("Digite o nome do produto que deseja remover:\n");
    scanf_s("%99s", produtoParaRemover, (unsigned)_countof(produtoParaRemover));

    // Abrir o arquivo original em modo leitura
    errOriginal = fopen_s(&fileOriginal, "produtos.txt", "r");
    if (errOriginal != 0 || fileOriginal == NULL) {
        printf("Erro ao abrir o arquivo original! Código de erro: %d\n", errOriginal);
        return;
    }

    // Criar o arquivo temporário em modo escrita
    errTemp = fopen_s(&fileTemp, "temp.txt", "w");
    if (errTemp != 0 || fileTemp == NULL) {
        printf("Erro ao criar o arquivo temporário! Código de erro: %d\n", errTemp);
        fclose(fileOriginal);
        return;
    }

    // Processar linha por linha
    while (fgets(linha, sizeof(linha), fileOriginal) != NULL) {
        // Verificar se a linha contém o nome do produto a ser removido
        if (strstr(linha, produtoParaRemover) != NULL && !encontrado) {
            encontrado = 1; // Produto encontrado
            // Ignorar as próximas duas linhas relacionadas ao produto (quantidade e preço)
            fgets(linha, sizeof(linha), fileOriginal); // Ignorar linha da quantidade
            fgets(linha, sizeof(linha), fileOriginal); // Ignorar linha do preço
            continue; // Pular para a próxima iteração
        }
        // Escrever a linha no arquivo temporário
        fputs(linha, fileTemp);
    }

    // Fechar os arquivos
    fclose(fileOriginal);
    fclose(fileTemp);

    // Substituir o arquivo original pelo temporário
    if (remove("produtos.txt") != 0) {
        printf("Erro ao remover o arquivo original!\n");
        return;
    }

    if (rename("temp.txt", "produtos.txt") != 0) {
        printf("Erro ao renomear o arquivo temporário!\n");
        return;
    }

    // Mensagem de sucesso ou falha
    if (encontrado) {
        printf("Produto '%s' removido com sucesso!\n", produtoParaRemover);
    }
    else {
        printf("Produto '%s' não encontrado no estoque.\n", produtoParaRemover);
    }
}



//função para atualizar os valorms que foram guardados no file
void atualizarStock() {


}




// função do menu
void menu() {
    int escolha1; // para o switch principal do menu
    int escolha2; // para o switch do ver stock
    int escolha3; // para o switch do (ter certeza se quero limpar o stock)
    do {
        system("cls");
        printf("1 - Adicionar Produto\n");
        printf("2 - Ver Stock\n");
        printf("3 - Limpar Stock\n");
        printf("4 - Remover item do Stock\n");
        printf("5 - Atualizar Stock\n");
        printf("6 - SAIR\n");
        printf("\nEscolha: ");
        scanf_s("%d", &escolha1);

        switch (escolha1) {
        case 1:
            system("cls");

            addProduto();

            system("pause");
            break;
        case 2:
            system("cls");

            printf("1-Ver Stock Total\n");
            printf("2-Procurar no Stock\n");
            printf("\nEscolha:");
            scanf_s("%d", &escolha2);

            switch (escolha2) {

            case 1:
                system("cls");

                verStock();

                system("pause");

                break;
            case 2:

                system("cls");

                procurarStock();

                system("pause");

                break;
            }
            break;
        case 3:
            system("cls");

            printf("Tem a certeza que deseja limpar o stock?\n");
            printf("1-Sim\n");
            printf("2-Não\n");
            printf("\nEscolha:");
            scanf_s("%d", &escolha3);

            switch (escolha3) {
            case 1:
                limparStock();
                printf("O seu stock foi limpo\n");
                system("pause");
                break;

            case 2:
                // Ao escolher 2, sai logo do switch e voltamos ao menu principal
                break;
            }
            break;
        case 4:
            system("cls");

            removerStock();

            system("pause");

            break;
        case 5:




            break;
        case 6:
            exit(0);

        default:
            printf("Opção inválida! Tente novamente.\n");
            system("pause");
        }
    } while (escolha1 != 6);
}



// MAIN
int main() {
    inicio();
    menu();
    return 0;
}
