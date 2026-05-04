#include "Help.h"
#include "Home.h"
#include "Arena1.h"
#include "Engine.h"

void Help::Init()
{
    // Certifique-se de criar essa imagem "HelpScreen.png" na pasta Resources
    backg = new Sprite("Resources/HelpScreen.png");
}

void Help::Update()
{
    // Inicia o game direto da ajuda
    if (window->KeyPress(VK_RETURN))
        Engine::Next<Arena1>();

    // Volta para a Home (Menu)
    if (window->KeyPress(VK_BACK) || window->KeyPress(VK_ESCAPE))
        Engine::Next<Home>();
}

void Help::Draw()
{
    backg->Draw(window->CenterX(), window->CenterY(), Layer::BACK);
}

void Help::Finalize()
{
    delete backg;
}