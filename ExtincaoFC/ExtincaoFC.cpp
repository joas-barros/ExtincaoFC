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
    // cria sistema de áudio
    audio = new Audio();
    audio->Add(GOAL_SCREAM, "Resources/goal.wav");
    audio->Add(INITIAL_WHISTLE, "Resources/initial_whistle.wav");
    audio->Add(FINAL_WHISTLE, "Resources/final_whistle.wav");
    audio->Add(KICK, "Resources/kick.wav");
    audio->Add(JUMP, "Resources/jump.wav");

    // cria cena do jogo
    scene = new Scene();

    smallFonts72 = new Font("Resources/small_fonts_72.png");
    smallFonts72->Spacing("Resources/small_fonts_72.dat");

    smallFonts = new Font("Resources/small_fonts_12.png");
    smallFonts->Spacing("Resources/small_fonts_12.dat");

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

    isKickoff = true;
    kickoffTimer = 0.0f;
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
    ball->Reset();

	sensor1->Reset();
	sensor2->Reset();

    isKickoff = true;
    kickoffTimer = 0.0f;
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

void ExtincaoFC::ProcessKickoff()
{
    kickoffTimer += gameTime;

    if (kickoffTimer >= KICKOFF_TIME)
    {
        isKickoff = false;
        player1->canMove = true;
        player2->canMove = true;

        ball->Kickoff(BallDirection());
		audio->Play(INITIAL_WHISTLE);
    }
}

bool ExtincaoFC::ProcessMatchTimer()
{
    if (!waitingReset)
    {
        matchTimer += gameTime;
        if (matchTimer >= MATCH_TIME_LIMIT)
        {
            TreatMatchEnding();
            return true; // Avisa o Maestro que o tempo esgotou!
        }
    }
    return false; // O jogo continua
}

void ExtincaoFC::ProcessGoalCelebration()
{
    uint currentTotalScoreBoard = player1->Score() + player2->Score();

    // Detectou o gol
    if (currentTotalScoreBoard > lastTotalScoreBoard)
    {
        if (player1->Score() > lastTrexScore) victimSpecie = TRICERATOPS;
        else victimSpecie = TREX;

        lastTrexScore = player1->Score();
        lastTriceratopsScore = player2->Score();
        lastTotalScoreBoard = currentTotalScoreBoard;

        waitingReset = true;
        resetTimer = 0.0f;
    }

    // Tempo da bola balançando na rede
    if (waitingReset)
    {
        resetTimer += gameTime;
        if (resetTimer >= TIME_TO_RESET)
        {
            waitingReset = false;

            if (!TreatMatchEnding())
            {
                ResetMatch();
            }
        }
    }
}

void ExtincaoFC::ManageMatchState()
{
    if (isKickoff)
    {
        ProcessKickoff();
        return;
    }

	if (ProcessMatchTimer()) return;

    ProcessGoalCelebration();
}

// ------------------------------------------------------------------------------

bool ExtincaoFC::TreatMatchEnding()
{
    // ==========================================
    // CONDIÇÃO 1: LIMITE DE GOLS ATINGIDO
    // ==========================================
    if (lastTrexScore >= SCORE_TO_WIN) {
        winner = TREX;

		audio->Play(FINAL_WHISTLE);
        Finalize();
        return true;
    }

    if (lastTriceratopsScore >= SCORE_TO_WIN) {
        winner = TRICERATOPS;
		audio->Play(FINAL_WHISTLE);
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

        audio->Play(FINAL_WHISTLE);
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

    DrawMatchTime();
    DrawSscoreBoard();
	DrawKickoffCountdown();

    if (viewBBox)
    {
        scene->DrawBBox();
    }
}

void ExtincaoFC::DrawMatchTime()
{
    float timeLeft = MATCH_TIME_LIMIT - matchTimer;

    Color textColor = timeLeft <= 10 ? Color{ 0.8, 0, 0, 1 } : Color{ 1, 1, 1, 1 }; // vermelho se faltar menos de 10 segundos, caso contrário branco

    string timeString = GetMatchTimeString();

    smallFonts->Draw(window->CenterX() + 10, 50.0f, timeString, textColor, Layer::FRONT, 3.0f);
}

void ExtincaoFC::DrawSscoreBoard()
{
	Color playerColor = { 0, 1, 0, 1 };

	Color scoreColor = { 1, 1, 1, 1 };

	// Desenha o nome dos jogadores
	smallFonts->Draw(60.0f, 50.0f, "T-REX", playerColor, Layer::FRONT, 3.0f);
	smallFonts->Draw(window->Width() - 300.0f, 50.0f, "TRICERATOPS", playerColor, Layer::FRONT, 3.0f);

	// Desenha os gols de cada jogador
	smallFonts->Draw(80.0f, 125.0f, to_string(player1->Score()), scoreColor, Layer::FRONT, 4.0f);
	smallFonts->Draw(window->Width(), 125.0f, to_string(player2->Score()), scoreColor, Layer::FRONT, 4.0f);
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

void ExtincaoFC::DrawKickoffCountdown()
{
    if (isKickoff)
    {
		Color yellowRed = { 1, 0.5f, 0, 1 }; 
        int countdown = (int)(KICKOFF_TIME - kickoffTimer) + 1; 
        string countdownText = to_string(countdown);
        smallFonts72->Draw(window->CenterX() + 150.0f, window->CenterY(), countdownText, yellowRed, Layer::FRONT, 3.0f);
    }
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
	delete smallFonts72;
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

