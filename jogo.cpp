#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <locale.h>

using namespace std;

#define LARGURA 70
#define ALTURA 25

// Função auxiliar
void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Classe que representa um jogador (ou goleiro)
class Jogador {
public:
    string nome;
    int pontuacao = 0;

    Jogador(string nome) : nome(nome) {}

    void adicionarPonto() {
        pontuacao++;
    }

    void reiniciarPontos() {
        pontuacao = 0;
    }
};

// Classe que representa o campo de jogo e desenha a interface
class Campo {
private:
    int bolaX = 20;
    int bolaY = ALTURA / 2;
    int goleiroX = LARGURA - 15;
    int goleiroY = ALTURA / 2;

public:
    void desenharCampo(Jogador &jogador, Jogador &goleiro) {
        system("cls");
        cout << "\n\t         === PLACAR: " << jogador.nome << " " << jogador.pontuacao
             << " x " << goleiro.pontuacao << " " << goleiro.nome << " ===\n\n";

        for (int j = 0; j < ALTURA; j++) {
            gotoxy(15, j);
            cout << "|";
            gotoxy(LARGURA - 5, j);
            if (j >= 5 && j <= ALTURA - 5)
                cout << "]";
            else
                cout << "|";
        }

        for (int j = 5; j <= ALTURA - 5; j++) {
            for (int i = LARGURA - 4; i < LARGURA - 1; i++) {
                gotoxy(i, j);
                cout << "=";
            }
        }

        desenharJogador(jogador);
        desenharGoleiro(goleiro);
    }

    void desenharJogador(Jogador &jogador) {
        gotoxy(5, ALTURA / 2 - 2);
        cout << "  O";
        gotoxy(5, ALTURA / 2 - 1);
        cout << " /|\\"; 
        gotoxy(5, ALTURA / 2);
        cout << " / \\"; 
        gotoxy(5, ALTURA / 2 - 4);
        cout << jogador.nome;
    }

    void desenharGoleiro(Jogador &goleiro) {
        gotoxy(goleiroX, goleiroY - 2);
        cout << " O ";
        gotoxy(goleiroX - 2, goleiroY - 1);
        cout << "===|===";
        gotoxy(goleiroX, goleiroY);
        cout << " | ";
        gotoxy(goleiroX, goleiroY + 1);
        cout << "/ \\"; 
        gotoxy(goleiroX - goleiro.nome.length() / 2, goleiroY - 4);
        cout << goleiro.nome;
    }

    void animarChute(int ladoChute, int defesa, bool ehGoleiro, Jogador &jogador, Jogador &goleiro) {
        int destinoX = LARGURA - 10;
        int destinoY = (ladoChute == 1) ? 7 : (ladoChute == 2) ? ALTURA / 2 : ALTURA - 7;

        // Posicionamento do goleiro
        if (ehGoleiro) {
            goleiroY = (defesa == 1) ? ALTURA - 6 : (defesa == 2) ? ALTURA / 2 : 6;
        } else {
            goleiroY = (defesa == 1) ? 6 : (defesa == 2) ? ALTURA / 2 : ALTURA - 6;
        }

        desenharCampo(jogador, goleiro);

        // Animação da bola
        while (bolaX < destinoX) {
            gotoxy(bolaX, bolaY); cout << " ";
            bolaX += 2;
            if (bolaY < destinoY) bolaY++;
            else if (bolaY > destinoY) bolaY--;

            // Verificar defesa
            if (bolaX >= goleiroX - 5 && abs(bolaY - goleiroY) <= 4) {
                goleiro.adicionarPonto();
                gotoxy(LARGURA / 2, ALTURA + 2);
                cout << "DEFESAAA!";
                resetBola();
                Sleep(2000);
                return;
            }

            gotoxy(bolaX, bolaY); cout << "O";
            desenharGoleiro(goleiro);
            Sleep(50);
        }

        jogador.adicionarPonto();
        gotoxy(LARGURA / 2, ALTURA + 2);
        cout << "GOOOOOOOL!";
        Sleep(2000);
        resetBola();
    }

