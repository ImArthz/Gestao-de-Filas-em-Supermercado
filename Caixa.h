#ifndef CAIXA_H
#define CAIXA_H

#include "Cliente.h"

#define MAX_CAIXAS 5

typedef struct Caixa {
    int n_ident;
    bool aberto;
    Cliente *fila;
} Caixa;

void inicializar_caixas();
void adicionar_cliente_ao_caixa(int caixa_idx, Cliente *cliente);
Cliente* atender_cliente_do_caixa(int caixa_idx);
void gerenciar_caixa(int caixa_idx);
void imprimir_filas_caixas();
bool caixa_aberto(int caixa_idx);
int contar_caixas_abertos();
void liberar_filas_caixas();

#endif