#include "Engine.h"
#include "Resources.h" 
#include "Home.h"
#include "ExtincaoFC.h"
#include "Arena1.h"
#include "Arena2.h"
#include "Arena3.h"
#include "GameOver.h"

void Arena3::Init()
{
    // 1. Inicializa como na Arena 1 (lados originais: P1 na Esquerda, P2 na Direita)
    Arena1::Init();

    // 2. Troca o fundo para o definitivo da Arena 3
    delete backg;
    backg = new Sprite("Resources/background_arena3.png");

    // O kickoff usará o BallDirection() da Arena 1 (Padrão)
    ResetMatch();
}

// -----------------------------------------------------------------------------
void Arena3::DrawSscoreBoard()
{
    Color green = { 0, 1, 0, 1 };
    Color white = { 1, 1, 1, 1 };
    Color starColor = { 1, 1, 0, 1 };

    // --- LADO ESQUERDO: T-REX (Player 1) ---
    // Na Arena 3, o T-Rex volta a ser o Player 1 na esquerda
    smallFonts->Draw(60.0f, 50.0f, "T-REX", green, Layer::FRONT, 3.0f);
    smallFonts->Draw(100.0f, 125.0f, to_string(player1->Score()), white, Layer::FRONT, 4.0f);

    // Como é a final, o T-Rex obrigatoriamente tem 1 estrela (da Arena 1 ou 2)
    if (gTotalWinsTrex > 0)
    {
        smallFonts->Draw(110.0f, 190.0f, "*", starColor, Layer::FRONT, 2.0f);
    }

    // --- LADO DIREITO: TRICERATOPS (Player 2) ---
    // Na Arena 3, o Triceratops volta a ser o Player 2 na direita
    float triX = window->Width() - 300.0f; // Ajuste para o nome longo
    smallFonts->Draw(triX, 50.0f, "TRICERATOPS", green, Layer::FRONT, 3.0f);
    smallFonts->Draw(window->Width() - 150.0f, 125.0f, to_string(player2->Score()), white, Layer::FRONT, 4.0f);

    // O Triceratops também tem 1 estrela garantida
    if (gTotalWinsTriceratops > 0)
    {
        smallFonts->Draw(window->Width() - 140.0f, 190.0f, "*", starColor, Layer::FRONT, 2.0f);
    }
}

// -----------------------------------------------------------------------------

void Arena3::Draw()
{
    backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    scene->Draw();

    DrawMatchTime();
    DrawSscoreBoard(); // Chama a versão da Arena 3 definida acima
    DrawKickoffCountdown();

    if (viewBBox)
        scene->DrawBBox();
}

// -----------------------------------------------------------------------------

bool Arena3::TreatMatchEnding()
{
    if (matchTimer <= 0 || player1->Score() >= SCORE_TO_WIN || player2->Score() >= SCORE_TO_WIN)
    {
        

        // --- ACUMULA OS GOLS DA ARENA 3 ---
        gTotalGolsTrex += player1->Score();
        gTotalGolsTriceratops += player2->Score();

        // Define o vencedor da última arena
        if (player1->Score() > player2->Score()) gTotalWinsTrex++;
        else if (player2->Score() > player1->Score()) gTotalWinsTriceratops++;

        // FIM DO JOGO: Vai para a tela final
        Engine::Next<GameOver>();
        return true;
    }
    return false;
}