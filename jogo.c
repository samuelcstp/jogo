#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <locale.h>

#define LARGURA 70
#define ALTURA 25

int main();
void goleiro();

// Variáveis de posição da bola e do goleiro
int bolaX = 20;
int bolaY = ALTURA/2;
int goleiroX = LARGURA-15;
int goleiroY = ALTURA/2;

// Contadores de gols
int contJogador = 0;
int contGoleiro = 0;

// Nomes dos participantes
char nomeJogador[20] = "Jogador";
char nomeGoleiro[20] = "Goleiro";

// Move o cursor no console para uma posição (x, y)
void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Desenha o boneco do jogador no lado esquerdo do campo
void desenharJogador() {
    gotoxy(5, ALTURA/2 - 2);
    printf("  O");
    gotoxy(5, ALTURA/2 - 1);
    printf(" /|\\");
    gotoxy(5, ALTURA/2);
    printf(" / \\");
    gotoxy(5, ALTURA/2 - 4);
    printf("%s", nomeJogador); // Exibe nome do jogador acima do boneco
}

// Desenha o goleiro (no lado direito)
void desenharGoleiro() {
    gotoxy(goleiroX, goleiroY - 2);
    printf(" O ");
    gotoxy(goleiroX-2, goleiroY - 1);
    printf("===|===");
    gotoxy(goleiroX, goleiroY);
    printf(" | ");
    gotoxy(goleiroX, goleiroY + 1);
    printf("/ \\");
    gotoxy(goleiroX - strlen(nomeGoleiro)/2, goleiroY - 4);
    printf("%s", nomeGoleiro); // Exibe nome do goleiro acima
}

// Desenha o campo com as marcações e bonecos
void desenharCampo() {
    system("cls");
    int i, j;
    printf("\n		      === PLACAR: %s %d x %d %s ===\n\n", nomeJogador, contJogador, contGoleiro, nomeGoleiro);
    
    for(j = 0; j < ALTURA; j++) {
        gotoxy(15, j);
        printf("|"); // linha lateral esquerda
        
        gotoxy(LARGURA-5, j); // Trave
        if(j >= 5 && j <= ALTURA-5)
            printf("]");
        else
            printf("|");
    }
    
    // Travessão
    for(j = 5; j <= ALTURA-5; j++) {
        for(i = LARGURA-4; i < LARGURA-1; i++) {
            gotoxy(i, j);
            printf("=");
        }
    }

    desenharJogador();
    desenharGoleiro();
}

// Faz a animação da bola indo em direção ao gol e verifica se foi gol ou defesa
void animarChute(int ladoChute, int nAl, int ehGoleiro) {
    int destinoX = LARGURA-10; // Posição final da bola no X (próxima do gol)
    int destinoY;

    // Define o Y de destino da bola conforme lado escolhido
    switch(ladoChute) {
        case 1: destinoY = 7; break;         
        case 2: destinoY = ALTURA/2; break;
        case 3: destinoY = ALTURA-7; break;
    }

    // Movimenta o goleiro para tentar defender
    if (ehGoleiro) {
        switch(nAl) {
            case 1: goleiroY = ALTURA-6; break;
            case 2: goleiroY = ALTURA/2; break;
            case 3: goleiroY = 6; break;
        }
    } else {
        switch(nAl) {
            case 1: goleiroY = 6; break;
            case 2: goleiroY = ALTURA/2; break;
            case 3: goleiroY = ALTURA-6; break;
        }
    }

    desenharCampo();

    // Movimento da bola até o destino
    while(bolaX < destinoX) {
        gotoxy(bolaX, bolaY);
        printf(" ");

        bolaX += 2;
        if(bolaY < destinoY) bolaY++;
        else if(bolaY > destinoY) bolaY--;

        // Verifica defesa
        if(bolaX >= goleiroX-5 && abs(bolaY - goleiroY) <= 4) {
            contGoleiro++;
            gotoxy(LARGURA/2, ALTURA+2);
            printf("DEFESASAAAAAAA!");
            break;
        }

        gotoxy(bolaX, bolaY);
        printf("O"); // bola
        desenharGoleiro();
        Sleep(50); // pausa para animação
    }

    // Se passou do goleiro, é gol
    if(bolaX >= destinoX) {
        contJogador++;
        gotoxy(LARGURA/2, ALTURA+2);
        printf("GOLAZOOOO!");
    }

    Sleep(2000);
    bolaX = 20;
    bolaY = ALTURA/2;
}

// Mostra o placar atualizado
void placar() {
    gotoxy(LARGURA/2-10, ALTURA+4);
    printf("====Placar====");
    gotoxy(LARGURA/2-10, ALTURA+5);
    printf("%s: %i", nomeJogador, contJogador);
    gotoxy(LARGURA/2-10, ALTURA+6);
    printf("%s: %i", nomeGoleiro, contGoleiro);
    gotoxy(0, ALTURA+8);
}

