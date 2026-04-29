#ifndef _EXTINCAOFC_GOAL_H_
#define _EXTINCAOFC_GOAL_H_

#include "Types.h"                      // tipos específicos da engine
#include "Object.h"                     // interface de Object
#include "Sprite.h"
#include "Player.h"

class Goal : public Object
{
private:
	Sprite* sprite;                  // sprite da plataforma
	Player* player;                  // jogador associado à meta

public:
	Goal(Player* p, float posX, float posY);
	~Goal();

	void OnCollision(Object* obj);     // resolução da colisão
	void Update();                      // atualização do objeto
	void Draw();                        // desenho do objeto
};

// Função Membro Inline

inline void Goal::Draw()
{
	sprite->Draw(x, y, z);
}

#endif
