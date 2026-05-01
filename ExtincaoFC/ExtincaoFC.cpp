/**********************************************************************************
// Platformer (Código Fonte)
//
// Criação:     05 Out 2011
// Atualização: 02 Set 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Exemplo de jogo estilo platforma
//
**********************************************************************************/

#include "Engine.h"
#include "ExtincaoFC.h"
#include "Resources.h"

// -----------------------------------------------------------------------------

Scene * ExtincaoFC::scene = nullptr;
Audio * ExtincaoFC::audio = nullptr;

// -----------------------------------------------------------------------------

void ExtincaoFC::Init()
{
    // cria cena do jogo
    scene = new Scene();

    consolas = new Font("Resources/consolas12.png");
    consolas->Spacing("Resources/consolas12.dat");

    smallFonts = new Font("Resources/small_fonts_12.png");
    smallFonts->Spacing("Resources/small_fonts_12.dat");

	// cria sistema de áudio
	audio = new Audio();

    

    // pano de fundo do jogo
    backg = new Sprite("Resources/background.png");

	// cria gramado
	Grass* grass = new Grass(window->CenterX(), window->Height());
    scene->Add(grass, STATIC);

	// cria jogadores
	player1 = new Player(TREX, 500.0f, window->Height() - 220.0f);
	scene->Add(player1, MOVING);

	player2 = new Player(TRICERATOPS, window->Width() - 500.0f, window->Height() - 220.0f);
	scene->Add(player2, MOVING);

	// cria metas
	goal1 = new Goal(player2, 130.0f, window->Height() - 320.0f);
	scene->Add(goal1, STATIC);

	goal2 = new Goal(player1, window->Width() - 130.0f, window->Height() - 320.0f);
	scene->Add(goal2, STATIC);

	// cria sensores de gol
	sensor1 = new GoalSensor(goal1);
	scene->Add(sensor1, STATIC);

	sensor2 = new GoalSensor(goal2);
	scene->Add(sensor2, STATIC);

	// cria bola
	ball = new Ball();
	scene->Add(ball, MOVING);

	// inicializa variáveis de controle do gol e reset
    lastTotalScoreBoard = 0;
    waitingReset = false;
    resetTimer = 0.0f;

	victimSpecie = -1; // nenhuma espécie é a vítima no início do jogo
}

// ------------------------------------------------------------------------------

int ExtincaoFC::BallDirection()
{
    if (victimSpecie == TREX) return -1; 

	if (victimSpecie == TRICERATOPS) return 1;

	return 0; // direção neutra se nenhuma espécie é a vítima
}

// ------------------------------------------------------------------------------

void ExtincaoFC::ResetMatch()
{
    player1->Reset();
    player2->Reset();
    ball->Reset(BallDirection());

	sensor1->Reset();
	sensor2->Reset();
}

// ------------------------------------------------------------------------------

void ExtincaoFC::ProcessInputs()
{
    if (window->KeyDown(VK_ESCAPE)) {
        window->Close();
    }

    if (window->KeyPress('B')) {
        viewBBox = !viewBBox;
    }
}

// ------------------------------------------------------------------------------

void ExtincaoFC::ManageMatchState()
{
    // 1. O RELÓGIO DA PARTIDA
    // Só contabiliza o tempo se a bola estiver rolando (não estamos comemorando gol)
    if (!waitingReset)
    {
        matchTimer += gameTime;

        // Se o tempo estourou no meio da jogada, o juiz apita o fim imediatamente!
        if (matchTimer >= MATCH_TIME_LIMIT)
        {
            TreatMatchEnding();
            return; // Sai da função para congelar o jogo
        }
    }

    uint currentTotalScoreBoard = player1->Score() + player2->Score();

    // Detectou um gol novo
    if (currentTotalScoreBoard > lastTotalScoreBoard)
    {
		// Verifica qual jogador marcou o gol e define a espécie vítima
        if (player1->Score() > lastTrexScore) {
            victimSpecie = TRICERATOPS;
        }
        else {
            victimSpecie = TREX;
        }

		// Salva o placar atual para comparação no próximo gol
        lastTrexScore = player1->Score();
        lastTriceratopsScore = player2->Score();

        lastTotalScoreBoard = currentTotalScoreBoard;
        waitingReset = true;
        resetTimer = 0.0f;
    }

    // Cronômetro de comemoração
    if (waitingReset)
    {
        resetTimer += gameTime;

        if (resetTimer >= TIME_TO_RESET)
        {
            waitingReset = false;

            if (!TreatMatchEnding()) {
                ResetMatch();
            }
        }
    }
}

