#include "Goal.h"
#include "ExtincaoFC.h"
#include <fstream>

Goal::Goal(Player* p, float posX, float posY)
{
	player = p;

	MoveTo(posX, posY, Layer::UPPER);

	Mixed* mixedBBox = new Mixed();

	switch (player->Specie())
	{
		case TREX:
		{
			sprite = new Sprite("Resources/goal_trex.png");

			LoadBBoxFromFile("Resources/goal_trex_bbox.txt", mixedBBox);

			break;
		}
			
		case TRICERATOPS:
		{
			sprite = new Sprite("Resources/goal_triceratops.png");

			LoadBBoxFromFile("Resources/goal_triceratops_bbox.txt", mixedBBox);
			break;
		}
	}

	BBox(mixedBBox);
	type = GOAL;
}

void Goal::LoadBBoxFromFile(string filename, Mixed* mixedBBox)
{
	ifstream file(filename);

	if (!file.is_open()) {
		return;
	}

	string shape;
	while (file >> shape)
	{
		if (shape == "CIRCLE")
		{
			float x, y, r;
			file >> x >> y >> r;

			Circle* c = new Circle(r);
			c->MoveTo(x, y);
			mixedBBox->Insert(c);
		}
		else if (shape == "RECT")
		{
			float l, t, r, b;
			file >> l >> t >> r >> b; 

			mixedBBox->Insert(new Rect(l, t, r, b));
		}
	}

	file.close(); 
}

Goal::~Goal()
{
	delete sprite;
}

void Goal::OnCollision(Object* obj)
{
	if (obj->Type() == PLAYER)
	{
		Player* player = (Player*)obj;

		// 1. Desfaz o movimento horizontal
		// Se o jogador tentou entrar na malha da rede, ele é empurrado de volta 
		// a exata quantidade de pixels que ele andou neste frame.
		if (player->velX != 0.0f)
		{
			player->Translate(-player->velX * gameTime, 0.0f);
		}


		// 2. Tratamento do Teto do Gol (Batendo a cabeça)
		// Se ele pulou contra o teto da rede, interrompemos a subida
		if (player->velY < 0.0f)
		{
			player->Translate(0.0f, -player->velY * gameTime); 
			player->velY = 0.0f;                             
		}
	}
}

void Goal::Update()
{
	// Implement update logic here
}
