#include <stdio.h>

// Definições de constantes
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define VALOR_AGUA 0
#define VALOR_NAVIO 3
#define VALOR_HABILIDADE 5

#define TAMANHO_HABILIDADE 5

// Inicializa o tabuleiro com água
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++)
        for (int j = 0; j < TAMANHO_TABULEIRO; j++)
            tabuleiro[i][j] = VALOR_AGUA;
}

// Exibe o tabuleiro
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("   ");
    for (char letra = 'A'; letra < 'A' + TAMANHO_TABULEIRO; letra++)
        printf("%c ", letra);
    printf("\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] == VALOR_AGUA) printf("~ ");          // Água
            else if (tabuleiro[i][j] == VALOR_NAVIO) printf("N ");    // Navio
            else if (tabuleiro[i][j] == VALOR_HABILIDADE) printf("* "); // Área habilidade
            else printf("? ");
        }
        printf("\n");
    }
}
int posicionarNavioHorizontal(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna) {
    if (coluna + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        if (tabuleiro[linha][coluna + i] != VALOR_AGUA) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        tabuleiro[linha][coluna + i] = VALOR_NAVIO;
    return 1;
}

int posicionarNavioVertical(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna) {
    if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        if (tabuleiro[linha + i][coluna] != VALOR_AGUA) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        tabuleiro[linha + i][coluna] = VALOR_NAVIO;
    return 1;
}

int posicionarNavioDiagonalPrincipal(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna) {
    if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO || coluna + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        if (tabuleiro[linha + i][coluna + i] != VALOR_AGUA) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        tabuleiro[linha + i][coluna + i] = VALOR_NAVIO;
    return 1;
}

int posicionarNavioDiagonalSecundaria(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna) {
    if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO || coluna - (TAMANHO_NAVIO - 1) < 0) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        if (tabuleiro[linha + i][coluna - i] != VALOR_AGUA) return 0;
    for (int i = 0; i < TAMANHO_NAVIO; i++)
        tabuleiro[linha + i][coluna - i] = VALOR_NAVIO;
    return 1;
}

// Função para criar matriz da habilidade Cone (tamanho 5x5)
void criarHabilidadeCone(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Cone com ponto no topo e expandindo para baixo
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            int centro = TAMANHO_HABILIDADE / 2;
            if (j >= centro - i && j <= centro + i) habilidade[i][j] = 1;
            else habilidade[i][j] = 0;
        }
    }
}

// Função para criar matriz da habilidade Cruz (5x5)
void criarHabilidadeCruz(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (i == centro || j == centro) habilidade[i][j] = 1;
            else habilidade[i][j] = 0;
        }
    }
}

// Função para criar matriz da habilidade Octaedro (5x5)
void criarHabilidadeOctaedro(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            int distancia = abs(i - centro) + abs(j - centro);
            if (distancia <= 2) habilidade[i][j] = 1;
            else habilidade[i][j] = 0;
        }
    }
}

// Função para sobrepor matriz habilidade no tabuleiro
void sobreporHabilidadeNoTabuleiro(
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
    int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE],
    int linhaOrigem, int colOrigem
) {
    int meio = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (habilidade[i][j] == 1) {
                int linhaTab = linhaOrigem - meio + i;
                int colTab = colOrigem - meio + j;
                // Verifica limites do tabuleiro
                if (linhaTab >= 0 && linhaTab < TAMANHO_TABULEIRO &&
                    colTab >= 0 && colTab < TAMANHO_TABULEIRO) {
                    // Marca área de habilidade
                    if (tabuleiro[linhaTab][colTab] == VALOR_AGUA)
                        tabuleiro[linhaTab][colTab] = VALOR_HABILIDADE;
                }
            }
        }
    }
}

int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    inicializarTabuleiro(tabuleiro);

    // Posiciona navios 
    posicionarNavioHorizontal(tabuleiro, 1, 4);
    posicionarNavioVertical(tabuleiro, 7, 9);
    posicionarNavioDiagonalPrincipal(tabuleiro, 0, 0);
    posicionarNavioDiagonalSecundaria(tabuleiro, 0, 9);

    // Cria matrizes das habilidades
    int cone[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int cruz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int octaedro[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];

    criarHabilidadeCone(cone);
    criarHabilidadeCruz(cruz);
    criarHabilidadeOctaedro(octaedro);

    // Define pontos de origem para cada habilidade no tabuleiro
    int origemConeLinha = 2, origemConeCol = 2;
    int origemCruzLinha = 5, origemCruzCol = 5;
    int origemOctaedroLinha = 7, origemOctaedroCol = 4;

    // Sobrepõe as habilidades no tabuleiro
    sobreporHabilidadeNoTabuleiro(tabuleiro, cone, origemConeLinha, origemConeCol);
    sobreporHabilidadeNoTabuleiro(tabuleiro, cruz, origemCruzLinha, origemCruzCol);
    sobreporHabilidadeNoTabuleiro(tabuleiro, octaedro, origemOctaedroLinha, origemOctaedroCol);

    // Exibe o tabuleiro final
    exibirTabuleiro(tabuleiro);

    return 0;
}