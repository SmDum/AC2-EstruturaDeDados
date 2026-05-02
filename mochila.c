/*
 * mochila.c
 * Implementacao da mochila como lista encadeada ordenada (crescente)
 * a insercao ordenada substitui a necessidade de um algoritmo de
 * ordenacao separado: cada tesouro ja entra na posicao certa
 */

#include <stdio.h>
#include <stdlib.h>
#include "mochila.h"

/* define a mochila como vazia */
void mochila_inicializar(Mochila *m) 
{
    m->inicio = NULL;
    m->total = 0;
    m->quantidade = 0;
}

int mochila_vazia(Mochila *m) 
{
    return m->inicio == NULL;
}

/*
 * insercao em lista encadeada ordenada (ordem crescente de valor)
 *
 * percorre a lista ate encontrar o ponto de insercao correto:
 *   - Se o novo valor for menor que todos = insere na cabeca
 *   - Caso contrario = insere entre o no atual e o proximo
 *
 * complexidade: O(n), onde n eh o numero de tesouros
 */
void mochila_inserir(Mochila *m, int valor) 
{
    /* aloca memoria para o novo no */
    NoTesouro *novo = (NoTesouro *)malloc(sizeof(NoTesouro));
    if (novo == NULL) 
    {
        printf("[ERRO] Falha ao alocar memoria para tesouro!\n");
        return;
    }
    novo->valor    = valor;
    novo->proximo  = NULL;

    /* caso 1: lista vazia OU novo valor eh menor que o primeiro */
    if (m->inicio == NULL || valor < m->inicio->valor)
    {
        novo->proximo = m->inicio;
        m->inicio = novo;
    } 
    else 
    {
        /* caso 2: percorre ate achar a posicao certa */
        NoTesouro *atual = m->inicio;
        while (atual->proximo != NULL && atual->proximo->valor <= valor) 
        {
            atual = atual->proximo;
        }
        /* insere novo entre 'atual' e 'atual->proximo' */
        novo->proximo  = atual->proximo;
        atual->proximo = novo;
    }

    m->total += valor;
    m->quantidade++;
}

/*
 * remove o primeiro no da lista (menor valor)
 * retorna o valor do tesouro perdido, ou -1 se vazia
 */
int mochila_remover_primeiro(Mochila *m) 
{
    if (mochila_vazia(m)) 
    {
        printf("[AVISO] Armadilha acionada, mas a mochila esta vazia! Nada foi perdido.\n");
        return -1;
    }

    NoTesouro *removido = m->inicio;
    int valor = removido->valor;

    m->inicio = removido->proximo; /* avancaa a cabeca da lista */
    free(removido);                /* libera a memoria do no removido */

    m->total -= valor;
    m->quantidade--;

    return valor;
}

/* imprime todos os tesouros da mochila em ordem */
void mochila_imprimir(Mochila *m) 
{
    if (mochila_vazia(m)) 
    {
        printf("  [ mochila vazia ]\n");
        return;
    }

    NoTesouro *atual = m->inicio;
    int i = 1;
    while (atual != NULL) 
    {
        printf("  [%d] %d moedas\n", i++, atual->valor);
        atual = atual->proximo;
    }
    printf("  TOTAL: %d moedas | Itens: %d\n", m->total, m->quantidade);
}

/* libera todos os nos da lista (evita memory leak) */
void mochila_liberar(Mochila *m) 
{
    NoTesouro *atual = m->inicio;
    while (atual != NULL) 
    {
        NoTesouro *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    m->inicio    = NULL;
    m->total     = 0;
    m->quantidade = 0;
}
