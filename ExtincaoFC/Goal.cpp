#include "Goal.h"
#include "ExtincaoFC.h"

Goal::Goal(Player* p, float posX, float posY)
{
	player = p;

	MoveTo(posX, posY, Layer::MIDDLE);

	switch (player->Specie())
	{
		case TREX:
			sprite = new Sprite("Resources/goal_trex.png");
		break;
		case TRICERATOPS:
			sprite = new Sprite("Resources/goal_triceratops.png");
		break;
	}

	type = GOAL;
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
