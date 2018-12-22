#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t mut; // mutex do resultado
pthread_mutex_t mutParc; // mutex das parciais

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

typedef struct {
    int k; // linha da rainha inicial
    int l; // coluna da rainha inicial
    int dim; // copia da dimensao
    int queens; // copia do numero de rainhas
} thread_arg;

unsigned long long resultado; // resultado global
Partial *parciais; // vetor global de parciais

void * calculaRainhas(void *ptr);
void insereNo(Stack *pilha);
void removeNo(Stack *pilha, int dim);
void colocaRainha(Stack *pilha, int dim, int k, int l);
int insereParcial(unsigned long long parcial);

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

    int mesmaLinha; // numero de repeticoes na mesma linha
    int restoThreads; // numero de celulas que sobraram
    int numThreads; // numero de threads
    int linhas; // numero de linhas que precisa ser percorrido
    int i, j, k, l; // contadores de for
    resultado = 0; // inicializa somatorio final

    // inicializa struct com vetor de parciais
    parciais = (Partial *) malloc(sizeof(Partial));
    parciais->vetor = NULL;
    parciais->tam = 0;

    // calcula numero de threads e valores para os for
    if (dim < 4) {
        numThreads = dim;
        mesmaLinha = 1;
        restoThreads = 0;
    } else {
        numThreads = 4;
        mesmaLinha = dim / 4;
        restoThreads = dim % 4;
    }

    pthread_t thread[numThreads]; // inicializa threads

    // inicializa mutexes
    pthread_mutex_init(&mut, NULL);
    pthread_mutex_init(&mutParc, NULL);

    // declara ponteiro para argumentos
    thread_arg * args;

    // calcula linhas a percorrer
    linhas = dim - queens + 1;

    for (i = 0; i < linhas; ++i) {
        l = 0;
        for (j = 0; j < mesmaLinha; ++j) {
            // calcula primeiras rainhas iniciais da linha
            for (k = 0; k < numThreads; ++k) {
                // aloca argumentos
                args = (thread_arg *) malloc(sizeof(thread_arg));
                args->dim = dim;
                args->queens = queens;
                args->k = i;
                args->l = l;
                // cria threads
                pthread_create(&thread[k], NULL, calculaRainhas, (void*) args);
                ++l;
            }
            // junta resultados das primeiras rainhas da linha
            for (k = 0; k < numThreads; ++k)
                pthread_join(thread[k], NULL);
        }
        // se tiver posicoes sobrando
        if (restoThreads != 0) {
            // calcula rainhas iniciais
            for (k = 0; k < restoThreads; ++k) {
                // aloca argumentos
                args = (thread_arg *) malloc(sizeof(thread_arg));
                args->dim = dim;
                args->queens = queens;
                args->k = i;
                args->l = l;
                // cria threads
                pthread_create(&thread[k], NULL, calculaRainhas, (void*) args);
                ++l;
            }
            // junta resultados
            for (k = 0; k < restoThreads; ++k)
                pthread_join(thread[k], NULL);
        }
    }

    return resultado;
}

void * calculaRainhas(void *ptr) {

    // recebe argumentos
    thread_arg * args = (thread_arg *) ptr;

    // monta pilha com possibilidades de posicionamentos no tabuleiro
    Stack * pilha;
    pilha = (Stack *) malloc(sizeof(Stack));
    pilha->topo = NULL;
    pilha->tam = 0;

    int i, j, x, y; // contadores de for
    int pot; // potencia do calculo
    int exclui; // flag de exclusao do no
    int parou; // flag de interrupcao de for aninhado
    int flagParcial; // flag de retorno de verificacao das parciais
    unsigned long long parcial; // soma parcial de posicao de rainhas

    // insere primeiro no na pilha
    insereNo(pilha);

    // aloca dinamicamente o tabuleiro e zera-o
    pilha->topo->tab = (char **) malloc(args->dim * sizeof(char *));
        for (i = 0; i < args->dim; ++i)
            pilha->topo->tab[i] = (char *) malloc(args->dim * sizeof(char));
        for (i = 0; i < args->dim; ++i)
            for (j = 0; j < args->dim; ++j)
                pilha->topo->tab[i][j] = '0';

    // coloca primeira rainha no tabuleiro
    colocaRainha(pilha, args->dim, args->k, args->l);

    // enquanto a pilha nao esvaziar
    while (pilha->topo != NULL) {

        // se o tamanho da pilha for o mesmo que o numero de
        // rainhas a inserir, contabiliza a parcial
        if (pilha->tam == args->queens) {
            // inicializa valores necessarios
            parcial = 0;
            pot = 0;
            // percorre o tabuleiro atual
            for (i = 0; i < args->dim; ++i) {
                for (j = 0; j < args->dim; ++j) {
                    // se encontra uma rainha, soma na parcial
                    if (pilha->topo->tab[i][j] == '2')
                        parcial = parcial + pow(2, pot);
                    // incrementa potencia da posicao
                    ++pot;
                }
            }
            // sessao critica das parciais
            pthread_mutex_lock(&mutParc);
            flagParcial = insereParcial(parcial);
            pthread_mutex_unlock(&mutParc);
            // se a parcial calculada nao esta no vetor,
            // soma no resultado a ser retornado
            if (flagParcial) {
                // sessao critica do resultado
                pthread_mutex_lock(&mut);
                resultado = resultado + parcial;
                pthread_mutex_unlock(&mut);
            }
            // remove no da pilha
            removeNo(pilha, args->dim);
        } else {
            // inicializa flags
            exclui = 1;
            parou = 0;
            // percorre o tabuleiro atual
            for (i = 0; i < args->dim; ++i) {
                for (j = 0; j < args->dim; ++j) {
                    // se encontra um espaco
                    if (pilha->topo->tab[i][j] == '0' && parou == 0) {
                        // muda a flag
                        exclui = 0;
                        // insere novo no
                        insereNo(pilha);
                        // aloca dinamicamente o tabuleiro do novo no
                        pilha->topo->tab = (char **) malloc(args->dim * sizeof(char *));
                        for (x = 0; x < args->dim; ++x)
                            pilha->topo->tab[x] = (char *) malloc(args->dim * sizeof(char));
                        // novo no recebe copia do tabuleiro atual
                        for (x = 0; x < args->dim; ++x)
                            for (y = 0; y < args->dim; ++y)
                                pilha->topo->tab[x][y] = pilha->topo->prox->tab[x][y];
                        // coloca rainha no novo no
                        colocaRainha(pilha, args->dim, i, j);
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
                removeNo(pilha, args->dim);
        }
    }

    // libera memoria alocada dinamicamente
    free(pilha);
    free(args);

    return (void *) 0;
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

int insereParcial(unsigned long long parcial){

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
