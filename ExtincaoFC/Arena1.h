/**********************************************************************************
// Platformer (Arquivo de Cabeçalho)
//
// Criação:     05 Out 2011
// Atualização: 02 Set 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Exemplo de jogo estilo platforma
//
**********************************************************************************/

#ifndef _ARENA1_H_
#define _ARENA1_H_

// ---------------------------------------------------------------------------------
// Inclusões

#include "Game.h"
#include "Scene.h"
#include "Audio.h"
#include "Player.h"
#include "Grass.h"
#include "Goal.h"
#include "Ball.h"
#include "Font.h"
#include "GoalSensor.h"
#include <string>

// ------------------------------------------------------------------------------

class Arena1 : public Game
{
protected:
    Player* player1 = nullptr;
    Player* player2 = nullptr;
    Sprite* backg = nullptr;
    Font* smallFonts = nullptr;
    Font* smallFonts72 = nullptr;

    Goal* goal1 = nullptr;
    Goal* goal2 = nullptr;

    GoalSensor* sensor1 = nullptr;
    GoalSensor* sensor2 = nullptr;

    Ball* ball = nullptr;

    uint lastTotalScoreBoard;          // pontuação total do placar
    uint lastTrexScore;               // pontuação do T-Rex no último placar
    uint lastTriceratopsScore;         // pontuação do Triceratops no último placar

    bool waitingReset;
    float resetTimer;
    float matchTimer;

    // atributos para o tempo de kickoff
    bool isKickoff;
    float kickoffTimer;

    uint winner;                      // espécie vencedora do jogo
    uint victimSpecie;

    bool viewBBox = false;             // visualiza bounding box

    virtual void ResetMatch();
    virtual void ProcessInputs();
    void ManageMatchState();
    virtual int BallDirection();

    // Metodos para tratar os estados do jogo
    void ProcessKickoff();
    bool ProcessMatchTimer();
    void ProcessGoalCelebration();

    // Metodos para tratar texto
    string GetMatchTimeString() const;
    void DrawMatchTime();
    virtual void DrawSscoreBoard();
    void DrawKickoffCountdown();

    virtual bool TreatMatchEnding();

public:
    static Scene* scene;           // gerenciador de cena
    static Audio* audio;           // gerenciador de áudio

    void Init();                    // inicialização
    void Update();                  // atualização
    void Draw();                    // desenho
    void Finalize();                // finalização
};

// ---------------------------------------------------------------------------------

#endif