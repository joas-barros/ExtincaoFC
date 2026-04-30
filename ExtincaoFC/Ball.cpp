#include "Ball.h"
#include "ExtincaoFC.h"
#include <cmath>
using namespace std;

Ball::Ball()
{
	
	moviment = new TileSet("Resources/BallMovement.png", 95, 95, 8, 64);
	anim = new Animation(moviment, 0.1f, true);

	type = BALL;
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

	velX = 0.0f;
	velY = 0.0f;
	state = STILL;
	active = false; 

	// cria bounding box
	BBox(new Circle(36.0f));

	MoveTo(window->CenterX(), window->CenterY() - 200.0f, Layer::LOWER);
}

Ball::~Ball()
{
	delete moviment;
	delete anim;
}

void Ball::Update()
{
	if (!active)
	{
		if (window->KeyPress(VK_RETURN)) { 
			active = true;
		}
		else {
			return; 
		}
	}

	// 2. APLICA GRAVIDADE E MOVIMENTO
	float gravity = 900.0f;
	velY += gravity * gameTime;
	Translate(velX * gameTime, velY * gameTime);

	// 3. RECUPERA OS LIMITES DO CÍRCULO PARA A COLISÃO COM A TELA
	Circle* c = (Circle*)BBox();
	float left = c->CenterX() - c->radius;
	float right = c->CenterX() + c->radius;
	float top = c->CenterY() - c->radius;
	float bottom = c->CenterY() + c->radius;

	// 4. QUIQUES NAS PAREDES (Restituição)
	float restitution = 0.8f; // Perde 20% da velocidade ao bater

	if (left < 0.0f) {
		Translate(-left, 0.0f); // Tira de dentro da parede
		velX = -velX * restitution;
	}
	else if (right > window->Width()) {
		Translate(window->Width() - right, 0.0f);
		velX = -velX * restitution;
	}

	if (top < 0.0f) {
		Translate(0.0f, -top);
		velY = -velY * restitution; // Bate no teto e desce
	}

	// 5. ANIMAÇÃO (MÁQUINA DE ESTADOS)
	float threshold = 30.0f;

	// Verifica se a bola está praticamente parada
	if (abs(velX) < threshold && abs(velY) < threshold) {
		state = STILL;
	}
	// Movimento puramente vertical
	else if (abs(velX) < threshold) {
		state = (velY < 0.0f) ? GOINGUP : GOINGDOWN;
	}
	// Movimento puramente horizontal
	else if (abs(velY) < threshold) {
		state = (velX < 0.0f) ? GOINGLEFT : GOINGRIGHT;
	}
	// Movimento nas diagonais
	else {
		if (velX > 0.0f && velY < 0.0f) state = GOUPRIGHT;
		else if (velX < 0.0f && velY < 0.0f) state = GOUPLEFT;
		else if (velX > 0.0f && velY > 0.0f) state = GODOWNRIGHT;
		else if (velX < 0.0f && velY > 0.0f) state = GODOWNLEFT;
	}

	// 5. Atualiza a animação
	anim->Select(state);

	if (state != STILL) {
		anim->NextFrame();
	}
}


void Ball::OnCollision(Object* obj)
{
	// Se a bola bateu em um jogador, atualizamos a referência
	if (obj->Type() == PLAYER)
	{
		lastPlayer = (Player*)obj;
	}
}