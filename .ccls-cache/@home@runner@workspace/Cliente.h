#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdbool.h>

#define MAX_NOME 100
#define CPF_LENGTH 11

typedef struct Cliente {
    char nome[MAX_NOME];
    char CPF[CPF_LENGTH + 1];
    int prio;
    int n_itens;
    struct Cliente *proximo;
} Cliente;

Cliente* criar_cliente(const char *nome, const char *cpf, int prio, int n_itens);
void ler_cpf(char *cpf);
int ler_prio();
int ler_itens();

#endif