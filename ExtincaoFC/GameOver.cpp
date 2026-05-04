#include "GameOver.h"
#include "ExtincaoFC.h"
#include "Engine.h"
#include "Player.h"
#include "home.h"

// Usamos uma variável global ou estática para o ponteiro do vencedor
static Player* winnerObj = nullptr;

void GameOver::Init()
{
    // 1. Inicializa a base (Fontes e sons)
    Arena1::Init();

    gAudio->Play(FINAL_WHISTLE);

    // 2. Define o nome e cria o objeto do vencedor baseado nas vitórias globais
    if (gTotalWinsTrex > gTotalWinsTriceratops)
    {
        winnerText = "T-REX: O GRANDE CAMPEAO!";
        // Criamos o objeto do T-Rex centralizado
        winnerObj = new Player(TREX, window->CenterX(), window->CenterY() + 80);
    }
    else if (gTotalWinsTriceratops > gTotalWinsTrex)
    {
        winnerText = "TRICERATOPS: O GRANDE CAMPEAO!";
        // Criamos o objeto do Triceratops centralizado
        winnerObj = new Player(TRICERATOPS, window->CenterX(), window->CenterY() + 80);
    }
    else
    {
        winnerText = "TEMOS UM EMPATE TECNICO!";
        winnerObj = new Player(TREX, window->CenterX(), window->CenterY() + 80);
    }

    // Garante que ele fique parado e visível
    if (winnerObj) {
        winnerObj->velX = 0;
        winnerObj->velY = 0;
    }

    // 3. Monta as strings de placar usando as globais que agora estão funcionando
    scoreText = "ARENAS VENCIDAS: " + std::to_string(gTotalWinsTrex) + " - " + std::to_string(gTotalWinsTriceratops);
    goalsText = "TOTAL DE GOLS: " + std::to_string(gTotalGolsTrex) + " - " + std::to_string(gTotalGolsTriceratops);

    // 4. Carrega o fundo
    delete backg;
    backg = new Sprite("Resources/background_gameover.png");
}

void GameOver::Update()
{
    if (window->KeyPress(VK_RETURN))
        window->Close();

    if (window->KeyPress('R'))
    {
        // Reset total antes de voltar
        gTotalWinsTrex = 0;
        gTotalWinsTriceratops = 0;
        gTotalGolsTrex = 0;
        gTotalGolsTriceratops = 0;
        gCurrentMatch = 1;
        Engine::Next<Home>();
    }

    if (winnerObj)
        winnerObj->Update();
}

void GameOver::Draw()
{
    if (backg)
        backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);

    // Se ele parou de aparecer, vamos forçar o Draw na camada FRONT
    if (winnerObj)
        winnerObj->Draw();

	Color darkGreen = { 0, 0.5f, 0, 1 };
    Color white = { 1, 1, 1, 1 };

    if (smallFonts) {
        // Exibe o Nome do Vencedor em destaque
        smallFonts->Draw(window->CenterX() - 450, 90, winnerText, darkGreen, Layer::FRONT, 3.0f);

        // Exibe o Placar de Arenas
        smallFonts->Draw(window->CenterX() - 200, 180, scoreText, darkGreen, Layer::FRONT, 2.0f);

        // Exibe o Placar de Gols Acumulados
        smallFonts->Draw(window->CenterX() - 180, 240, goalsText, darkGreen, Layer::FRONT, 1.0f);

        smallFonts->Draw(window->CenterX() - 280, window->Height() - 90, "PRESSIONE ENTER PARA SAIR", white, Layer::FRONT, 1.5f);
        smallFonts->Draw(window->CenterX() - 280, window->Height() - 60, "PRESSIONE 'R' PARA REINICIAR", white, Layer::FRONT, 1.0f);
    }
}

void GameOver::Finalize()
{
    if (winnerObj) {
        delete winnerObj;
        winnerObj = nullptr;
    }
    Arena1::Finalize();
}