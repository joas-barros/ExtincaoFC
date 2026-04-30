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
#include "ExtincaoFC.h"                 

// ---------------------------------------------------------------------------------

Player::Player(Dinasaur t, float posX, float posY)
{
    this->specie = t;
    MoveTo(posX, posY, Layer::MIDDLE);

	type = PLAYER;

	score = 0;

	onGround = true;

    switch (specie)
    {
    case TREX:
        sprite = new Sprite("Resources/trex.png");
        break;
    case TRICERATOPS:
        sprite = new Sprite("Resources/triceratops.png");
        break;
    }

	velX = 0.0f;
	velY = 0.0f;

	drawBBox(specie);

}

// ---------------------------------------------------------------------------------

Player::~Player()
{
	delete sprite;
}

// ---------------------------------------------------------------------------------

void Player::OnCollision(Object* obj)
{
    if (obj->Type() == PLAYER)
    {
        Player* other = (Player*)obj;

        // 1. Usamos os nossos getters direto do player!
        float l1 = this->Left();
        float r1 = this->Right();
        float t1 = this->Top();
        float b1 = this->Bottom();

        float l2 = other->Left();
        float r2 = other->Right();
        float t2 = other->Top();
        float b2 = other->Bottom();

        // 2. Calculamos a sobreposição
        float overlapLeft = r1 - l2; // Este jogador invadindo o outro pela esquerda
        float overlapRight = r2 - l1; // Este jogador invadindo o outro pela esquerda
        float overlapTop = b1 - t2; // Este jogador caindo em cima do outro
        float overlapBottom = b2 - t1; // Este jogador subindo e batendo no outro

        float overlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
        float overlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

        // 3. Resolvemos a colisão
        if (overlapX < overlapY)
        {
            // COLISÃO HORIZONTAL (Bateu de frente ou de costas)
            if (overlapLeft < overlapRight) {
                this->Translate(-overlapX / 2.0f, 0.0f);
            }
            else {
                this->Translate(overlapX / 2.0f, 0.0f);
            }
        }
        else
        {
            // COLISÃO VERTICAL (Caiu em cima ou bateu a cabeça por baixo)
            if (overlapTop < overlapBottom) {
                this->Translate(0.0f, -overlapY);
                this->velY = 0.0f;
                /*this->onGround = true;*/
            }
            else {
                // Este jogador bateu a cabeça no que estava em cima
                if (this->velY < 0.0f) {
                    this->velY = 0.0f;
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------------

void Player::Update()
{

    velX = 0.0f; 

    if (specie == TREX)
    {
        if (window->KeyDown('A')) velX = -PLAYER_MOVE_SPEED;
        if (window->KeyDown('D')) velX = PLAYER_MOVE_SPEED;

        
        if (window->KeyPress('W') && onGround) {
            velY = -PLAYER_JUMP_FORCE; 
            onGround = false;  
        }

        
        if (window->KeyDown('S') && !onGround) {
            velY += PLAYER_FAST_FALL_FORCE * gameTime;
        }
    }
    else if (specie == TRICERATOPS)
    {
        if (window->KeyDown(VK_LEFT)) velX = -PLAYER_MOVE_SPEED;
        if (window->KeyDown(VK_RIGHT)) velX = PLAYER_MOVE_SPEED;

        if (window->KeyPress(VK_UP) && onGround) {
            velY = -PLAYER_JUMP_FORCE;
            onGround = false;
        }

        if (window->KeyDown(VK_DOWN) && !onGround) {
            velY += PLAYER_FAST_FALL_FORCE * gameTime;
        }
    }

    
    if (!onGround) {
        velY += PLAYER_GRAVITY * gameTime; 
    }

    Translate(velX * gameTime, velY * gameTime);

    // mantém personagem dentro da tela
    if (this->Left() < 0.0f) {
        Translate(-this->Left(), 0.0f);
    }
    else if (this->Right() > window->Width()) {
        Translate(window->Width() - this->Right(), 0.0f);
    }

}

void Player::drawBBox(Dinasaur type)
{
    // Cria o BBox misto vazio
    Mixed* mixedBBox = new Mixed();

    if (type == TREX)
    {
        
        float headR = 55.0f, headX = 15.0f, headY = -25.0f;
        float bodyL = -40.0f, bodyT = 10.0f, bodyR = 20.0f, bodyB = 65.0f;
        float tailL = -70.0f, tailT = 10.0f, tailR = -40.0f, tailB = 30.0f;

        bLeft = tailL;               
        bRight = headX + headR;     
        bTop = headY - headR;        
        bBottom = bodyB;             

        Circle* head = new Circle(headR);
        head->MoveTo(headX, headY);

        Rect* body = new Rect(bodyL, bodyT, bodyR, bodyB);
        Rect* tail = new Rect(tailL, tailT, tailR, tailB);

        mixedBBox->Insert(head);
        mixedBBox->Insert(body);
        mixedBBox->Insert(tail);
    }
    else if (type == TRICERATOPS)
    {
        
        float headR = 55.0f, headX = 0.0f, headY = -10.0f;
        float bodyL = -20.0f, bodyT = 25.0f, bodyR = 50.0f, bodyB = 60.0f;
        float tailL = 50.0f, tailT = 35.0f, tailR = 65.0f, tailB = 55.0f;

        
        bLeft = headX - headR;       
        bRight = tailR;              
        bTop = headY - headR;        
        bBottom = bodyB;             

        
        Circle* head = new Circle(headR);
        head->MoveTo(headX, headY);

        Rect* body = new Rect(bodyL, bodyT, bodyR, bodyB);
        Rect* tail = new Rect(tailL, tailT, tailR, tailB);

        mixedBBox->Insert(head);
        mixedBBox->Insert(body);
        mixedBBox->Insert(tail);
    }

    // Aplica o bounding box construído ao objeto
    BBox(mixedBBox);
}

// ---------------------------------------------------------------------------------