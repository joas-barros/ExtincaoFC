#ifndef _EXTINCAOFC_GOAL_H_
#define _EXTINCAOFC_GOAL_H_

#include "Types.h"                      // tipos específicos da engine
#include "Object.h"                     // interface de Object
#include "Sprite.h"
#include "Player.h"
#include <string>
using namespace std;

class Goal : public Object
{
private:
	Player* scorer;                  // jogador associado à meta
	Rect* sensorBBox;

public:
	Sprite* sprite;                  // sprite do gol

	void LoadBBoxFromFile(string filename, Mixed* mixedBBox);
	void ReloadBBox(string filename);

	Goal(Player* p, float posX, float posY);
	~Goal();

	// GETTERS
	Player* GetScorer() const { return scorer; }
	Rect* SensorBBox() const { return sensorBBox; }

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
