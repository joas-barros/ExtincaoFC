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

#define MATCH_TIME_LIMIT 90.0f
#define SCORE_TO_WIN 3
#define TIME_TO_RESET 3.0f
#define KICKOFF_TIME 2.0f

// ------------------------------------------------------------------------------

enum Sounds { GOAL_SCREAM, INITIAL_WHISTLE, FINAL_WHISTLE, KICK, JUMP };

enum ObjTypes
{
    PLAYER,
    BALL,
    GRASS,
    GOAL,
    SENSOR
};

// ------------------------------------------------------------------------------

class Scene; // Forward declaration
class Audio;

// Isso permite que QUALQUER arquivo .cpp use 'gAudio' ou 'gScene'
extern Audio* gAudio;
extern Scene* gScene;
extern int gTotalWinsTrex;
extern int gTotalWinsTriceratops;
extern int gCurrentMatch; // Para saber se estamos na partida 1, 2 ou 3
extern int gTotalGolsTrex;         // Acumulado de gols do T-Rex
extern int gTotalGolsTriceratops;  // Acumulado de gols do Triceratops

#endif