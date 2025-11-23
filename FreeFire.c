//trabalho feito por: 
//Leonardo Félix de Souza Júnior
//matricula: 202403278839

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10  // limite máximo de itens na mochila

// enum para definir os critérios de ordenação possíveis
enum CriterioOrdenacao { CRIT_NOME, CRIT_TIPO, CRIT_PRIORIDADE };

// struct para representar cada item na mochila
typedef struct {
    char nome[50];       // nome do item
    char tipo[30];       // tipo (ex: arma, comida, curativo, etc)
    int quantidade;      // quantos desse item eu tenho
    int prioridade;      // prioridade de 1 a 5 (quanto mais alto, pode ser “mais importante”)
} Item;

// protótipos das funções para organizar o programa
void limparTela();
void exibirMenu(bool ordenadoPorNome);
void inserirItem(Item mochila[], int *numItens, bool *ordenadoPorNome);
void removerItem(Item mochila[], int *numItens, bool *ordenadoPorNome);
void listarItens(Item mochila[], int numItens);
void menuDeOrdenacao(Item mochila[], int numItens, bool *ordenadoPorNome);
void insertionSort(Item mochila[], int numItens, enum CriterioOrdenacao criterio, int *comparacoes);
int buscaBinariaPorNome(Item mochila[], int numItens, char *nomeBusca);

int main() {
    Item mochila[MAX_ITENS];     // aqui a gente guarda os itens coletados
    int numItens = 0;            // quantos itens já foram adicionados
    bool ordenadoPorNome = false; // flag para saber se está ordenado por nome

    int opcao;

    do {
        limparTela();                 // dá uma “limpada” na tela (só imprimir linhas)
        exibirMenu(ordenadoPorNome);  // mostra o menu com as opções, depende da flag

        printf("escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            // se o usuário digitar algo que não é número, trata isso
            int lixo;
            printf("entrada invalida.\n");
            while ((lixo = getchar()) != '\n' && lixo != EOF); // descarta o resto
            opcao = -1; // força uma opção inválida para cair no default
        }

        switch (opcao) {
            case 1:
                inserirItem(mochila, &numItens, &ordenadoPorNome);
                break;
            case 2:
                removerItem(mochila, &numItens, &ordenadoPorNome);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4:
                menuDeOrdenacao(mochila, numItens, &ordenadoPorNome);
                break;
            case 5: {
                // busca binária por nome só funciona se a mochila estiver ordenada por nome
                if (!ordenadoPorNome) {
                    printf("voce precisa ordenar por nome antes da busca binaria!\n");
                } else {
                    char nomeBusca[50];
                    int lixo;
                    printf("digite o nome do item para buscar (binaria): ");
                    while ((lixo = getchar()) != '\n' && lixo != EOF);
                    scanf(" %49[^\n]", nomeBusca);

                    int idx = buscaBinariaPorNome(mochila, numItens, nomeBusca);
                    if (idx >= 0) {
                        Item *it = &mochila[idx];
                        printf("item encontrado:\n");
                        printf("  nome: %s\n", it->nome);
                        printf("  tipo: %s\n", it->tipo);
                        printf("  quantidade: %d\n", it->quantidade);
                        printf("  prioridade: %d\n", it->prioridade);
                    } else {
                        printf("item nao encontrado na busca binaria.\n");
                    }
                }
                break;
            }
            case 0:
                printf("saindo do programa… boa sorte na ilha!\n");
                break;
            default:
                printf("opcao invalida, tenta de novo.\n");
                break;
        }

        if (opcao != 0) {
            int lixo;
            printf("\npressione enter para continuar...");
            while ((lixo = getchar()) != '\n' && lixo != EOF);
            getchar(); // espera o usuário apertar enter
        }

    } while (opcao != 0);

    return 0;
}

void limparTela() {
    // simula a limpeza da tela apenas imprimindo muitas linhas vazias
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

void exibirMenu(bool ordenadoPorNome) {
    // mostra as opções do menu para o usuário
    printf("==== gestao da mochila da ilha ====\n");
    // informa se a lista está ou não ordenada por nome
    printf("itens na mochila: %s\n", ordenadoPorNome ? "ordenada por nome" : "nao ordenada por nome");
    printf("1. adicionar item\n");
    printf("2. remover item\n");
    printf("3. listar todos os itens\n");
    printf("4. ordenar itens (nome, tipo ou prioridade)\n");
    printf("5. busca binaria por nome (so se tiver ordenado por nome)\n");
    printf("0. sair\n");
}

void inserirItem(Item mochila[], int *numItens, bool *ordenadoPorNome) {
    // adiciona um novo item na mochila, se ainda houver espaço
    if (*numItens >= MAX_ITENS) {
        printf("mochila cheia! nao da pra adicionar mais itens.\n");
        return;
    }

    int lixo;
    // limpa o buffer pra evitar que restos de input anterior atrapalhem
    while ((lixo = getchar()) != '\n' && lixo != EOF);

    // lê o nome do item (aceita espaços até o final da linha)
    printf("digite o nome do item: ");
    scanf(" %49[^\n]", mochila[*numItens].nome);

    // lê o tipo do item
    printf("digite o tipo do item: ");
    scanf(" %29[^\n]", mochila[*numItens].tipo);

    // lê a quantidade (número inteiro)
    printf("digite a quantidade: ");
    scanf("%d", &mochila[*numItens].quantidade);

    // lê a prioridade de 1 a 5
    printf("digite a prioridade (1 a 5): ");
    int p;
    scanf("%d", &p);
    if (p < 1) p = 1;
    if (p > 5) p = 5;  // valida para que fique entre 1 e 5
    mochila[*numItens].prioridade = p;

    // incrementa o número de itens cadastrados
    (*numItens)++;
    // sempre que agregar algo, a ordenação por nome deixa de ser confiável
    *ordenadoPorNome = false;

    printf("item adicionado com sucesso!\n");
}

void removerItem(Item mochila[], int *numItens, bool *ordenadoPorNome) {
    // remove um item pelo nome, se existir algum
    if (*numItens == 0) {
        printf("mochila vazia, nada pra remover.\n");
        return;
    }

    char nomeRemover[50];
    int pos = -1;
    int lixo;

    // limpa buffer antes de ler o nome a remover
    while ((lixo = getchar()) != '\n' && lixo != EOF);

    printf("digite o nome do item pra remover: ");
    scanf(" %49[^\n]", nomeRemover);

    // busca sequencial para encontrar a posição desse nome
    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        // se não achou, avisa que nada foi removido
        printf("item nao encontrado. nada removido.\n");
    } else {
        // se achou, “puxa” todos os itens após esse para trás para preencher o espaço
        for (int j = pos; j < *numItens - 1; j++) {
            mochila[j] = mochila[j + 1];
        }
        (*numItens)--;  // diminui a contagem de itens
        *ordenadoPorNome = false;  // como mudou a ordem, já não pode garantir ordenação por nome
        printf("item removido com sucesso.\n");
    }
}