// Verifica se o jogador ganhou
void resultadoJogador() {
    if(contJogador == 2)
        printf("\n%s ganhou!!!", nomeJogador);
    else if (contGoleiro == 2)
        printf("\n%s perdeu!!! HaHa!", nomeJogador);    
}

// Verifica se o goleiro ganhou
void resultadoGoleiro() {
    if(contGoleiro == 2)
        printf("\n%s ganhou!!!", nomeGoleiro);
    else if (contJogador == 2)
        printf("\n%s perdeu!!! HaHa!", nomeGoleiro);    
}

// Modo de jogo: Jogador chuta
void jogador() {
    printf("Digite o nome do Jogador (max 19 caracteres): ");
    scanf("%19s", nomeJogador);
    printf("Digite o nome do Goleiro (max 19 caracteres): ");
    scanf("%19s", nomeGoleiro);
    
    do {
        desenharCampo();
        placar();
        int ladoChute;
        printf("============%s============\n", nomeJogador);
        printf("Selecione um lado para chutar: (1-Esq | 2-Mei | 3-Dir)\n");
        scanf("%i", &ladoChute);
        int nAl = (rand() % 3) + 1; // Posição do goleiro aleatória
        animarChute(ladoChute, nAl, 0);
    } while (contJogador < 2 && contGoleiro < 2);

    resultadoJogador();

    // Pergunta se quer jogar novamente
    int again;
    printf("\n\nDeseja jogar novamente? (1-sim  2-não):\n");
    scanf("%i", &again);

    if(again==1) {
        contJogador=0;
        contGoleiro=0;
        system("pause");
        system("cls");
        printf("Com qual deseja jogar:?\nJogador|Goleiro (1|2): ");
        int op;
        scanf("%i", &op);
        if(op == 1) jogador();
        else goleiro();
    } else {
        printf("\nFinalizando....");
        exit(0);
    }
}

// Modo de jogo: Goleiro defende
void goleiro() {
    printf("Digite o nome do Jogador (max 19 caracteres): ");
    scanf("%19s", nomeJogador);
    printf("Digite o nome do Goleiro (max 19 caracteres): ");
    scanf("%19s", nomeGoleiro);
    
    do {
        desenharCampo();
        placar();
        int ladoChute;
        printf("============%s============\n", nomeGoleiro);
        printf("Selecione um lado para defender: (1-Esq | 2-Mei | 3-Dir)\n");
        scanf("%i", &ladoChute);
        int nAl = (rand() % 3) + 1; // Chute aleatório do jogador
        animarChute(nAl, ladoChute, 1);
    } while (contJogador < 2 && contGoleiro < 2);

    resultadoGoleiro();

    // Deseja jogar novamente?
    int again;
    printf("\n\nDeseja jogar novamente? (1-sim  2-não):\n");
    scanf("%i", &again);

    if(again==1) {
        contJogador=0;
        contGoleiro=0;
        system("pause");
        system("cls");
        printf("Com qual deseja jogar:?\nJogador|Goleiro (1|2): ");
        int op;
        scanf("%i", &op);
        if(op == 1) jogador();
        else goleiro();
    } else {
        printf("\nFinalizando....");
        exit(0);
    }
}

// Mostra as regras e como jogar
void mostrarRegras() {
    system("cls");
    printf("========REGRAS DO JOGO========\n\n");
    printf("====JOGADOR====\n");
    printf("1 - Chutar para ESQUERDA do goleiro\n");
    printf("2 - Chutar para o MEIO do gol\n");
    printf("3 - Chutar para DIREITA do goleiro\n");
    printf("O goleiro defenderá aleatoriamente em uma das 3 posições\n");

    printf("\n====GOLEIRO====\n");
    printf("1 - Defender seu lado ESQUERDO\n");
    printf("2 - Defender o MEIO do gol\n");
    printf("3 - Defender seu lado DIREITO\n");
    printf("O jogador chutará aleatoriamente em uma das 3 posições\n");

    printf("\nCOMO GANHAR:\n");
    printf("Melhor de 3 - O primeiro a fazer 2 pontos vence!\n\n");
    system("pause");
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese_Brazil");
    srand(time(NULL)); // Inicializa gerador de aleatórios

    int op;
    mostrarRegras(); // Mostra as instruções do jogo

    system("cls");
    printf("Com qual deseja jogar:?\n");
    printf("Jogador|Goleiro (1|2): ");
    scanf("%i", &op);

    if(op == 1)
        jogador();
    else
        goleiro();

    getchar(); getchar(); // Espera para fechar
    return 0;
}

