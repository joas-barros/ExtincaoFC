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

#ifndef _ARENA2_H_
#define _ARENA2_H_

#include "Arena1.h"

class Arena2 : public Arena1
{
protected:   
    void ResetMatch();
    int BallDirection();
    void ProcessInputs();
    void Update();
    bool TreatMatchEnding();

public:
    void Init();  
    void DrawSscoreBoard();
    void Draw();
};

#endif