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

	drawBBox(type);

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

void Player::drawBBox(Dinasaur type)
{
    // Cria o BBox misto vazio
    Mixed* mixedBBox = new Mixed();

    switch (type)
    {
    case TREX:
    {

        Circle* head = new Circle(55.0f); 
        head->MoveTo(15.0f, -25.0f);      

        Rect * body = new Rect(-40.0f, 10.0f, 20.0f, 65.0f); 

		Rect* tail = new Rect(-70.0f, 10.0f, -40.0f, 30.0f); 

        mixedBBox->Insert(head);
        mixedBBox->Insert(body);
        mixedBBox->Insert(tail);
        break;
    }
    case TRICERATOPS:
    {

        Circle* head = new Circle(55.0f);
        head->MoveTo(0.0f, -10.0f);     

        Rect* body = new Rect(-20.0f, 25.0f, 50.0f, 60.0f);

		Rect* tail = new Rect(50.0f, 35.0f, 65.0f, 55.0f); 

        mixedBBox->Insert(head);
        mixedBBox->Insert(body);
		mixedBBox->Insert(tail);
        break;
    }
    }

    // Aplica o bounding box construído ao objeto
    BBox(mixedBBox);
}

// ---------------------------------------------------------------------------------