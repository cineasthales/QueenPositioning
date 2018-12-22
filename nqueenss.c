#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    char **tab; // estado do tabuleiro
    struct Node *prox; // proximo no da pilha
} Node;

typedef struct {
    Node *topo; // topo da pilha
    int tam; // tamanho da pilha
} Stack;

typedef struct {
    unsigned long long *vetor; // valores parciais dos posicionamentos
    int tam; // tamanho do vetor
} Partial;

void insereNo(Stack *pilha);
void removeNo(Stack *pilha, int dim);
void colocaRainha(Stack *pilha, int dim, int k, int l);
int insereParcial(Partial *parciais, unsigned long long parcial);

// PARAMETROS
// dim: dimensao do tabuleiro quadrado
// queens: numero de rainhas

unsigned long long nqueens(int dim, int queens) {

    // casos de valores invalidos
    if (dim < 1 || queens < 1)
        return 0;

    // se for apenas uma rainha, retornar 2^(dim^2) - 1
    if (queens == 1)
        return pow(2, dim * dim) - 1;

    // se for mais de uma rainha, monta pilha com
    // possibilidades de posicionamentos no tabuleiro
    Stack * pilha;
    pilha = (Stack *) malloc(sizeof(Stack));
    pilha->topo = NULL;
    pilha->tam = 0;

    // inicializa struct com vetor de parciais
    Partial *parciais;
    parciais = (Partial *) malloc(sizeof(Partial));
    parciais->vetor = NULL;
    parciais->tam = 0;

    int i, j, k, l, x, y; // contadores de for
    int pot; // potencia do calculo
    int exclui; // flag de exclusao do no
    int parou; // flag de interrupcao de for aninhado
    unsigned long long parcial; // soma parcial de posicao de rainhas
    unsigned long long resultado = 0; // soma a ser retornada

    // calcula numero de linhas que precisa ser percorrido
    int linhas = dim - queens + 1;

    // para cada posicao inicial de rainha no tabuleiro
    for (k = 0; k < linhas; ++k) {
        for (l = 0; l < dim; ++l) {

            // insere primeiro no na pilha
            insereNo(pilha);

            // aloca dinamicamente o tabuleiro e zera-o
            pilha->topo->tab = (char **) malloc(dim * sizeof(char *));
            for (i = 0; i < dim; ++i)
                pilha->topo->tab[i] = (char *) malloc(dim * sizeof(char));
            for (i = 0; i < dim; ++i)
                for (j = 0; j < dim; ++j)
                    pilha->topo->tab[i][j] = '0';

            // coloca primeira rainha no tabuleiro
            colocaRainha(pilha, dim, k, l);

            // enquanto a pilha nao esvaziar
            while (pilha->topo != NULL) {

                // se o tamanho da pilha for o mesmo que o número de
                // rainhas a inserir, contabiliza a parcial
                if (pilha->tam == queens) {
                    // inicializa valores necessarios
                    parcial = 0;
                    pot = 0;
                    // percorre o tabuleiro atual
                    for (i = 0; i < dim; ++i) {
                        for (j = 0; j < dim; ++j) {
                            // se encontra uma rainha, soma na parcial
                            if (pilha->topo->tab[i][j] == '2')
                                parcial = parcial + pow(2, pot);
                            // incrementa potencia da posicao
                            ++pot;
                        }
                    }
                    // se a parcial calculada nao esta no vetor,
                    // soma no resultado a ser retornado
                    if (insereParcial(parciais, parcial)) {
                        resultado = resultado + parcial;
                        // se teve overflow, libera memoria e retorna zero
                        if (resultado < 0){
                            while (pilha->topo != NULL) {
                                removeNo(pilha, dim);
                            }
                            free(parciais->vetor);
                            free(parciais);
                            free(pilha);
                            return 0;
                        }
                    }
                    // remove no da pilha
                    removeNo(pilha, dim);
                } else {
                    // inicializa flags
                    exclui = 1;
                    parou = 0;
                    // percorre o tabuleiro atual
                    for (i = 0; i < dim; ++i) {
                        for (j = 0; j < dim; ++j) {
                            // se encontra um espaco
                            if (pilha->topo->tab[i][j] == '0' && parou == 0) {
                                // muda a flag
                                exclui = 0;
                                // insere novo no
                                insereNo(pilha);
                                // aloca dinamicamente o tabuleiro do novo no
                                pilha->topo->tab = (char **) malloc(dim * sizeof(char *));
                                for (x = 0; x < dim; ++x)
                                    pilha->topo->tab[x] = (char *) malloc(dim * sizeof(char));
                                // novo no recebe copia do tabuleiro atual
                                for (x = 0; x < dim; ++x)
                                    for (y = 0; y < dim; ++y)
                                        pilha->topo->tab[x][y] = pilha->topo->prox->tab[x][y];
                                // coloca rainha no novo no
                                colocaRainha(pilha, dim, i, j);
                                // marca posicao ja verificada
                                pilha->topo->prox->tab[i][j] = '1';
                                // interrompe verificacao
                                parou = 1;
                                break;
                            }
                        }
                        // interrompe verificacao
                        if (parou)
                            break;
                    }
                    // se nao tem mais possibilidades, remove no da pilha
                    if (exclui)
                        removeNo(pilha, dim);
                }
            }
        }
    }

    // libera memoria alocada dinamicamente
    free(parciais->vetor);
    free(parciais);
    free(pilha);

    return resultado;
}

