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
    // cria cena do jogo
    scene = new Scene();

	// cria sistema de áudio
	audio = new Audio();

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

	// cria bola
	ball = new Ball();
	scene->Add(ball, MOVING);
}

// ------------------------------------------------------------------------------

void ExtincaoFC::Update()
{
    // sai com o pressionar do ESC
    if (window->KeyDown(VK_ESCAPE))
        window->Close();

    if (window->KeyPress('B'))
        viewBBox = !viewBBox;

    // ----------------------------------
    // atualiza a posição dos objetos
    // ----------------------------------

    scene->Update();
    scene->CollisionDetection();
} 

// ------------------------------------------------------------------------------

void ExtincaoFC::Draw()
{
	backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    scene->Draw();

    if (viewBBox)
    {
        scene->DrawBBox();
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
	delete ball;
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

