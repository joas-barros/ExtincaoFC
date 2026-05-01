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

	Reset();

	// cria bounding box
	BBox(new Circle(36.0f));
}

Ball::~Ball()
{
	delete moviment;
	delete anim;
}

void Ball::Reset()
{
	MoveTo(window->CenterX(), window->CenterY() - 300.0f, Layer::LOWER);
	velX = 0.0f;
	velY = 0.0f;
	state = STILL;
	active = false; 
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
	velY += BALL_GRAVITY * gameTime;
	Translate(velX * gameTime, velY * gameTime);

	// 3. RECUPERA OS LIMITES DO CÍRCULO PARA A COLISÃO COM A TELA
	Circle* c = (Circle*)BBox();
	float left = c->CenterX() - c->radius;
	float right = c->CenterX() + c->radius;
	float top = c->CenterY() - c->radius;
	float bottom = c->CenterY() + c->radius;

	// 4. QUIQUES NAS PAREDES (Restituição)

	if (left < 0.0f) {
		Translate(-left, 0.0f); // Tira de dentro da parede
		velX = -velX * BALL_RESTITUTION;
	}
	else if (right > window->Width()) {
		Translate(window->Width() - right, 0.0f);
		velX = -velX * BALL_RESTITUTION;
	}

	if (top < 0.0f) {
		Translate(0.0f, -top);
		velY = -velY * BALL_RESTITUTION; // Bate no teto e desce
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
		Player* p = (Player*)obj;

		// Salva o último jogador a tocar na bola
		lastPlayer = p;

		// Encontra o "centro" do Player usando as extremidades da Bbox do Player
		float pCenterX = p->Left() + (p->Right() - p->Left()) / 2.0f;
		float pCenterY = p->Top() + (p->Bottom() - p->Top()) / 2.0f;

		// Pega o centro da bola
		Circle* c = (Circle*)this->BBox();
		float bCenterX = c->CenterX();
		float bCenterY = c->CenterY();

		// Calcula a distância entre os centros nos eixos X e Y
		float dx = bCenterX - pCenterX;
		float dy = bCenterY - pCenterY;

		// Normaliza o vetor (transforma a "seta" para ter tamanho 1)
		float distance = sqrt(dx * dx + dy * dy);
		if (distance == 0.0f) distance = 1.0f; // Previne divisão por zero

		float dirX = dx / distance;
		float dirY = dy / distance;

		this->velX = dirX * BALL_BASE_BOUNCE_FORCE;
		this->velY = dirY * BALL_BASE_BOUNCE_FORCE;

		// Transfere parte da velocidade atual do jogador para a bola
		this->velX += p->velX * SPEED_TRANSFER_RATE_PLAYER_TO_BALL;
		this->velY += p->velY * SPEED_TRANSFER_RATE_PLAYER_TO_BALL;

		// Trava a velocidade horizontal dentro do limite
		if (this->velX > BALL_MAX_SPEED)  this->velX = BALL_MAX_SPEED;
		if (this->velX < -BALL_MAX_SPEED) this->velX = -BALL_MAX_SPEED;

		// Trava a velocidade vertical dentro do limite
		if (this->velY > BALL_MAX_SPEED)  this->velY = BALL_MAX_SPEED;
		if (this->velY < -BALL_MAX_SPEED) this->velY = -BALL_MAX_SPEED;

		// Dá um micro-empurrãozinho na bola na direção que ela vai voar 
		// para garantir que ela não bata 2x no mesmo jogador no próximo frame
		this->Translate(dirX * 5.0f, dirY * 5.0f);
	}

	if (obj->Type() == GOAL)
	{
		Circle* c = (Circle*)this->BBox();

		

		// 2. SEPARAÇÃO DO IMPACTO
		// O teto do seu gol agora fica entre -150.0f e -130.0f. 
		// Usamos uma margem de -110.0f para garantir que o centro da bola bateu por cima.
		if (this->velY > 0.0f && c->CenterY() < obj->Y() - 110.0f)
		{
			// ==========================================
			// COLISÃO SUPERIOR 
			// ==========================================

			// 1. BACKTRACKING: Tira a bola de dentro da geometria
			this->Translate(0.0f, -this->velY * gameTime);

			// Faz a bola dar um quique menor no teto
			this->velY = -this->velY * NET_DAMPING;

			if (this->X() < window->CenterX()) {
				// Se está na esquerda, acelera suavemente para a direita
				if (this->velX < BALL_GOAL_MAX_SLIDE_SPEED) this->velX += BALL_GOAL_ACCELERATION * gameTime;
			}
			else {
				// Se está na direita, acelera suavemente para a esquerda
				if (this->velX > -BALL_GOAL_MAX_SLIDE_SPEED) this->velX -= BALL_GOAL_ACCELERATION * gameTime;
			}
		}
		else
		{
			// ==========================================
			// COLISÃO INTERNA (A REDE INCLINADA)
			// ==========================================

			// 1. Backtrack TOTAL (X e Y) porque ela trombou na rede e não deve atravessar
			this->Translate(-this->velX * gameTime, -this->velY * gameTime);

			this->velX = -this->velX * NET_DAMPING;
			this->velY = -this->velY * NET_DAMPING;
		}
	}
}