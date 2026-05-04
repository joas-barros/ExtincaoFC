/**********************************************************************************
// Player (Código Fonte)
// Descrição: Define a classe para o jogador com física de pulo e colisão ajustadas.
**********************************************************************************/
#include "Player.h"
#include "Arena1.h"  
#include "ExtincaoFC.h"
#include <algorithm> 

using namespace std;

Player::Player(Dinasaur t, float posX, float posY)
{
    this->specie = t;
    this->initialPosX = posX;
    this->initialPosY = posY;

    MoveTo(posX, posY, Layer::MIDDLE);

    type = PLAYER;
    score = 0;
    onGround = true;
    velX = 0.0f;
    velY = 0.0f;
    scaleX = 1.0f;
    canMove = false;

    switch (specie)
    {
    case TREX:
        sprite = new Sprite("Resources/trex.png");
        break;
    case TRICERATOPS:
        sprite = new Sprite("Resources/triceratops.png");
        break;
    }

    drawBBox(specie, false);
}

Player::~Player()
{
    delete sprite;
}

void Player::Reset()
{
    MoveTo(initialPosX, initialPosY);
    velX = 0.0f;
    velY = 0.0f;
    onGround = true;
    canMove = false;
}

void Player::OnCollision(Object* obj)
{
    if (obj->Type() == PLAYER)
    {
        Player* other = (Player*)obj;

        float l1 = this->Left();
        float r1 = this->Right();
        float t1 = this->Top();
        float b1 = this->Bottom();

        float l2 = other->Left();
        float r2 = other->Right();
        float t2 = other->Top();
        float b2 = other->Bottom();

        // --- DEFINIÇÃO DAS VARIÁVEIS (Correção do erro de identifier undefined) ---
        float overlapLeft = r1 - l2;
        float overlapRight = r2 - l1;
        float overlapTop = b1 - t2;
        float overlapBottom = b2 - t1;

        float overlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
        float overlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

        if (overlapX < overlapY)
        {
            if (overlapLeft < overlapRight)
                this->Translate(-overlapX / 2.0f, 0.0f);
            else
                this->Translate(overlapX / 2.0f, 0.0f);
        }
        else
        {
            if (overlapTop < overlapBottom)
            {
                // Só pousa se estiver descendo ou parado (permite o pulo livre)
                if (this->velY >= 0.0f)
                {
                    this->Translate(0.0f, -overlapY);
                    this->velY = 0.0f;
                    this->onGround = true;
                }
            }
            else
            {
                if (this->velY < 0.0f)
                    this->velY = 0.0f;
            }
        }
    }
}

void Player::Update()
{
    velX = 0.0f;

    // --- ENTRADA DE COMANDOS ---
    if (specie == TREX)
    {
        if (window->KeyDown('A') && canMove) velX = -PLAYER_MOVE_SPEED;
        if (window->KeyDown('D') && canMove) velX = PLAYER_MOVE_SPEED;

        if (window->KeyPress('W') && onGround) {
            velY = -PLAYER_JUMP_FORCE;
            onGround = false;
            if (gAudio) gAudio->Play(JUMP);
        }
        if (window->KeyDown('S') && !onGround)
            velY += PLAYER_FAST_FALL_FORCE * gameTime;
    }
    else if (specie == TRICERATOPS)
    {
        if (window->KeyDown(VK_LEFT) && canMove) velX = -PLAYER_MOVE_SPEED;
        if (window->KeyDown(VK_RIGHT) && canMove) velX = PLAYER_MOVE_SPEED;

        if (window->KeyPress(VK_UP) && onGround) {
            velY = -PLAYER_JUMP_FORCE;
            onGround = false;
            if (gAudio) gAudio->Play(JUMP);
        }
        if (window->KeyDown(VK_DOWN) && !onGround)
            velY += PLAYER_FAST_FALL_FORCE * gameTime;
    }

    // --- FÍSICA E MOVIMENTO ---
    if (!onGround)
        velY += PLAYER_GRAVITY * gameTime;

    Translate(velX * gameTime, velY * gameTime);

    // --- VERIFICAÇÃO DE CHÃO ---
    if (this->y >= window->Height() - 220.0f)
    {
        MoveTo(this->x, window->Height() - 220.0f);
        velY = 0.0f;
        onGround = true;
    }
    else
    {
        // Se estiver no ar, assumimos onGround false. 
        // A OnCollision confirmará se ele está pisando no outro player.
        onGround = false;
    }

    // --- LIMITES DA TELA ---
    if (this->Left() < 0.0f) Translate(-this->Left(), 0.0f);
    else if (this->Right() > window->Width()) Translate(window->Width() - this->Right(), 0.0f);
}

void Player::drawBBox(Dinasaur type, bool flipped)
{
    Mixed* mixedBBox = new Mixed();

    if (type == TREX)
    {
        float headR = 55.0f, headX = 15.0f, headY = -25.0f;
        float bodyL = -40.0f, bodyT = 10.0f, bodyR = 20.0f, bodyB = 65.0f;
        float tailL = -70.0f, tailT = 10.0f, tailR = -40.0f, tailB = 30.0f;

        if (flipped)
        {
            headX = -headX;
            float nBL = -bodyR, nBR = -bodyL;
            float nTL = -tailR, nTR = -tailL;
            bodyL = nBL; bodyR = nBR;
            tailL = nTL; tailR = nTR;
        }

        bLeft = (tailL < bodyL) ? tailL : bodyL;
        if (headX - headR < bLeft) bLeft = headX - headR;
        bRight = (tailR > bodyR) ? tailR : bodyR;
        if (headX + headR > bRight) bRight = headX + headR;
        bTop = headY - headR;
        bBottom = bodyB;

        Circle* head = new Circle(headR);
        head->MoveTo(headX, headY);
        mixedBBox->Insert(head);
        mixedBBox->Insert(new Rect(bodyL, bodyT, bodyR, bodyB));
        mixedBBox->Insert(new Rect(tailL, tailT, tailR, tailB));
    }
    else if (type == TRICERATOPS)
    {
        float headR = 55.0f, headX = 0.0f, headY = -10.0f;
        float bodyL = -20.0f, bodyT = 25.0f, bodyR = 50.0f, bodyB = 60.0f;
        float tailL = 50.0f, tailT = 35.0f, tailR = 65.0f, tailB = 55.0f;

        if (flipped)
        {
            float nBL = -bodyR, nBR = -bodyL;
            float nTL = -tailR, nTR = -tailL;
            bodyL = nBL; bodyR = nBR;
            tailL = nTL; tailR = nTR;
        }

        bLeft = (tailL < bodyL) ? tailL : bodyL;
        if (headX - headR < bLeft) bLeft = headX - headR;
        bRight = (tailR > bodyR) ? tailR : bodyR;
        if (headX + headR > bRight) bRight = headX + headR;
        bTop = headY - headR;
        bBottom = bodyB;

        Circle* head = new Circle(headR);
        head->MoveTo(headX, headY);
        mixedBBox->Insert(head);
        mixedBBox->Insert(new Rect(bodyL, bodyT, bodyR, bodyB));
        mixedBBox->Insert(new Rect(tailL, tailT, tailR, tailB));
    }

    BBox(mixedBBox);
}