void insereNo(Stack *pilha) {

    // aloca espaco para novo no
    Node *no = (Node *) malloc(sizeof(Node));
    no->tab = NULL;
    no->prox = NULL;

    // posiciona o novo no na pilha
    if (pilha->topo != NULL)
        no->prox = pilha->topo;
    pilha->topo = no;
    // incrementa contador de nos na pilha
    pilha->tam += 1;
}

void removeNo(Stack *pilha, int dim) {

    // no a ser removido
    Node *no = pilha->topo;

    // libera da memoria estado do tabuleiro
    for (int i = 0; i < dim; ++i)
        free(no->tab[i]);
    free(no->tab);

    // remove o no da pilha
    pilha->topo = pilha->topo->prox;
    // decrementa contador de nos na pilha
    pilha->tam -= 1;

    // libera o no da memoria
    free(no);
}


void colocaRainha(Stack *pilha, int dim, int k, int l) {

    int i, j; // contadores de for

    // coloca rainha
    pilha->topo->tab[k][l] = '2';

    // nao pode outras rainhas na mesma linha
    for (i = 0; i < dim; ++i)
        if (pilha->topo->tab[k][i] == '0')
            pilha->topo->tab[k][i] = '1';

    // nao pode outras rainhas na mesma coluna
    for (i = 0; i < dim; ++i)
        if (pilha->topo->tab[i][l] == '0')
            pilha->topo->tab[i][l] = '1';

    // nao pode outras rainhas na diagonal cima-esquerda
    i = k - 1;
    j = l - 1;
    while (i > -1 && j > -1) {
        if (pilha->topo->tab[i][j] == '0')
            pilha->topo->tab[i][j] = '1';
        --i;
        --j;
    }

    // nao pode outras rainhas na diagonal cima-direita
    i = k - 1;
    j = l + 1;
    while (i > -1 && j < dim) {
        if (pilha->topo->tab[i][j] == '0')
            pilha->topo->tab[i][j] = '1';
        --i;
        ++j;
    }

    // nao pode outras rainhas na diagonal baixo-esquerda
    i = k + 1;
    j = l - 1;
    while (i < dim && j > -1) {
        if (pilha->topo->tab[i][j] == '0')
            pilha->topo->tab[i][j] = '1';
        ++i;
        --j;
    }

    // nao pode outras rainhas na diagonal baixo-direita
    i = k + 1;
    j = l + 1;
    while (i < dim && j < dim) {
        if (pilha->topo->tab[i][j] == '0')
            pilha->topo->tab[i][j] = '1';
        ++i;
        ++j;
    }
}

int insereParcial(Partial *parciais, unsigned long long parcial){

    // se achar parcial no vetor, retorna falso
    for (int i = 0; i < parciais->tam; ++i)
        if (parcial == parciais->vetor[i])
            return 0;

    // aloca espaco para a nova parcial no vetor
    if (parciais->tam > 0)
        parciais->vetor = (unsigned long long *) realloc(parciais->vetor, (parciais->tam + 1) * sizeof(unsigned long long));
    else
        parciais->vetor = (unsigned long long *) malloc(sizeof(unsigned long long));

    // insere nova parcial no vetor
    parciais->vetor[parciais->tam] = parcial;
    parciais->tam += 1;

    // retorna verdadeiro
    return 1;
}
