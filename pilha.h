/*
 * pilha.h
 * Definicao da estrutura de Pilha usada no backtracking
 * a pilha guarda as posicoes (linha, coluna) ja visitadas
 * durante a busca pelo caminho no labirinto
 */
#ifndef PILHA_H
#define PILHA_H

/* tamanho maximo da pilha (40x40 = 1600 celulas, com folga) */
#define MAX_PILHA 2000

/* representa uma posicao no labirinto */
typedef struct 
{
    int linha;
    int coluna;
} Posicao;

/* estrutura da pilha usando array estatico */
typedef struct 
{
    Posicao dados[MAX_PILHA];
    int topo;
} Pilha;

/* inicializa a pilha (topo = -1 indica vazia) */
void pilha_inicializar(Pilha *p);

/* retorna 1 se a pilha estiver vazia, 0 caso contrario */
int pilha_vazia(Pilha *p);

/* retorna 1 se a pilha estiver cheia, 0 caso contrario */
int pilha_cheia(Pilha *p);

/* empilha uma posicao, retorna 1 em sucesso, 0 se cheia */
int pilha_empilhar(Pilha *p, Posicao pos);

/* desempilha e retorna a posicao do topo */
Posicao pilha_desempilhar(Pilha *p);

#endif
