#include <stdio.h>
#include "lab.h"
#include <stdlib.h>


// struct para o produto com as informa��es
struct Produto {
    int quant;
    float preco;
    char name[100];
} produto;



// fun��o default de in�cio
void inicio() {
    // As fun��es abaixo est�o presumidamente no lab.h
    setFullScreen(true);
    setColor(MY_COLOR_WHITE, MY_COLOR_DARK_BLUE);
    system("cls");
    showRectAt(40, 10, 30, 10);
    gotoxy(45, 15);
    printf("      STOCKSAGE \n");
    gotoxy(10, 22);
    system("pause");
}



// fun��o para abrir um ficheiro e armazenar os dados da struct Produto
void armazenarProduto(struct Produto p) {
    FILE* file = NULL;
    errno_t err;  // Vari�vel para armazenar o c�digo de erro

    err = fopen_s(&file, "produtos.txt", "a");

    // Verificar se houve erro ao abrir o .txt
    if (err != 0 || file == NULL) {
        printf("Erro ao abrir o ficheiro! C�digo de erro: %d\n", err);
        return;
    }

    // Escreve no .txt
    fprintf(file, "Produto: %s\n", p.name);
    fprintf(file, "Quantidade: %d\n", p.quant);
    fprintf(file, "Pre�o: %.2f �\n", p.preco);
    fprintf(file, "---------------------------\n");

    // Fechar o file .txt
    fclose(file);
    printf("Dados armazenados com sucesso!\n");
}



// fun��o para adicionar produto
void addProduto() {
    // Inicializar os valores da struct
    produto.quant = 0;
    produto.preco = 0;

    printf("Indique o nome do produto que deseja adicionar:\n");
    scanf_s("%s", produto.name, (unsigned)_countof(produto.name));

    printf("Indique a sua quantidade atual em stock:\n");
    scanf_s("%d", &produto.quant);

    printf("Indique o seu pre�o:\n");
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
        printf("Erro ao abrir o ficheiro! C�digo de erro: %d\n", err);
        return;
    }

    // Fechar o ficheiro imediatamente ap�s a abertura em modo "w" apaga o conte�do
    fclose(file);
    printf("Stock limpo com sucesso!\n");
}




void verStock() {

    FILE* file = NULL;
    errno_t err;
    char linha[200];  // Buffer para armazenar cada linha do ficheiro

    err = fopen_s(&file, "produtos.txt", "r");

    if (err != 0 || file == NULL) {
        printf("Erro ao abrir o ficheiro! C�digo de erro: %d\n", err);
        return;
    }
    
    printf("---- Stock Atual ----\n");
    while (fgets(linha, sizeof(linha), file) != NULL) {
        printf("%s", linha);
    }

    fclose(file);
    printf("Stock aberto com sucesso!\n");
}


// fun��o de um menu
void menu() {
    int escolha;
    do {
        system("cls");
        printf("1 - Adicionar Produto\n");
        printf("2 - Ver Stock\n");
        printf("3 - Limpar Stock\n");
        printf("4 - SAIR\n");
        printf("\nEscolha: ");
        scanf_s("%d", &escolha);

        switch (escolha) {
        case 1:
            system("cls");

            addProduto();

            system("pause");
            break;
        case 2:
            system("cls");

            verStock();

            system("pause");
            break;
        case 3:
            system("cls");
            limparStock();
            printf("O seu stock foi limpo\n");

            system("pause");
            break;
        case 4:
            exit(0);  // Sai do programa
        default:
            printf("Op��o inv�lida! Tente novamente.\n");
            system("pause");
        }
    } while (escolha != 4);
}



// MAIN
int main() {

    inicio();
    menu();

    return 0;
}
