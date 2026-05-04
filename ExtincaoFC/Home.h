/**********************************************************************************
// Home (Arquivo de Cabeçalho)
//
// Criação:     18 Jan 2013
// Atualização: 04 Mar 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Tela de abertura do jogo
//
**********************************************************************************/

#ifndef _HOME_H_
#define _HOME_H_

// ------------------------------------------------------------------------------
// Inclusões

#include "Game.h"
#include "Sprite.h"
#include "Audio.h"

// ------------------------------------------------------------------------------

class Home : public Game
{
private:
    Sprite* titleBg;
    Sprite* helpBg;
    Audio* homeSound;

    bool showingHelp; // Controla qual tela está sendo exibida

public:
    void Init();                    // inicialização
    void Update();                  // atualização
    void Draw();                    // desenho
    void Finalize();                // finalização
};

// -----------------------------------------------------------------------------
#endif