void listarItens(Item mochila[], int numItens) {
    // imprime uma tabela com todos os itens da mochila
    if (numItens == 0) {
        printf("mochila ta vazia.\n");
        return;
    }

    printf("\nitens na mochila:\n");
    // cabeçalho da tabela: nome, tipo, quantidade, prioridade
    printf("%-20s %-15s %-10s %-10s\n", "nome", "tipo", "quantidade", "prioridade");
    // para cada item, imprime seus dados formatados
    for (int i = 0; i < numItens; i++) {
        printf("%-20s %-15s %-10d %-10d\n",
            mochila[i].nome,
            mochila[i].tipo,
            mochila[i].quantidade,
            mochila[i].prioridade);
    }
}

void menuDeOrdenacao(Item mochila[], int numItens, bool *ordenadoPorNome) {
    // abre o menu para escolher como ordenar os itens
    if (numItens < 2) {
        // se tiver menos de 2 itens, nem faz sentido ordenar
        printf("precisa ter pelo menos 2 itens pra ordenar.\n");
        return;
    }

    int opc;
    int comparacoes = 0;  // vai contar quantas comparações o insertion sort fez

    printf("escolha o criterio para ordenar:\n");
    printf("0 - nome\n");
    printf("1 - tipo\n");
    printf("2 - prioridade (1 a 5)\n");
    printf("criterio: ");
    scanf("%d", &opc);

    // valida se o critério é válido
    if (opc < CRIT_NOME || opc > CRIT_PRIORIDADE) {
        printf("criterio invalido.\n");
        return;
    }

    // chama o insertion sort passando o critério e a variável para contar comparações
    insertionSort(mochila, numItens, (enum CriterioOrdenacao)opc, &comparacoes);
    printf("ordenacao feita! comparacoes feitas: %d\n", comparacoes);

    // se o critério escolhido for pelo nome, marca que agora está ordenado por nome
    if (opc == CRIT_NOME) {
        *ordenadoPorNome = true;
    } else {
        *ordenadoPorNome = false;
    }
}

void insertionSort(Item mochila[], int numItens, enum CriterioOrdenacao criterio, int *comparacoes) {
    // algoritmo insertion sort adaptado para diferentes critérios
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];  // “pega” o item que vai inserir na parte certa
        int j = i - 1;

        // laço para mover itens que devem ficar depois da chave
        while (j >= 0) {
            (*comparacoes)++;  // cada comparação entre elementos conta

            bool deveMover = false;
            // dependendo do critério, compara nome, tipo ou prioridade
            if (criterio == CRIT_NOME) {
                if (strcmp(mochila[j].nome, chave.nome) > 0) {
                    deveMover = true;
                }
            } else if (criterio == CRIT_TIPO) {
                if (strcmp(mochila[j].tipo, chave.tipo) > 0) {
                    deveMover = true;
                }
            } else { // prioridade
                if (mochila[j].prioridade > chave.prioridade) {
                    deveMover = true;
                }
            }

            if (!deveMover) {
                // se não precisa mover mais, sai do laço
                break;
            }

            // move o item da posição j para j+1 (faz a “troca”)
            mochila[j + 1] = mochila[j];
            j--;
        }

        // coloca a “chave” na posição correta (j+1)
        mochila[j + 1] = chave;
    }
}

int buscaBinariaPorNome(Item mochila[], int numItens, char *nomeBusca) {
    // busca binária para encontrar um item pelo nome, assumindo que está ordenado por nome
    int low = 0;
    int high = numItens - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(mochila[mid].nome, nomeBusca);

        if (cmp == 0) {
            // se for igual, achamos o item
            return mid;
        } else if (cmp < 0) {
            // se nomeBusca é “maior” que o nome no meio, procura na metade direita
            low = mid + 1;
        } else {
            // se nomeBusca é “menor”, procura na metade esquerda
            high = mid - 1;
        }
    }

    // se sair do loop, não achou
    return -1;
}