    void resetBola() {
        bolaX = 20;
        bolaY = ALTURA / 2;
    }
};

// Classe que controla o jogo
class Jogo {
private:
    Campo campo;
    Jogador jogador;
    Jogador goleiro;
    bool modoJogador;

public:
    Jogo(string nomeJogador, string nomeGoleiro, bool modoJogador)
        : jogador(nomeJogador), goleiro(nomeGoleiro), modoJogador(modoJogador) {}

    void executar() {
        char jogarNovamente;

        do {
            // Iniciar o jogo
            while (jogador.pontuacao < 2 && goleiro.pontuacao < 2) {
                campo.desenharCampo(jogador, goleiro);

                int chuteOuDefesa;
                if (modoJogador) {
                    gotoxy(0, 25);
                    cout << "                                               "; // limpa a linha
                    gotoxy(0, 25);
                    cout << jogador.nome << ", escolha o lado para chutar (1-Esq | 2-Meio | 3-Dir): ";
                    cin >> chuteOuDefesa;
                    int defesaAleatoria = rand() % 3 + 1;
                    campo.animarChute(chuteOuDefesa, defesaAleatoria, false, jogador, goleiro);
                } else {
                    gotoxy(0, 25);
                    cout << "                                               ";
                    gotoxy(0, 25);
                    cout << goleiro.nome << ", escolha o lado para defender (1-Esq | 2-Meio | 3-Dir): ";
                    cin >> chuteOuDefesa;
                    int chuteAleatorio = rand() % 3 + 1;
                    campo.animarChute(chuteAleatorio, chuteOuDefesa, true, jogador, goleiro);
                }
            }

            mostrarResultado();

            // Pergunta se deseja jogar novamente
            cout << "\n\nDeseja jogar novamente? (s/n): ";
            cin >> jogarNovamente;

            if (jogarNovamente == 's' || jogarNovamente == 'S') {
                // Reinicia as pontuações
                jogador.reiniciarPontos();
                goleiro.reiniciarPontos();

                // Deixa o jogador escolher novamente o modo
                int modo;
                cout << "\nEscolha com qual deseja jogar novamente: Jogador(1) ou Goleiro(2): ";
                cin >> modo;
                modoJogador = (modo == 1);  // Muda o modo de jogo
            }

        } while (jogarNovamente == 's' || jogarNovamente == 'S');  // Se a resposta for sim, reinicia a partida.

        cout << "\nObrigado por jogar!" << endl;
    }

    void mostrarResultado() {
        cout << "\n\n";
        if (jogador.pontuacao == 2)
            cout << jogador.nome << " venceu!";
        else
            cout << goleiro.nome << " venceu!";
    }
};

void mostrarRegras() {
    system("cls");
    cout << "======== REGRAS DO JOGO ========\n\n";
    cout << "==== JOGADOR ====\n";
    cout << "1 - Chutar para ESQUERDA\n";
    cout << "2 - Chutar para o MEIO\n";
    cout << "3 - Chutar para DIREITA\n";
    cout << "Goleiro defende aleatoriamente.\n\n";

    cout << "==== GOLEIRO ====\n";
    cout << "1 - Defender ESQUERDA\n";
    cout << "2 - Defender MEIO\n";
    cout << "3 - Defender DIREITA\n";
    cout << "Jogador chuta aleatoriamente.\n\n";

    cout << "==== COMO VENCER ====\n";
    cout << "Melhor de 3: quem fizer 2 gols primeiro, vence!\n\n";
    system("pause");
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand((unsigned)time(NULL));

    mostrarRegras();

    string nomeJ, nomeG;
    int modo;

    system("cls");
    cout << "Digite o nome do Jogador: ";
    cin >> nomeJ;
    cout << "Digite o nome do Goleiro: ";
    cin >> nomeG;

    cout << "Com qual deseja jogar? Jogador(1) ou Goleiro(2): ";
    cin >> modo;

    bool modoJogador = (modo == 1);

    Jogo jogo(nomeJ, nomeG, modoJogador);
    jogo.executar();

    return 0;
}

