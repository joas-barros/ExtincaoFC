#include "Engine.h"
#include "Home.h"
#include "Arena1.h"
#include "ExtincaoFC.h"

// ------------------------------------------------------------------------------

void Home::Init()
{
    // Carrega as duas imagens na memória de uma vez
    titleBg = new Sprite("Resources/TitleScreen.png");
    helpBg = new Sprite("Resources/HelpScreen.png");

    homeSound = new Audio();
    homeSound->Add(CHAMPIONS_SONG, "Resources/champions_song.wav");
    homeSound->Play(CHAMPIONS_SONG);

    // O jogo sempre começa mostrando a tela inicial, não a de ajuda
    showingHelp = false;
}

// ------------------------------------------------------------------------------

void Home::Finalize()
{
    delete homeSound;
    delete titleBg;
    delete helpBg; // Importante limpar o novo sprite também!
}

// ------------------------------------------------------------------------------

void Home::Update()
{
    if (showingHelp)
    {

        // Inicia o game direto da ajuda
        if (window->KeyPress(VK_RETURN))
            Engine::Next<Arena1>();

        // Volta para a Home (Menu)
        if (window->KeyPress(VK_BACK) || window->KeyPress(VK_ESCAPE))
        {
            showingHelp = false; // Apenas muda a flag, a música continua rolando!
        }
    }
    else
    {

        // Fecha o jogo
        if (window->KeyPress(VK_ESCAPE))
            window->Close();

        // Inicia o game
        if (window->KeyPress(VK_RETURN))
            Engine::Next<Arena1>();

        // Abre a tela de ajuda
        if (window->KeyPress('H'))
        {
            showingHelp = true; 
        }
    }
}

// ------------------------------------------------------------------------------

void Home::Draw()
{
    // Desenha a imagem correta dependendo do estado atual
    if (showingHelp)
    {
        helpBg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    }
    else
    {
        titleBg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
    }
}

// ------------------------------------------------------------------------------