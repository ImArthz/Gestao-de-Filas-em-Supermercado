#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 100
#define MAX_CAIXAS 5
#define CPF_LENGTH 11 // Tamanho fixo do CPF

// Códigos de cores ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"


typedef struct Cliente {
    char nome[MAX_NOME];
    char CPF[CPF_LENGTH + 1]; // CPF como string (11 dígitos + '\0')
    int prio;
    int n_itens;
    struct Cliente *proximo;
} Cliente;

typedef struct Caixa {
    int n_ident;
    bool aberto;
    Cliente *fila; // Ponteiro para a fila de clientes
} Caixa;

Caixa caixas[MAX_CAIXAS];

// Função para inicializar os caixas
void inicializarCaixas() {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        caixas[i].n_ident = i + 1;
        caixas[i].aberto = true;
        caixas[i].fila = NULL; // Inicializa a fila como vazia
    }
}

// Função para ler um inteiro com validação
int lerInteiro() {
    int valor;
    while (true) {
        if (scanf("%d", &valor) != 1) {
            printf(CYAN"Entrada inválida. Por favor, digite um número inteiro.\n"RESET);
            while (getchar() != '\n'); // Limpa o buffer
        } else {
            while (getchar() != '\n'); // Limpa o buffer
            return valor;
        }
    }
}
int leritens() {
    int valor;
    while (true) {
        int result = scanf("%d", &valor);
        if (result != 1 || valor <= 0) { // <-- Corrigido para OU lógico (||) e valor <= 0
            printf(RED "Entrada inválida. Digite um número maior que 0.\n" RESET);
            while (getchar() != '\n'); // Limpa o buffer
        } else {
            while (getchar() != '\n'); // Limpa o buffer
            return valor;
        }
    }
}
int lerPrio() {
    int valor;
    while (true) {
        int result = scanf("%d", &valor);
        if (result != 1 || valor < 1 || valor > 3) { // <-- Verifica se está fora do intervalo
            printf(RED "Prioridade inválida. Use 1 (alta), 2 (média) ou 3 (baixa).\n" RESET);
            while (getchar() != '\n'); // Limpa o buffer
        } else {
            while (getchar() != '\n'); // Limpa o buffer
            return valor;
        }
    }
}
// Função para ler CPF como string (com validação)
void lerCPF(char *cpf) {
    char input[20]; // Buffer temporário
    while (1) {
        printf(CYAN "CPF (apenas números): " RESET);
        if (scanf("%11s", input) != 1) { // Lê exatamente 11 caracteres
            printf(RED "Entrada inválida.\n" RESET);
            while (getchar() != '\n'); // Limpa o buffer
            continue;
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
            continue;
        }

        // Limpa o buffer (caso o usuário digitou mais de 11 caracteres)
        while (getchar() != '\n');

        // Copia para o campo CPF do cliente
        strncpy(cpf, input, CPF_LENGTH);
        cpf[CPF_LENGTH] = '\0'; // Garante terminação nula
        return;
    }
}


// Função para cadastrar um cliente

