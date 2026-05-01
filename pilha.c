/*
 * pilha.c
 * Implementacao das funcoes da pilha
 * usamos um array estatico para evitar alocacao dinamica desnecessaria,
 * ja que o tamanho maximo do labirinto eh conhecido (40x40).
 */

#include <stdio.h>
#include "pilha.h"

/* inicializa a pilha definindo topo como -1 (convencao de pilha vazia) */
void pilha_inicializar(Pilha *p) {
    p->topo = -1;
}

/* verifica se o topo eh -1 (nenhum elemento inserido ainda) */
int pilha_vazia(Pilha *p) {
    return p->topo == -1;
}

/* verifica se o proximo indice ultrapassa o limite do array */
int pilha_cheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

/*
 * incrementa o topo e armazena a posicao
 * retorna 0 se a pilha estiver cheia (erro de overflow)
 */
int pilha_empilhar(Pilha *p, Posicao pos) {
    if (pilha_cheia(p)) {
        printf("[ERRO] Pilha cheia! Caminho muito longo.\n");
        return 0;
    }
    p->topo++;
    p->dados[p->topo] = pos;
    return 1;
}

/*
 * remove e retorna o elemento do topo
 * ATENCAO: sempre verifique pilha_vazia() antes de chamar esta funcao.
 */
Posicao pilha_desempilhar(Pilha *p) {
    Posicao pos = {-1, -1}; /* posicao invalida como sentinela */
    if (pilha_vazia(p)) {
        printf("[ERRO] Tentativa de desempilhar pilha vazia!\n");
        return pos;
    }
    pos = p->dados[p->topo];
    p->topo--;
    return pos;
}

