#include <stdio.h>
#include <stdlib.h>
#include "Cliente.h"
#include "Caixa.h"


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

int lerInteiro() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        printf(CYAN "Entrada inválida.\n" RESET);
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}

void cadastrarCliente() {
    char nome[MAX_NOME], cpf[CPF_LENGTH + 1];
    printf(CYAN "Nome: " RESET);
    scanf(" %[^\n]", nome);
    ler_cpf(cpf);
    int prio = ler_prio(), itens = ler_itens();
    Cliente *novo = criar_cliente(nome, cpf, prio, itens);
    if (!novo) { printf(RED "Erro.\n" RESET); return; }
    printf(CYAN "Caixa (1-%d): " RESET, MAX_CAIXAS);
    int idx = lerInteiro() - 1;
    if (idx < 0 || idx >= MAX_CAIXAS || !caixa_aberto(idx)) {
        printf(RED "Caixa inválido.\n" RESET); free(novo); return;
    }
    adicionar_cliente_ao_caixa(idx, novo);
    printf(GREEN "Cliente cadastrado.\n" RESET);
}

void atenderCliente() {
    printf(CYAN "Caixa para atender (1-%d): " RESET, MAX_CAIXAS);
    int idx = lerInteiro() - 1;
    Cliente *cliente = atender_cliente_do_caixa(idx);
    if (!cliente) printf(YELLOW "Fila vazia.\n" RESET);
    else { printf(GREEN "Atendido: %s\n" RESET, cliente->nome); free(cliente); }
}

void gerenciarCaixa() {
    printf(CYAN "Caixa para gerenciar (1-%d): " RESET, MAX_CAIXAS);
    gerenciar_caixa(lerInteiro() - 1);
}

int main() {
    inicializar_caixas();
    int opcao;
    do {
        printf(CYAN "\nMenu:\n1-Cadastrar\n2-Atender\n3-Gerenciar\n4-Imprimir\n0-Sair\nOpção: " RESET);
        opcao = lerInteiro();
        switch(opcao) {
            case 1: cadastrarCliente(); break;
            case 2: atenderCliente(); break;
            case 3: gerenciarCaixa(); break;
            case 4: imprimir_filas_caixas(); break;
            case 0: printf(MAGENTA "Saindo...\n" RESET); break;
            default: printf(RED "Opção inválida.\n" RESET);
        }
    } while (opcao != 0);
    liberar_filas_caixas();
    return 0;
}