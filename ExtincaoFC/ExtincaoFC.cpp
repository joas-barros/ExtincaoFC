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
#include "Home.h"
#include "Arena1.h"
#include "ExtincaoFC.h"
#include "Resources.h"

// ------------------------------------------------------------------------------

Audio* gAudio = nullptr;
Scene* gScene = nullptr;

int gTotalWinsTrex = 0;
int gTotalWinsTriceratops = 0;
int gCurrentMatch = 1;

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // cria engine
    Engine* engine = new Engine();

    // configura motor
    engine->window->Mode(WINDOWED);
    engine->window->Size(1600, 860);
    engine->window->Color(80, 80, 50);
    engine->window->Title("ExtincaoFC");
    engine->window->Icon(IDI_ICON);
    //engine->window->Cursor(IDC_CURSOR);
    //engine->graphics->VSync(true);

    // inicia o jogo
    engine->Start(new Home());

    // destrói engine e jogo
    delete engine;
}

// ----------------------------------------------------------------------------

