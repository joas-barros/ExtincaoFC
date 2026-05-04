/**********************************************************************************
// Platformer (C�digo Fonte)
//
// Cria��o:     05 Out 2011
// Atualiza��o: 02 Set 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Exemplo de jogo estilo platforma
//
**********************************************************************************/

#include "Engine.h"
#include "Arena1.h"
#include "Resources.h" 
#include "Home.h"
#include "ExtincaoFC.h"
#include "Arena2.h"
#include "GameOver.h"

// -----------------------------------------------------------------------------

Scene* Arena1::scene = nullptr;
Audio* Arena1::audio = nullptr;

// -----------------------------------------------------------------------------

void Arena1::Init()
{
    // cria sistema de �udio
    audio = new Audio();
    gAudio = audio;
    audio->Add(GOAL_SCREAM, "Resources/goal.wav");
    audio->Add(INITIAL_WHISTLE, "Resources/initial_whistle.wav");
    audio->Add(FINAL_WHISTLE, "Resources/final_whistle.wav");
    audio->Add(KICK, "Resources/kick.wav");
    audio->Add(JUMP, "Resources/jump.wav");

    // cria cena do jogo
    scene = new Scene();
    gScene = scene;

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

    // inicializa vari�veis de controle do gol e reset
    lastTotalScoreBoard = 0;
    waitingReset = false;
    resetTimer = 0.0f;

    isKickoff = true;
    kickoffTimer = 0.0f;
    victimSpecie = -1; // nenhuma esp�cie � a v�tima no in�cio do jogo
}

// ------------------------------------------------------------------------------

int Arena1::BallDirection()
{
    if (victimSpecie == TREX) return -1;

    if (victimSpecie == TRICERATOPS) return 1;

    return 0; // dire��o neutra se nenhuma esp�cie � a v�tima
}

// ------------------------------------------------------------------------------

void Arena1::ResetMatch()
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

void Arena1::ProcessInputs()
{
    if (window->KeyDown(VK_ESCAPE)) {
        window->Close();
    }

    if (window->KeyPress('B')) {
        viewBBox = !viewBBox;
    }

    if (window->KeyPress('N'))
    {
        // passa manualmente para o próximo nível
        Engine::Next<Arena2>();
    }
}

// ------------------------------------------------------------------------------

void Arena1::ProcessKickoff()
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

bool Arena1::ProcessMatchTimer()
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

void Arena1::ProcessGoalCelebration()
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

    // Tempo da bola balan�ando na rede
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

void Arena1::ManageMatchState()
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

bool Arena1::TreatMatchEnding()
{
    bool matchEnded = false;
    uint matchWinner = 99; // 99 para empate ou indefinido

    // Verifica vencedor por Gols
    if (lastTrexScore >= SCORE_TO_WIN) {
        matchWinner = TREX;
        matchEnded = true;
    }
    else if (lastTriceratopsScore >= SCORE_TO_WIN) {
        matchWinner = TRICERATOPS;
        matchEnded = true;
    }
    // Verifica vencedor por Tempo
    else if (matchTimer >= MATCH_TIME_LIMIT) {
        if (lastTrexScore > lastTriceratopsScore) matchWinner = TREX;
        else if (lastTriceratopsScore > lastTrexScore) matchWinner = TRICERATOPS;
        else matchWinner = 99; // Empate
        matchEnded = true;
    }

    if (matchEnded) {
        // 1. Toca o apito usando o gAudio global
        gAudio->Play(FINAL_WHISTLE);

        // 2. ACUMULA OS GOLS PARA O PLACAR GERAL (A "buniteza" que faltava)
        gTotalGolsTrex += lastTrexScore;
        gTotalGolsTriceratops += lastTriceratopsScore;

        // 3. Atualiza placar global de vitórias (estrelas)
        if (matchWinner == TREX) gTotalWinsTrex++;
        else if (matchWinner == TRICERATOPS) gTotalWinsTriceratops++;

        gCurrentMatch++; // Avança o contador de partidas

        // 4. Lógica de transição
        // Se alguém já ganhou 2 arenas, vai direto para o GameOver
        if (gTotalWinsTrex == 2 || gTotalWinsTriceratops == 2 || gCurrentMatch > 3) {
            Engine::Next<GameOver>();
        }
        else {
            // Caso contrário, segue o campeonato para a Arena 2
            Engine::Next<Arena2>();
        }
        return true;
    }

    return false;
}

// ------------------------------------------------------------------------------

void Arena1::Update()
{
    ProcessInputs();       // Lida com teclas pressionadas
    ManageMatchState();    // Lida com placares e cronômetros

    if (gScene == nullptr) return;

    scene->Update();
    scene->CollisionDetection();
}

// ------------------------------------------------------------------------------

void Arena1::Draw()
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

void Arena1::DrawMatchTime()
{
    float timeLeft = MATCH_TIME_LIMIT - matchTimer;

    Color textColor = timeLeft <= 10 ? Color{ 0.8, 0, 0, 1 } : Color{ 1, 1, 1, 1 }; // vermelho se faltar menos de 10 segundos, caso contr�rio branco

    string timeString = GetMatchTimeString();

    smallFonts->Draw(window->CenterX() + 10, 50.0f, timeString, textColor, Layer::FRONT, 3.0f);
}

void Arena1::DrawSscoreBoard()
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

string Arena1::GetMatchTimeString() const
{
    // 1. Calcula os segundos restantes (e impede de ficar negativo)
    int timeLeft = (int)(MATCH_TIME_LIMIT - matchTimer);
    if (timeLeft < 0) timeLeft = 0;

    // 2. Extrai os minutos e segundos da matem�tica b�sica
    int minutes = timeLeft / 60;
    int seconds = timeLeft % 60;

    // 3. Monta a string do texto (ex: "1:05" ou "0:09")
    std::string timeString = std::to_string(minutes) + ":";

    // Se os segundos forem menores que 10, coloca um '0' na frente para n�o ficar "0:9"
    if (seconds < 10) {
        timeString += "0";
    }
    timeString += to_string(seconds);

    return timeString;
}

void Arena1::DrawKickoffCountdown()
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

void Arena1::Finalize()
{
    gScene = nullptr; // CRUCIAL: A trava do Update depende disso
    gAudio = nullptr;

    delete scene;
    delete backg;
    delete audio;
    delete smallFonts72;
    delete smallFonts;
}