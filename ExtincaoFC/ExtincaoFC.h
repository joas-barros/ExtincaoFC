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

#ifndef _EXTINCAOFC_H_
#define _EXTINCAOFC_H_

// ---------------------------------------------------------------------------------
// Inclusões

#include "Game.h"
#include "Scene.h"
#include "Audio.h"
#include "Font.h"
#include "Player.h"
#include "Grass.h"
#include "Goal.h"
#include "Ball.h"
#include "GoalSensor.h"
#include <string>
using namespace std;

#define SCORE_TO_WIN 5
#define TIME_TO_RESET 1.0f
#define MATCH_TIME_LIMIT 60.0f

// ------------------------------------------------------------------------------

enum { MUSIC, TRANSITION };

enum ObjTypes
{
    PLAYER,                             
    BALL,                               
    GRASS,
	GOAL,
	SENSOR
};

// ------------------------------------------------------------------------------

class ExtincaoFC : public Game
{
private:
    Player * player1 = nullptr; 
	Player * player2 = nullptr;
    Sprite * backg = nullptr;
    Font* smallFonts = nullptr;
    Font* smallFonts72 = nullptr;

	Goal * goal1 = nullptr;
	Goal * goal2 = nullptr;

	GoalSensor * sensor1 = nullptr;
	GoalSensor * sensor2 = nullptr;

	Ball * ball = nullptr;

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

    void ResetMatch();
    void ProcessInputs();
    void ManageMatchState();
    int BallDirection();

    // Metodos para tratar texto
	string GetMatchTimeString() const;
	void DrawMatchTime();
	void DrawSscoreBoard();

    bool TreatMatchEnding();

public:
    static Scene * scene;           // gerenciador de cena
	static Audio * audio;           // gerenciador de áudio

    void Init();                    // inicialização
    void Update();                  // atualização
    void Draw();                    // desenho
    void Finalize();                // finalização
};

// ---------------------------------------------------------------------------------

#endif