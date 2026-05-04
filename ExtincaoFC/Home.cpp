/**********************************************************************************
// Home (Código Fonte)
//
// Criação:     18 Jan 2013
// Atualização: 04 Mar 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Tela de abertura do jogo
//
**********************************************************************************/

#include "Engine.h"
#include "Home.h"
#include "ExtincaoFC.h"
#include "Arena1.h"
#include "Help.h"

// ------------------------------------------------------------------------------

void Home::Init()
{
    backg = new Sprite("Resources/TitleScreen.png");
}

// ------------------------------------------------------------------------------

void Home::Finalize()
{
    delete backg;
}

// ------------------------------------------------------------------------------

void Home::Update()
{
    if (window->KeyPress(VK_ESCAPE))
        window->Close();

    if (window->KeyPress(VK_RETURN))
        Engine::Next<Arena1>();

    // NOVIDADE: Abre a tela de ajuda ao pressionar 'H'
    if (window->KeyPress('H'))
        Engine::Next<Help>();
}

// ------------------------------------------------------------------------------

void Home::Draw()
{
    backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
}

// ------------------------------------------------------------------------------