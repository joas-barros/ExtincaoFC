/**********************************************************************************
// Player (Código Fonte)
// 
// Criação:     20 Abr 2012
// Atualização: 02 Set 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Define uma classe para o jogador
//
**********************************************************************************/

#include "Player.h"
#include "ExtincaoFC.h"                 // plataforma

// ---------------------------------------------------------------------------------

Player::Player(Dinasaur type, float posX, float posY)
{
    this->type = type;
    MoveTo(posX, posY, Layer::MIDDLE);

	score = 0;

    switch (type)
    {
    case TREX:
        sprite = new Sprite("Resources/trex.png");
        break;
    case TRICERATOPS:
        sprite = new Sprite("Resources/triceratops.png");
        break;
    }

}

// ---------------------------------------------------------------------------------

Player::~Player()
{
	delete sprite;
}

// ---------------------------------------------------------------------------------

void Player::OnCollision(Object * obj)
{
    
}

// ---------------------------------------------------------------------------------

void Player::Update()
{
    
}

// ---------------------------------------------------------------------------------