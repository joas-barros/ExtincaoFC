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

			
			Circle* netCurve = new Circle(20.0f);
			netCurve->MoveTo(95.0f, -120.0f);
			mixedBBox->Insert(netCurve);

			mixedBBox->Insert(new Rect(40.0f, -130.0f, 80.0f, -110.0f)); 

			mixedBBox->Insert(new Rect(0.0f, -120.0f, 40.0f, -100.0f)); 
			mixedBBox->Insert(new Rect(-30.0f, -100.0f, 0.0f, -90.0f)); 
			mixedBBox->Insert(new Rect(-50.0f, -90.0f, -30.0f, -80.0f)); 
			mixedBBox->Insert(new Rect(-60.0f, -80.0f, -50.0f, -65.0f)); 
			mixedBBox->Insert(new Rect(-70.0f, -65.0f, -55.0f, -50.0f)); 

			mixedBBox->Insert(new Rect(-90.0f, -50.0f, -70.0f, -30.0f)); 
			mixedBBox->Insert(new Rect(-100.0f, -30.0f, -75.0f, 10.0f)); 
			mixedBBox->Insert(new Rect(-110.0f, 10.0f, -80.0f, 40.0f)); 
			mixedBBox->Insert(new Rect(-130.0f, 40.0f, -90.0f, 80.0f)); 
			mixedBBox->Insert(new Rect(-130.0f, 80.0f, -100.0f, 120.0f)); 
			mixedBBox->Insert(new Rect(-130.0f, 120.0f, -100.0f, 150.0f)); 

			break;
		}
			
		case TRICERATOPS:
		{
			sprite = new Sprite("Resources/goal_triceratops.png");

			Circle* netCurve = new Circle(20.0f);
			netCurve->MoveTo(-95.0f, -120.0f);
			mixedBBox->Insert(netCurve);

			mixedBBox->Insert(new Rect(-40.0f, -130.0f, -80.0f, -110.0f));

			mixedBBox->Insert(new Rect(0.0f, -120.0f, -40.0f, -100.0f));
			mixedBBox->Insert(new Rect(30.0f, -100.0f, 0.0f, -90.0f));
			mixedBBox->Insert(new Rect(50.0f, -90.0f, 30.0f, -80.0f));
			mixedBBox->Insert(new Rect(60.0f, -80.0f, 50.0f, -65.0f));
			mixedBBox->Insert(new Rect(70.0f, -65.0f, 55.0f, -50.0f));

			mixedBBox->Insert(new Rect(90.0f, -50.0f, 70.0f, -30.0f));
			mixedBBox->Insert(new Rect(100.0f, -30.0f, 75.0f, 10.0f));
			mixedBBox->Insert(new Rect(110.0f, 10.0f, 80.0f, 40.0f));
			mixedBBox->Insert(new Rect(130.0f, 40.0f, 90.0f, 80.0f));
			mixedBBox->Insert(new Rect(130.0f, 80.0f, 100.0f, 120.0f));
			mixedBBox->Insert(new Rect(130.0f, 120.0f, 100.0f, 150.0f));
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
	// Implement collision logic here
}

void Goal::Update()
{
	// Implement update logic here
}
