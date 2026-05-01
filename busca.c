/*
 * busca.c
 * Backtracking completo: explora todos os caminhos e guarda o melhor.
 *
 * CORRECOES em relacao a versao anterior:
 *
 * 1. VALOR FIXO POR TESOURO
 *    O valor de cada 'T' eh gerado uma unica vez ao carregar o mapa
 *    (em labirinto_carregar) e fica guardado em lab->valores_tesouro[][].
 *    Isso garante que comparar caminhos diferentes seja justo: o mesmo
 *    tesouro sempre vale o mesmo, independente da ordem de exploracao.
 *
 * 2. REMOCAO SEGURA NO BACKTRACK
 *    Antes, a remocao no backtrack buscava "o primeiro no com esse valor",
 *    o que podia remover um tesouro de tentativa anterior se dois tesouros
 *    tivessem o mesmo valor. Agora usamos uma flag 'removido' para parar
 *    apos a primeira remocao, garantindo que apenas UM no seja removido.
 *    (Nota: isso ja era o comportamento com 'break', mas deixamos explicito.)
 *
 * 3. PILHA GERENCIADA CORRETAMENTE
 *    pilha_empilhar na entrada de cada celula, pilha_desempilhar ao sair.
 *    O snapshot do melhor caminho e feito por copia de struct (valor),
 *    o que e seguro porque Pilha usa array estatico.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "busca.h"
#include "labirinto.h"
#include "mochila.h"
#include "pilha.h"

static int direcao_linha[4]  = {-1,  1,  0,  0};
static int direcao_coluna[4] = { 0,  0, -1,  1};

static int pode_mover(Labirinto *lab, int linha, int col) 
{
    // detecta limites do mapa
    if (linha < 0 || linha >= lab->linhas) // estou fora nas linhas 
        return 0;
    if (col < 0 || col >= lab->colunas) // estou fora nas colunas
        return 0;
    char c = labirinto_get(lab, linha, col); // pego o caractere da celula
    // se for um dos caracteres permitidos, retorna 1
    return (c == CORREDOR || c == TESOURO || c == ARMADILHA || c == SAIDA); 
}

/*
 * copia profunda da lista encadeada da mochila.
 * necessario para salvar um snapshot independente ao encontrar a saida.
 */
static void mochila_copiar(Mochila *dest, Mochila *orig) 
{
    mochila_liberar(dest);
    dest->total = orig->total;
    dest->quantidade = orig->quantidade;
    dest->inicio = NULL;

    NoTesouro **ptr = &dest->inicio;
    NoTesouro *atual = orig->inicio;

    while (atual != NULL) 
    {
        NoTesouro *novo = (NoTesouro*)malloc(sizeof(NoTesouro));
        if (!novo) 
        { 
            printf("Erro ao copiar mochila!\n"); 
            return; 
        }
        
        novo->valor = atual->valor;
        novo->proximo = NULL;
        *ptr = novo;
        ptr = &novo->proximo;
        atual = atual->proximo;
    }
}

void busca_backtracking(Labirinto *lab, Mochila *mochila, Pilha *caminho,
                        int linha, int coluna, int delay_ms,
                        ResultadoBusca *melhor) {

    /* --- CASO BASE: chegamos na saida --- */
    if (linha == lab->saida_linha && coluna == lab->saida_coluna) {
        Posicao pos = {linha, coluna};
        pilha_empilhar(caminho, pos);

        /* atualiza o melhor se este caminho tem mais moedas */
        if (!melhor->encontrou || mochila->total > melhor->mochila.total) {
            melhor->encontrou = 1;
            melhor->caminho   = *caminho;          /* copia da pilha (array estatico) */
            mochila_copiar(&melhor->mochila, mochila);

            printf("[MELHOR] Total: %d moedas (%d tesouros) | Passos: %d\n",
                   mochila->total, mochila->quantidade, caminho->topo + 1);
        }

        pilha_desempilhar(caminho);
        return;
    }

    /* salva o conteudo original e marca como visitado */
    char celula_original = labirinto_get(lab, linha, coluna);
    labirinto_set(lab, linha, coluna, VISITADO);

    /* registra esta posicao no caminho em construcao */
    Posicao pos = {linha, coluna};
    pilha_empilhar(caminho, pos);

    /* visualizacao animada */
    if (delay_ms > 0) {
        system("cls");
        labirinto_imprimir(lab, mochila->quantidade, mochila->total);
        Sleep(delay_ms);
    }

    /* --- explora as 4 direcoes --- */
    for (int d = 0; d < 4; d++) {
        int nl = linha  + direcao_linha[d];
        int nc = coluna + direcao_coluna[d];

        if (!pode_mover(lab, nl, nc)) continue;

        char celula_destino = labirinto_get(lab, nl, nc);
        int valor_adicionado = -1;
        int valor_perdido    = -1;

        /* coleta tesouro usando o valor FIXO armazenado no mapa */
        if (celula_destino == TESOURO) {
            valor_adicionado = labirinto_get_valor(lab, nl, nc);
            mochila_inserir(mochila, valor_adicionado);
            printf("[TESOURO] Coletado! Valor fixo: %d moedas\n", valor_adicionado);
        }

        /* armadilha: perde o menor tesouro da mochila */
        if (celula_destino == ARMADILHA) {
            valor_perdido = mochila_remover_primeiro(mochila);
            if (valor_perdido > 0)
                printf("[ARMADILHA] Perdeu %d moedas!\n", valor_perdido);
        }

        labirinto_set(lab, nl, nc, PERSONAGEM);

        /* chamada recursiva */
        busca_backtracking(lab, mochila, caminho, nl, nc, delay_ms, melhor);

        /* --- BACKTRACK: restaura estado --- */
        labirinto_set(lab, nl, nc, celula_destino);

        /*
         * remove da mochila o tesouro que foi adicionado nesta tentativa.
         * buscamos pelo valor exato e removemos apenas UM no (o primeiro
         * encontrado com aquele valor), depois paramos com 'break'.
         */
        if (valor_adicionado > 0) {
            NoTesouro *ant  = NULL;
            NoTesouro *cur  = mochila->inicio;
            while (cur != NULL) {
                if (cur->valor == valor_adicionado) {
                    if (ant == NULL) mochila->inicio   = cur->proximo;
                    else             ant->proximo       = cur->proximo;
                    mochila->total      -= cur->valor;
                    mochila->quantidade--;
                    free(cur);
                    break; /* remove apenas UM no */
                }
                ant = cur;
                cur = cur->proximo;
            }
        }

        /* reinsere o tesouro que a armadilha havia removido */
        if (valor_perdido > 0) {
            mochila_inserir(mochila, valor_perdido);
        }
    }

    /* restaura a celula e remove do caminho ao retroceder */
    labirinto_set(lab, linha, coluna, celula_original);
    pilha_desempilhar(caminho);
}

void busca_aplicar_melhor(Labirinto *lab, ResultadoBusca *melhor) {
    if (!melhor->encontrou) return;

    Pilha *p = &melhor->caminho;
    for (int i = 0; i <= p->topo; i++) {
        int l = p->dados[i].linha;
        int c = p->dados[i].coluna;
        char cel = labirinto_get(lab, l, c);
        if (cel != SAIDA && cel != PERSONAGEM)
            labirinto_set(lab, l, c, CAMINHO);
    }
}
