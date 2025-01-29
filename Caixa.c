#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Caixa.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

static Caixa caixas[MAX_CAIXAS];

void inicializar_caixas() {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        caixas[i].n_ident = i + 1;
        caixas[i].aberto = true;
        caixas[i].fila = NULL;
    }
}

void adicionar_cliente_ao_caixa(int caixa_idx, Cliente *cliente) {
    if (caixa_idx < 0 || caixa_idx >= MAX_CAIXAS) return;
    Cliente **fila = &caixas[caixa_idx].fila;
    if (*fila == NULL || (*fila)->prio > cliente->prio) {
        cliente->proximo = *fila;
        *fila = cliente;
    } else {
        Cliente *atual = *fila;
        while (atual->proximo && atual->proximo->prio <= cliente->prio)
            atual = atual->proximo;
        cliente->proximo = atual->proximo;
        atual->proximo = cliente;
    }
}

Cliente* atender_cliente_do_caixa(int caixa_idx) {
    if (caixa_idx < 0 || caixa_idx >= MAX_CAIXAS || !caixas[caixa_idx].aberto)
        return NULL;
    Cliente *cliente = caixas[caixa_idx].fila;
    if (!cliente) return NULL;
    caixas[caixa_idx].fila = cliente->proximo;
    cliente->proximo = NULL;
    return cliente;
}

void gerenciar_caixa(int caixa_idx) {
    if (caixa_idx < 0 || caixa_idx >= MAX_CAIXAS) {
        printf(RED "Caixa inválido.\n" RESET); return;
    }
    if (caixas[caixa_idx].aberto && contar_caixas_abertos() <= 1) {
        printf(RED "Não é possível fechar o último caixa.\n" RESET); return;
    }
    caixas[caixa_idx].aberto = !caixas[caixa_idx].aberto;
    if (!caixas[caixa_idx].aberto) {
        printf(YELLOW "Caixa %d fechado.\n" RESET, caixa_idx + 1);
        Cliente *cliente = caixas[caixa_idx].fila;
        caixas[caixa_idx].fila = NULL;
        while (cliente) {
            Cliente *temp = cliente;
            cliente = cliente->proximo;
            int menorFilaIndex = -1, menorFilaCount = __INT_MAX__;
            for (int i = 0; i < MAX_CAIXAS; i++) {
                if (caixas[i].aberto) {
                    int count = 0;
                    Cliente *atual = caixas[i].fila;
                    while (atual) { count++; atual = atual->proximo; }
                    if (count < menorFilaCount) {
                        menorFilaCount = count; menorFilaIndex = i;
                    }
                }
            }
            if (menorFilaIndex != -1)
                adicionar_cliente_ao_caixa(menorFilaIndex, temp);
            else
                free(temp);
        }
    } else {
        printf(YELLOW "Caixa %d aberto.\n" RESET, caixa_idx + 1);
    }
}

void imprimir_filas_caixas() {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        printf(YELLOW "Caixa %d (%s): ", caixas[i].n_ident, caixas[i].aberto ? "Aberto" : "Fechado");
        Cliente *atual = caixas[i].fila;
        while (atual) {
            printf(GREEN "%s (CPF: %s) -> ", atual->nome, atual->CPF);
            atual = atual->proximo;
        }
        printf(RESET "NULL\n");
    }
}

bool caixa_aberto(int caixa_idx) {
    return (caixa_idx >= 0 && caixa_idx < MAX_CAIXAS && caixas[caixa_idx].aberto);
}

int contar_caixas_abertos() {
    int count = 0;
    for (int i = 0; i < MAX_CAIXAS; i++)
        if (caixas[i].aberto) count++;
    return count;
}

void liberar_filas_caixas() {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        Cliente *atual = caixas[i].fila;
        while (atual) {
            Cliente *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        caixas[i].fila = NULL;
    }
}