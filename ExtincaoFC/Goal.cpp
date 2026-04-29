#include "Goal.h"

Goal::Goal(Player* p, float posX, float posY)
{
	player = p;

	MoveTo(posX, posY, Layer::MIDDLE);

	switch (player->Type())
	{
		case TREX:
			sprite = new Sprite("Resources/goal_trex.png");
		break;
		case TRICERATOPS:
			sprite = new Sprite("Resources/goal_triceratops.png");
		break;
	}
}

Goal::~Goal()
{
	delete sprite;
}

void Goal::OnCollision(Object* obj)
{
	// Implement collision logic here
}

void Goal::Update()
{
	// Implement update logic here
}
