#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pessoa {
    char nome[50];
    char cpf[11];
    int idade;
    struct Pessoa* proximo;
} Pessoa;

typedef struct Frasco {
    int dosesDisponiveis;
    struct Frasco* proximo;
} Frasco;

void adicionarPessoa(Pessoa** fila, char* nome, char* cpf, int idade) {
    Pessoa* novaPessoa = (Pessoa*)malloc(sizeof(Pessoa));

    if (novaPessoa == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }

    strcpy(novaPessoa->nome, nome);
    strcpy(novaPessoa->cpf, cpf);
    novaPessoa->idade = idade;
    novaPessoa->proximo = NULL;

    if (*fila == NULL) {
        *fila = novaPessoa;
    } else {
        Pessoa* atual = *fila;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novaPessoa;
    }
}

void empilharFrasco(Frasco** pilha) {
    Frasco* novoFrasco = (Frasco*)malloc(sizeof(Frasco));

    if (novoFrasco == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }

    novoFrasco->dosesDisponiveis = 5;
    novoFrasco->proximo = *pilha;
    *pilha = novoFrasco;
}

Frasco* desempilharFrasco(Frasco** pilha) {
    if (*pilha == NULL) return NULL;

    Frasco* removido = *pilha;
    *pilha = removido->proximo;
    return removido;
}

void vacinarPessoa(Pessoa** fila, Frasco** pilha, int* totalVacinados, int* frascoCont) {
    if (*pilha == NULL) {
        printf("Nenhum frasco disponível!\n");
        return;
    }

    if (*fila == NULL) {
        printf("Fila vazia! Nenhuma pessoa para vacinar.\n");
        return;
    }

    Frasco* frascoAtual = *pilha;

    if (frascoAtual->dosesDisponiveis > 0) {
        Pessoa* vacinada = *fila;
        *fila = vacinada->proximo;

        (*totalVacinados)++;
        frascoAtual->dosesDisponiveis--;

        printf("Pessoa vacinada: %s, CPF: %s, Idade: %d\n", vacinada->nome, vacinada->cpf, vacinada->idade);
        free(vacinada);

        if (frascoAtual->dosesDisponiveis == 0) {
            (*frascoCont)++;
            printf("Frasco (%d) vazio! Desempilhando...\n", *frascoCont);
            free(desempilharFrasco(pilha));
        }
    }
}

void imprimirFila(Pessoa* fila) {
    Pessoa* atual = fila;
    int contPessoaFila = 0;

    if (atual == NULL) {
        printf("Fila Vazia!\n");
        return;
    }

    while (atual != NULL) {
        contPessoaFila += 1;
        printf("%d- Nome: %s, CPF: %s, Idade: %d\n", contPessoaFila, atual->nome, atual->cpf, atual->idade);
        atual = atual->proximo;
    }
}

int main() {
    Pessoa* fila = NULL;
    Frasco* pilha = NULL;

    int totalVacinados = 0;
    const int limitePessoas = 15;
    int frascoCont = 0;

    for (int i = 0; i < 3; i++) {
        empilharFrasco(&pilha);
    }
    printf("Frascos de vacina empilhados!\n");

    while (1) {
        printf("\nMenu:\n"
               "1. Adicionar pessoa na fila\n"
               "2. Vacinar pessoa\n"
               "3. Imprimir fila\n"
               "4. Exibir total de vacinados\n"
               "0. Encerrar atendimento\n"
               "Escolha uma opção: ");

        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (totalVacinados >= limitePessoas) {
                    printf("Apenas 15 pessoas podem ser atendidas por dia!\n");
                } else {
                    char nome[50];
                    char cpf[11];
                    int idade;

                    printf("Digite o nome completo: ");
                    getchar();  // Limpar o buffer
                    fgets(nome, 50, stdin);
                    nome[strcspn(nome, "\n")] = '\0';  // Remover o '\n' no final da string

                    printf("Digite seu CPF: ");
                    fgets(cpf, 11, stdin);
                    cpf[strcspn(cpf, "\n")] = '\0';  // Remover o '\n' no final da string

                    printf("Digite a idade: ");
                    scanf("%d", &idade);

                    adicionarPessoa(&fila, nome, cpf, idade);
                    printf("Pessoa adicionada na fila!\n");
                }
                break;
            }

            case 2: {
                vacinarPessoa(&fila, &pilha, &totalVacinados, &frascoCont);
                break;
            }

            case 3: {
                imprimirFila(fila);
                break;
            }

            case 4: {
                printf("Total de vacinados: %d\n", totalVacinados);
                break;
            }

            case 0: {
                printf("Encerrando atendimento...\n");

                while (fila != NULL) {
                    Pessoa* temp = fila;
                    fila = fila->proximo;
                    free(temp);
                }
                while (pilha != NULL) {
                    Frasco* temp = pilha;
                    pilha = pilha->proximo;
                    free(temp);
                }

                return 0;
            }

            default: {
                printf("Opção inválida! Tente novamente.\n");
                break;
            }
        }
    }

    return 0;
}
