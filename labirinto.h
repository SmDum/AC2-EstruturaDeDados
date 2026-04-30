/*
 * labirinto.h
 */
#ifndef LABIRINTO_H
#define LABIRINTO_H

#define MAX_LINHAS  40
#define MAX_COLUNAS 40

/* simbolos do mapa */
#define PAREDE     '#'
#define CORREDOR   ' '
#define PERSONAGEM 'P'
#define TESOURO    'T'
#define ARMADILHA  'A'
#define SAIDA      'S'
#define VISITADO   '.'
#define CAMINHO    '*'

typedef struct {
    char celulas[MAX_LINHAS * MAX_COLUNAS];
    int  valores_tesouro[MAX_LINHAS * MAX_COLUNAS]; /* valor fixo de cada tesouro */
    int  linhas;
    int  colunas;
    int  inicio_linha;
    int  inicio_coluna;
    int  saida_linha;
    int  saida_coluna;
} Labirinto;

int  labirinto_carregar(Labirinto *lab, const char *caminho_arquivo);
char labirinto_get(Labirinto *lab, int linha, int col);
void labirinto_set(Labirinto *lab, int linha, int col, char c);
int  labirinto_get_valor(Labirinto *lab, int linha, int col);
void labirinto_imprimir(Labirinto *lab, int tesouros, int total_moedas);
void labirinto_salvar_solucao(Labirinto *lab, const char *caminho_arquivo);

#endif
