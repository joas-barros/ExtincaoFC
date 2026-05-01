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
#include "Player.h"
#include "Grass.h"
#include "Goal.h"
#include "Ball.h"
#include "GoalSensor.h"

#define SCORE_TO_WIN 5

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

	Goal * goal1 = nullptr;
	Goal * goal2 = nullptr;

	GoalSensor * sensor1 = nullptr;
	GoalSensor * sensor2 = nullptr;

	Ball * ball = nullptr;

	uint lastTotalScoreBoard;          // pontuação total do placar
    bool waitingReset;
	float resetTimer;

    bool viewBBox = false;             // visualiza bounding box

    void ResetMatch();

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