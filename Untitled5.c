#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>

#define MAX_ID_LENGTH 20
#define LARGURA 50
#define ALTURA 20
#define ARQUIVO_PLACAR "placar.txt"

/* Estruturas */
typedef struct {
    char nome[MAX_ID_LENGTH];
    int pontos;
    int fase;
} Jogador;

typedef struct {
    int x, y, dx, dy;
} Bola;

typedef struct {
    int x, y;
} Raquete;

/* Funções */
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void desenharParedes() {
    int i, j;
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            if (i == 0 || i == ALTURA - 1)
                printf("=");
            else if (j == 0 || j == LARGURA - 1)
                printf("|");
            else
                printf(" ");
        }
        printf("\n");
    }
}

void desenharRaquete(Raquete *raquete) {
    int i;
    for (i = 0; i < 4; i++) {
        printf("\033[%d;%dH", raquete->y + i, raquete->x);
        putchar('|');
    }
}

void desenharBola(Bola *bola) {
    printf("\033[%d;%dH", bola->y, bola->x);
    putchar('O');
}

void moverBola(Bola *bola) {
    bola->x += bola->dx;
    bola->y += bola->dy;

    if (bola->y == 1 || bola->y == ALTURA - 2)
        bola->dy = -bola->dy;

    if (bola->x == LARGURA - 1)
        bola->dx = -bola->dx;
}

void exibirGameOver(Jogador *jogador) {
    limparTela();
    printf("############## GAME OVER ##############\n");
    printf("Jogador: %s - Pontos: %d\n", jogador->nome, jogador->pontos);
    printf("Pressione '9' para voltar ao menu principal.\n");

    // Salva o placar no arquivo
    FILE *arquivo = fopen(ARQUIVO_PLACAR, "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%s %d\n", jogador->nome, jogador->pontos);
        fclose(arquivo);
    }

    char tecla;
    do {
        tecla = getch(); // Espera o jogador pressionar uma tecla
    } while (tecla != '9'); // Continua até que '9' seja pressionado
}

void movimentarRaquete(Raquete *raquete, char tecla) {
    if (tecla == 'w' && raquete->y > 1)
        raquete->y--;
    if (tecla == 's' && raquete->y < ALTURA - 5)
        raquete->y++;
}

void carregarConfiguracaoFase(int fase, int *velocidadeBola, int *pontosProximaFase, int *posRaquete) {
    FILE *arquivo;
    char nomeArquivo[15];
    sprintf(nomeArquivo, "fase%d.txt", fase + 1);

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        exit(1);
    }

    fscanf(arquivo, "%d %d %d", velocidadeBola, pontosProximaFase, posRaquete);
    fclose(arquivo);
}

void iniciarJogo(Jogador *jogador) {
    Bola *bola = malloc(sizeof(Bola));
    Raquete *raquete = malloc(sizeof(Raquete));
    int velocidadeBola, pontosProximaFase, posRaquete;
    char tecla;
    int pontosJogador = jogador->pontos;

    int fase = jogador->fase;

    while (1) {
        carregarConfiguracaoFase(fase, &velocidadeBola, &pontosProximaFase, &posRaquete);

        bola->x = LARGURA / 2;
        bola->y = ALTURA / 2;
        bola->dx = 1;
        bola->dy = 1;

        raquete->x = 2;
        raquete->y = posRaquete;

        while (1) {
            limparTela();
            desenharParedes();
            desenharRaquete(raquete);
            desenharBola(bola);

            moverBola(bola);

            if (bola->x == raquete->x + 1 && bola->y >= raquete->y && bola->y <= raquete->y + 3) {
                bola->dx = -bola->dx;
                pontosJogador++;
            }

            if (bola->x <= 0) {
                jogador->pontos = pontosJogador;
                exibirGameOver(jogador);
                free(bola);
                free(raquete);
                return;
            }

            printf("\033[%d;0H", ALTURA);
            printf("Jogador: %s | Pontos: %d | Fase: %d\n", jogador->nome, pontosJogador, fase + 1);

            if (_kbhit()) {
                tecla = getch();
                movimentarRaquete(raquete, tecla);
            }

            if (pontosJogador >= pontosProximaFase) {
                fase++;
                jogador->fase = fase;
                break;
            }

            usleep(50000 / velocidadeBola);
        }
    }
}

void exibirPlacar() {
    FILE *arquivo = fopen(ARQUIVO_PLACAR, "r");
    if (arquivo != NULL) {
        printf("\n############## Placar de Jogadores ##############\n");
        char nome[MAX_ID_LENGTH];
        int pontos;
        while (fscanf(arquivo, "%s %d", nome, &pontos) != EOF) {
            printf("Jogador: %s - Pontos: %d\n", nome, pontos);
        }
        fclose(arquivo);
    }
    printf("\nPressione qualquer tecla para voltar ao menu.\n");
    getch();
}

void exibirMenu(Jogador *jogador) {
    int opcao;
    while (1) {
        limparTela();
        printf("#############################\n");
        printf("###       Pong Menu       ###\n");
        printf("#############################\n");
        printf("1 - Iniciar Jogo\n");
        printf("2 - Placar de Jogadores\n");
        printf("3 - Fechar Jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite seu ID de jogador: ");
                scanf("%s", jogador->nome);
                jogador->pontos = 0;
                jogador->fase = 0;
                iniciarJogo(jogador);
                break;

            case 2:
                exibirPlacar();
                break;

            case 3:
                exit(0);

            default:
                break;
        }
    }
}

int main() {
    Jogador jogador;
    exibirMenu(&jogador);
    return 0;
}