// Função para cadastrar cliente 
void cadastrarCliente() {
    Cliente *novoCliente = (Cliente *)malloc(sizeof(Cliente));
    if (novoCliente == NULL) {
        printf(RED "Erro ao alocar memória.\n" RESET);
        return;
    }

    printf(CYAN "Nome do cliente: " RESET);
    scanf(" %[^\n]", novoCliente->nome);
    lerCPF(novoCliente->CPF); 

 
    printf(CYAN "Prioridade (1-alta, 2-média, 3-baixa): " RESET);
    novoCliente->prio = lerPrio();
    printf(CYAN "Número de itens no carrinho: " RESET);
    novoCliente->n_itens = leritens();
    novoCliente->proximo = NULL;


    int caixaEscolhida;
    printf(CYAN "Escolha um caixa (1-%d): " RESET, MAX_CAIXAS);
    caixaEscolhida = lerInteiro() - 1;

    if (caixaEscolhida < 0 || caixaEscolhida >= MAX_CAIXAS || !caixas[caixaEscolhida].aberto) {
        printf(RED "Caixa inválido ou fechado.\n" RESET);
        free(novoCliente);
        return;
    }

    Cliente **fila = &caixas[caixaEscolhida].fila;
    if (*fila == NULL || (*fila)->prio > novoCliente->prio) {
        novoCliente->proximo = *fila;
        *fila = novoCliente;
    } else {
        Cliente *atual = *fila;
        while (atual->proximo != NULL && atual->proximo->prio <= novoCliente->prio) {
            atual = atual->proximo;
        }
        novoCliente->proximo = atual->proximo;
        atual->proximo = novoCliente;
    }

    printf(GREEN "Cliente cadastrado com sucesso no caixa %d.\n" RESET, caixaEscolhida + 1);
}
// Função para atender um cliente
void atenderCliente() {
    int caixaEscolhida;
    printf(CYAN"Escolha um caixa para atendimento (1-%d): "RESET, MAX_CAIXAS);
    caixaEscolhida = lerInteiro() - 1; // Ajuste para índice

    if (caixaEscolhida < 0 || caixaEscolhida >= MAX_CAIXAS || !caixas[caixaEscolhida].aberto) {
        printf(RED"Caixa inválido ou fechado.\n"RESET);
        return;
    }

    Cliente *cliente = caixas[caixaEscolhida].fila;
    if (cliente == NULL) {
        printf(YELLOW"Não há clientes na fila do caixa %d.\n"RESET, caixaEscolhida + 1);
        return;
    }

    caixas[caixaEscolhida].fila = cliente->proximo; // Remove o cliente da fila
    printf(GREEN"Cliente %s atendido no caixa %d.\n"RESET, cliente->nome, caixaEscolhida + 1);
    free(cliente);
}
int contarCaixasAbertos() {
    int count = 0;
    for (int i = 0; i < MAX_CAIXAS; i++) {
        if (caixas[i].aberto) {
            count++;
        }
    }
    return count;
}
// Função para abrir ou fechar um caixa
void gerenciarCaixa() {
    int caixaEscolhida;
    printf(CYAN"Escolha um caixa para abrir/fechar (1-%d): "RESET, MAX_CAIXAS);
    caixaEscolhida = lerInteiro() - 1; // Ajuste para índice

    if (caixaEscolhida < 0 || caixaEscolhida >= MAX_CAIXAS) {
        printf(RED"Caixa inválido.\n"RESET);
        return;
    }

    // Verifica se o caixa está aberto e se há mais de um caixa aberto
    if (caixas[caixaEscolhida].aberto && contarCaixasAbertos() <= 1) {
        printf(RED"Não é possível fechar o último caixa aberto.\n"RESET);
        return;
    }

    caixas[caixaEscolhida].aberto = !caixas[caixaEscolhida].aberto;
    if (!caixas[caixaEscolhida].aberto) {
        printf(YELLOW"Caixa %d fechado. Realoque clientes.\n"RESET, caixaEscolhida + 1);
        // Realoque clientes para outros caixas abertos
        Cliente *cliente = caixas[caixaEscolhida].fila;
        caixas[caixaEscolhida].fila = NULL;

        while (cliente != NULL) {
            Cliente *temp = cliente;
            cliente = cliente->proximo;

            // Encontra o caixa com a menor fila aberta
            int menorFilaIndex = -1;
            int menorFilaCount = __INT_MAX__; // Inicia com um valor grande
            for (int i = 0; i < MAX_CAIXAS; i++) {
                if (caixas[i].aberto) {
                    int filaCount = 0;
                    Cliente *tempFila = caixas[i].fila;
                    while (tempFila != NULL) {
                        filaCount++;
                        tempFila = tempFila->proximo;
                    }
                    if (filaCount < menorFilaCount) {
                        menorFilaCount = filaCount;
                        menorFilaIndex = i;
                    }
                }
            }

            // Realoque para o caixa com a menor fila
            if (menorFilaIndex != -1) {
                temp->proximo = caixas[menorFilaIndex].fila;
                caixas[menorFilaIndex].fila = temp;
            } else {
                // Se não há caixa aberto, os clientes são descartados
                free(temp);
            }
        }
    } else {
        printf(YELLOW"Caixa %d aberto.\n"RESET, caixaEscolhida + 1);
    }
}
// Função para imprimir clientes (ajustada para CPF string)
void imprimirClientes() {
    for (int i = 0; i < MAX_CAIXAS; i++) {
        printf(YELLOW "Caixa %d (%s): ", caixas[i].n_ident, caixas[i].aberto ? "Aberto" : "Fechado" RESET);
        Cliente *atual = caixas[i].fila;
        while (atual != NULL) {
            printf(GREEN "%s (CPF: %s, Prioridade: %d, Itens: %d) -> " RESET, 
                   atual->nome, atual->CPF, atual->prio, atual->n_itens);
            atual = atual->proximo;
        }
        printf("NULL\n");
    }
}

int main() {
    inicializarCaixas();
    int opcao;

    do {
        printf(CYAN"\n\t Menu \n");
        printf("\t1- Cadastrar Cliente \n");
        printf("\t2- Atender Cliente \n");
        printf("\t3- Gerenciar Caixas \n");
        printf("\t4- Imprimir Clientes em Espera \n");
        printf("\t0- Sair \n");
        printf("Escolha uma opção: "RESET);
        opcao = lerInteiro();

        switch(opcao) {
            case 1:
                cadastrarCliente();
                break;
            case 2:
                atenderCliente();
                break;
            case 3:
                gerenciarCaixa();
                break;
            case 4:
                imprimirClientes();
                break;
            case 0:
                printf(MAGENTA"Saindo do programa.\n"RESET);
                break;
            default:
                printf(RED"\nOpção Inválida.\n"RESET);
        }
    } while (opcao != 0);

    return 0;
}