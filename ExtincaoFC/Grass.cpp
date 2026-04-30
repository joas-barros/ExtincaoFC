#include "Grass.h"
#include "ExtincaoFC.h"
#include "Player.h"
#include <cmath>
using namespace std;

Grass::Grass(float posX, float posY)
{
    MoveTo(posX, posY, Layer::UPPER);
    type = GRASS;
    sprite = new Sprite("Resources/grass.png");

    // cria bounding box
    BBox(new Rect(-sprite->Width() / 2.0f,
        -sprite->Height() / 2.0f,
        sprite->Width() / 2.0f,
        sprite->Height() / 2.0f));
}

Grass::~Grass()
{
    delete sprite;
}

void Grass::Update()
{
    // lógica de atualização do objeto
}

void Grass::OnCollision(Object* obj)
{
    if (obj->Type() == PLAYER)
    {
		Player* player = (Player*)obj;

        // Verifica se o jogador está efetivamente caindo (movimento para baixo)
        if (player->velY > 0.0f)
        {
            player->velY = 0.0f;

			player->onGround = true;

			player->MoveTo(player->X(), window->Height() - 220.0f);
        }

	}

    if (obj->Type() == BALL)
    {
        Ball* ball = (Ball*)obj;

        // Verifica se a bola está caindo
        if (ball->velY > 0.0f)
        {
            // 1. Corrige a posição para ela não afundar no chão
            Circle* c = (Circle*)ball->BBox();

            ball->MoveTo(ball->X(), window->Height() - 220.0f - c->radius);

            // 2. Aplica o Quique (Restituição)
            float restitution = 0.7f; // A grama amortece um pouco mais que a parede (0.8)
            ball->velY = -ball->velY * restitution;

            // 3. Aplica o Atrito (Friction) para ela parar de rolar aos poucos
            float friction = 0.98f;
            ball->velX = ball->velX * friction;

            // Para a bola não ficar quicando microscopicamente para sempre
            if (std::abs(ball->velY) < 40.0f) {
                ball->velY = 0.0f; // Se a força for muito fraquinha, ela finalmente para de quicar
            }
        }
    }
}