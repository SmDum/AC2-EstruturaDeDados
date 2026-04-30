/*
 * labirinto.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "labirinto.h"

#define COR_RESET     "\033[0m"
#define COR_PAREDE    "\033[90m"
#define COR_PESSOA    "\033[93m"
#define COR_TESOURO   "\033[33m"
#define COR_ARMADILHA "\033[31m"
#define COR_SAIDA     "\033[92m"
#define COR_VISITADO  "\033[34m"
#define COR_CAMINHO   "\033[96m"

int labirinto_carregar(Labirinto *lab, const char *caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        printf("[ERRO] Arquivo '%s' nao encontrado!\n", caminho_arquivo);
        return 0;
    }

    if (fscanf(arquivo, "%dx%d\n", &lab->linhas, &lab->colunas) != 2) {
        printf("[ERRO] Formato invalido. A primeira linha deve ser 'LxC'.\n");
        fclose(arquivo);
        return 0;
    }

    if (lab->linhas > MAX_LINHAS || lab->colunas > MAX_COLUNAS) {
        printf("[ERRO] Labirinto maior que o suportado (%dx%d).\n", MAX_LINHAS, MAX_COLUNAS);
        fclose(arquivo);
        return 0;
    }

    lab->inicio_linha  = -1;
    lab->inicio_coluna = -1;
    lab->saida_linha   = -1;
    lab->saida_coluna  = -1;

    /* zera a matriz de valores */
    memset(lab->valores_tesouro, 0, sizeof(lab->valores_tesouro));

    char buffer[MAX_COLUNAS + 2];
    for (int i = 0; i < lab->linhas; i++) {
        if (fgets(buffer, sizeof(buffer), arquivo) == NULL) {
            for (int j = 0; j < lab->colunas; j++)
                labirinto_set(lab, i, j, PAREDE);
            continue;
        }

        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') { buffer[len - 1] = '\0'; len--; }

        for (int j = 0; j < lab->colunas; j++) {
            char c = (j < len) ? buffer[j] : PAREDE;
            labirinto_set(lab, i, j, c);

            if (c == TESOURO) {
                /* valor fixo gerado uma unica vez ao carregar o mapa */
                lab->valores_tesouro[i * lab->colunas + j] = (rand() % 100) + 1;
            }
            if (c == PERSONAGEM) { lab->inicio_linha = i; lab->inicio_coluna = j; }
            else if (c == SAIDA) { lab->saida_linha  = i; lab->saida_coluna  = j; }
        }
    }

    fclose(arquivo);

    if (lab->inicio_linha == -1) { printf("[ERRO] 'P' nao encontrado!\n"); return 0; }
    if (lab->saida_linha  == -1) { printf("[ERRO] 'S' nao encontrado!\n"); return 0; }

    return 1;
}

char labirinto_get(Labirinto *lab, int linha, int col) {
    return lab->celulas[linha * lab->colunas + col];
}

void labirinto_set(Labirinto *lab, int linha, int col, char c) {
    lab->celulas[linha * lab->colunas + col] = c;
}

/* retorna o valor fixo do tesouro naquela posicao (0 se nao for tesouro) */
int labirinto_get_valor(Labirinto *lab, int linha, int col) {
    return lab->valores_tesouro[linha * lab->colunas + col];
}

void labirinto_imprimir(Labirinto *lab, int tesouros, int total_moedas) {
    system("cls");

    printf("<------------------------------------------------------------------------------------------------\n");
    printf("    LABIRINTO DE DADOS - AC2    \n");
    printf("------------------------------------------------------------------------------------------------>\n\n");

    for (int i = 0; i < lab->linhas; i++) {
        for (int j = 0; j < lab->colunas; j++) {
            char c = labirinto_get(lab, i, j);
            switch (c) {
                case PAREDE:    printf(COR_PAREDE    "#" COR_RESET); break;
                case PERSONAGEM:printf(COR_PESSOA    "P" COR_RESET); break;
                case TESOURO:   printf(COR_TESOURO   "T" COR_RESET); break;
                case ARMADILHA: printf(COR_ARMADILHA "A" COR_RESET); break;
                case SAIDA:     printf(COR_SAIDA     "S" COR_RESET); break;
                case VISITADO:  printf(COR_VISITADO  "." COR_RESET); break;
                case CAMINHO:   printf(COR_CAMINHO   "*" COR_RESET); break;
                default:        printf("%c", c);                      break;
            }
        }
        printf("\n");
    }

    printf("\n");
    printf("Tesouros coletados: %d | Total: %d moedas\n", tesouros, total_moedas);
    printf("Legenda: P=Personagem  T=Tesouro  A=Armadilha  S=Saida  .=Visitado  *=Caminho\n");
}

void labirinto_salvar_solucao(Labirinto *lab, const char *caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (arquivo == NULL) {
        printf("[ERRO] Nao foi possivel criar '%s'!\n", caminho_arquivo);
        return;
    }

    fprintf(arquivo, "--- SOLUCAO DO LABIRINTO ---\n\n");
    for (int i = 0; i < lab->linhas; i++) {
        for (int j = 0; j < lab->colunas; j++)
            fprintf(arquivo, "%c", labirinto_get(lab, i, j));
        fprintf(arquivo, "\n");
    }
    fprintf(arquivo, "\nLegenda: * = caminho percorrido, . = celulas visitadas\n");
    fclose(arquivo);

    printf("\n[OK] Solucao salva em '%s'\n", caminho_arquivo);
}
