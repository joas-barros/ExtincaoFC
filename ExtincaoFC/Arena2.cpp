/**********************************************************************************
// Arena2 (Código Fonte)
//
// Descrição:   Arena 2 do jogo Extinção FC
//              Herda Arena1. Lados dos jogadores são invertidos:
//              - Triceratops (Player2) fica na ESQUERDA, olhando para a DIREITA
//              - T-Rex (Player1) fica na DIREITA, olhando para a ESQUERDA
//
**********************************************************************************/

#include "Arena2.h"
#include "Engine.h"
#include "ExtincaoFC.h"
#include "Arena3.h"

// -----------------------------------------------------------------------------

void Arena2::Init()
{
    // 1. Inicializa recursos base
    Arena1::Init();

    // 2. Limpeza total de objetos da Arena 1
    scene->Remove(goal1, STATIC);
    scene->Remove(goal2, STATIC);
    scene->Remove(sensor1, STATIC);
    scene->Remove(sensor2, STATIC);
    delete goal1; delete goal2;
    delete sensor1; delete sensor2;

    // 3. RECRIAR GOLS COM AS BBOXES NOVAS
    // Goal 1 (ESQUERDA): Defesa do Triceratops (P2)
    goal1 = new Goal(player1, 130.0f, window->Height() - 320.0f);
    delete goal1->sprite;
    goal1->sprite = new Sprite("Resources/goal_triceratops_arena2.png");
    // CARREGA O ARQUIVO TXT QUE VOCÊ CRIOU PARA A ESQUERDA[cite: 1]
    goal1->ReloadBBox("Resources/goal_triceratops_arena2_bbox.txt");

    // Goal 2 (DIREITA): Defesa do T-Rex (P1)
    goal2 = new Goal(player2, window->Width() - 130.0f, window->Height() - 320.0f);
    delete goal2->sprite;
    goal2->sprite = new Sprite("Resources/goal_trex_arena2.png");
    // CARREGA O ARQUIVO TXT QUE VOCÊ CRIOU PARA A DIREITA[cite: 2]
    goal2->ReloadBBox("Resources/goal_trex_arena2_bbox.txt");

    sensor1 = new GoalSensor(goal1);
    sensor2 = new GoalSensor(goal2);
    scene->Add(goal1, STATIC);
    scene->Add(goal2, STATIC);
    scene->Add(sensor1, STATIC);
    scene->Add(sensor2, STATIC);

    // 4. POSICIONAMENTO E BBOX DOS JOGADORES (Ambos flipados para o centro)

    // Triceratops (P2) na ESQUERDA: Agora ele precisa de flipped = true 
    // para a cabeça ir para a esquerda (acompanhando o sprite da Arena 2)
    player2->SetInitialPos(500.0f, window->Height() - 220.0f);
    delete player2->sprite;
    player2->sprite = new Sprite("Resources/triceratops_arena2.png");
    player2->scaleX = 1.0f;
    player2->drawBBox(TRICERATOPS, true); // MUDANÇA AQUI: flipped = true

    // T-Rex (P1) na DIREITA: flipped = true (cabeça para a esquerda)
    player1->SetInitialPos(window->Width() - 500.0f, window->Height() - 220.0f);
    delete player1->sprite;
    player1->sprite = new Sprite("Resources/trex_arena2.png");
    player1->scaleX = -1.0f;
    player1->drawBBox(TREX, true);

    // 5. Fundo da Arena 2
    delete backg;
    backg = new Sprite("Resources/background_arena2.png");

    ResetMatch();
}

// -----------------------------------------------------------------------------

int Arena2::BallDirection()
{
    // Na Arena2 os lados são invertidos em relação à Arena1:
    // - Triceratops está na ESQUERDA  → após sofrer gol, bola vai para a ESQUERDA (-1)
    // - T-Rex está na DIREITA         → após sofrer gol, bola vai para a DIREITA  (+1)

    if (victimSpecie == TRICERATOPS) return -1;
    if (victimSpecie == TREX)        return  1;

    return 0;
}

// -----------------------------------------------------------------------------

void Arena2::ResetMatch()
{
    // Chama o reset da Arena1 (repositiciona jogadores e bola, inicia kickoff)
    // O kickoff usará o BallDirection() desta classe (virtual override)
    Arena1::ResetMatch();
}

// -----------------------------------------------------------------------------

void Arena2::ProcessInputs()
{
    if (window->KeyDown(VK_ESCAPE)) window->Close();

    if (window->KeyPress('B')) viewBBox = !viewBBox;

    // Tecla 'N' para avançar manualmente para a Arena3
     if (window->KeyPress('N')) Engine::Next<Arena3>();
}

// -----------------------------------------------------------------------------

void Arena2::Update()
{
    ProcessInputs();
    ManageMatchState();

    if (gScene == nullptr) return;

    scene->Update();
    scene->CollisionDetection();
}

// -----------------------------------------------------------------------------

void Arena2::DrawSscoreBoard()
{
    Color green = { 0, 1, 0, 1 };
    Color white = { 1, 1, 1, 1 };
    Color starColor = { 1, 1, 0, 1 };

    // LADO ESQUERDO: TRICERATOPS (Player2) — fisicamente na esquerda na Arena2
    smallFonts->Draw(60.0f, 50.0f, "TRICERATOPS", green, Layer::FRONT, 3.0f);
    smallFonts->Draw(100.0f, 125.0f, to_string(player2->Score()), white, Layer::FRONT, 4.0f);

    // Estrela se o Triceratops ganhou a Arena1
    if (gTotalWinsTriceratops > 0)
        smallFonts->Draw(110.0f, 190.0f, "*", starColor, Layer::FRONT, 2.0f);

    // LADO DIREITO: T-REX (Player1) — fisicamente na direita na Arena2
    float trexX = window->Width() - 200.0f;
    smallFonts->Draw(trexX, 50.0f, "T-REX", green, Layer::FRONT, 3.0f);
    smallFonts->Draw(window->Width() - 150.0f, 125.0f, to_string(player1->Score()), white, Layer::FRONT, 4.0f);

    // Estrela se o T-Rex ganhou a Arena1
    if (gTotalWinsTrex > 0)
        smallFonts->Draw(window->Width() - 140.0f, 190.0f, "*", starColor, Layer::FRONT, 2.0f);
}

// -----------------------------------------------------------------------------

void Arena2::Draw()
{
    backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    scene->Draw();

    DrawMatchTime();
    DrawSscoreBoard(); // chama o DrawSscoreBoard() da Arena2, não da Arena1
    DrawKickoffCountdown();

    if (viewBBox)
        scene->DrawBBox();
}

// -----------------------------------------------------------------------------
