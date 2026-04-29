#include "Ball.h"

Ball::Ball()
{
	
	moviment = new TileSet("Resources/BallMovement.png", 95, 95, 8, 64);
	anim = new Animation(moviment, 0.1f, true);

	uint SeqDown[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	uint SeqLeft[8] = { 8, 9, 10, 11, 12, 13, 14, 15 };
	uint SeqUp[8] = { 16, 17, 18, 19, 20, 21, 22, 23 };
	uint SeqRight[8] = { 24, 25, 26, 27, 28, 29, 30, 31 };
	uint SeqDownLeft[8] = { 32, 33, 34, 35, 36, 37, 38, 39 };
	uint SeqDownRight[8] = { 40, 41, 42, 43, 44, 45, 46, 47 };
	uint SeqUpLeft[8] = { 48, 49, 50, 51, 52, 53, 54, 55 };
	uint SeqUpRight[8] = { 56, 57, 58, 59, 60, 61, 62, 63 };

	uint SeqStill[1] = { 0 };

	anim->Add(STILL, SeqStill, 1);
	anim->Add(GOINGUP, SeqUp, 8);
	anim->Add(GOINGDOWN, SeqDown, 8);
	anim->Add(GOINGLEFT, SeqLeft, 8);
	anim->Add(GOINGRIGHT, SeqRight, 8);
	anim->Add(GOUPRIGHT, SeqUpRight, 8);
	anim->Add(GOUPLEFT, SeqUpLeft, 8);
	anim->Add(GODOWNRIGHT, SeqDownRight, 8);
	anim->Add(GODOWNLEFT, SeqDownLeft, 8);

	speed = 0.0f;
	state = STILL;
	// cria bounding box
	BBox(new Circle(40.0f));

	MoveTo(window->CenterX(), window->CenterY(), Layer::LOWER);
}

Ball::~Ball()
{
	delete moviment;
	delete anim;
}

void Ball::Update()
{
	// lógica de atualização do objeto
	anim->Select(state);
}