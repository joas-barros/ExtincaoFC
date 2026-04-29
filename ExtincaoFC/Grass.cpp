#include "Grass.h"
#include "ExtincaoFC.h"
#include "Player.h"

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

			float overlap = 4;

			player->MoveTo(player->X(), window->Height() - 220.0f);
        }

	}
}