// ------------------------------------------------------------------------------

bool ExtincaoFC::TreatMatchEnding()
{
    // ==========================================
    // CONDIÇÃO 1: LIMITE DE GOLS ATINGIDO
    // ==========================================
    if (lastTrexScore >= SCORE_TO_WIN) {
        winner = TREX; 
        Finalize();
        return true;
    }

    if (lastTriceratopsScore >= SCORE_TO_WIN) {
        winner = TRICERATOPS;
        Finalize();
        return true;
    }

    // ==========================================
    // CONDIÇÃO 2: TEMPO ESGOTADO
    // ==========================================
    if (matchTimer >= MATCH_TIME_LIMIT)
    {
        // Descobre quem tem mais gols
        if (lastTrexScore > lastTriceratopsScore) {
            winner = TREX;
        }
        else if (lastTriceratopsScore > lastTrexScore) {
            winner = TRICERATOPS;
        }
        else {
            // EMPATE! 
            // Garanta que 'winner' fique nulo ou crie um estado específico para isso
            winner = 99;
        }

        Finalize();
        return true;
    }

    // Se não atingiu o limite de gols e o tempo não acabou, o jogo segue!
    return false;
}

// ------------------------------------------------------------------------------

void ExtincaoFC::Update()
{
    ProcessInputs();       // Lida com teclas pressionadas
    ManageMatchState();    // Lida com placares e cronômetros

    scene->Update();
    scene->CollisionDetection();
}

// ------------------------------------------------------------------------------

void ExtincaoFC::Draw()
{
	backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    scene->Draw();

	float timeLeft = MATCH_TIME_LIMIT - matchTimer;

	Color textColor = timeLeft <= 10 ? Color{ 0.8, 0, 0, 1 } : Color{ 1, 1, 1, 1 }; // vermelho se faltar menos de 10 segundos, caso contrário branco

	string timeString = GetMatchTimeString();

	smallFonts->Draw(window->CenterX() + 10, 50.0f, timeString, textColor, Layer::FRONT, 3.0f);

    if (viewBBox)
    {
        scene->DrawBBox();
    }
}

string ExtincaoFC::GetMatchTimeString() const
{
    // 1. Calcula os segundos restantes (e impede de ficar negativo)
    int timeLeft = (int)(MATCH_TIME_LIMIT - matchTimer);
    if (timeLeft < 0) timeLeft = 0;

    // 2. Extrai os minutos e segundos da matemática básica
    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;

    // 3. Monta a string do texto (ex: "1:05" ou "0:09")
    std::string timeString = std::to_string(minutes) + ":";

    // Se os segundos forem menores que 10, coloca um '0' na frente para não ficar "0:9"
    if (seconds < 10) {
        timeString += "0";
    }
    timeString += to_string(seconds);

    return timeString;
}

// ------------------------------------------------------------------------------

void ExtincaoFC::Finalize()
{
    delete scene;
	delete backg;
	delete audio;
	delete player1;
	delete player2;
	delete goal1;
	delete goal2;
	delete sensor1;
	delete sensor2;
	delete ball;
	delete consolas;
	delete smallFonts;
}


// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // cria engine
    Engine * engine = new Engine();

    // configura motor
    engine->window->Mode(WINDOWED);
    engine->window->Size(1600, 860);
    engine->window->Color(80, 80, 50);
    engine->window->Title("ExtincaoFC");
    engine->window->Icon(IDI_ICON);
    //engine->window->Cursor(IDC_CURSOR);
    //engine->graphics->VSync(true);
    
    // inicia o jogo
    engine->Start(new ExtincaoFC());

    // destrói engine e jogo
    delete engine;
}

// ----------------------------------------------------------------------------

