#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Cliente.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"

Cliente* criar_cliente(const char *nome, const char *cpf, int prio, int n_itens) {
    Cliente *cliente = (Cliente*)malloc(sizeof(Cliente));
    if (cliente == NULL) return NULL;
    strncpy(cliente->nome, nome, MAX_NOME);
    strncpy(cliente->CPF, cpf, CPF_LENGTH + 1);
    cliente->prio = prio;
    cliente->n_itens = n_itens;
    cliente->proximo = NULL;
    return cliente;
}

void ler_cpf(char *cpf) {
    char input[20];
    while (1) {
        printf(CYAN "CPF (apenas números): " RESET);
        if (scanf("%11s", input) != 1) {
            printf(RED "Entrada inválida.\n" RESET);
            while (getchar() != '\n'); // Limpa o buffer
            continue; // Volta ao início do loop
        }

        // Verifica se todos são dígitos
        bool valido = true;
        for (int i = 0; i < CPF_LENGTH; i++) {
            if (!isdigit(input[i])) {
                valido = false;
                break;
            }
        }

        if (strlen(input) != CPF_LENGTH || !valido) {
            printf(RED "CPF deve ter exatamente 11 dígitos numéricos.\n" RESET);
            while (getchar() != '\n'); // Limpa o buffer
            continue; // Volta ao início do loop
        }

        // Limpa o buffer (caso o usuário digitou mais de 11 caracteres)
        while (getchar() != '\n');

        // Copia para o campo CPF do cliente
        strncpy(cpf, input, CPF_LENGTH);
        cpf[CPF_LENGTH] = '\0'; // Garante terminação nula
        return;
    }
}
int ler_prio() {
    int valor;
    while (1) {
        printf(CYAN "Prioridade (1-alta, 2-média, 3-baixa): " RESET);
        int result = scanf("%d", &valor);
        if (result != 1 || valor < 1 || valor > 3) {
            printf(RED "Prioridade inválida.\n" RESET);
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            return valor;
        }
    }
}

int ler_itens() {
    int valor;
    while (1) {
        printf(CYAN "Número de itens: " RESET);
        int result = scanf("%d", &valor);
        if (result != 1 || valor <= 0) {
            printf(RED "Número inválido.\n" RESET);
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            return valor;
        }
    }
}