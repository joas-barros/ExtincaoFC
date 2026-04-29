#ifndef _EXTINCAOFC_BALL_H_
#define _EXTINCAOFC_BALL_H_

#include "Types.h"                      // tipos específicos da engine
#include "Object.h"                     // interface de Object
#include "Animation.h"                  // animação de sprites
#include "Player.h"

// ------------------------------------------------------------------------------

enum BallState { STILL, GOINGUP, GOINGDOWN, GOINGLEFT, GOINGRIGHT, GOUPRIGHT, GOUPLEFT, GODOWNRIGHT, GODOWNLEFT };

// ---------------------------------------------------------------------------------

class Ball : public Object
{
private:
    TileSet* moviment;                // folha de sprites do personagem
    Animation* anim;                   // animação do personagem
    float       speed;                  // velocidade do personagem
	Player* player = nullptr;                  // jogador associado à bola


public:
    uint state;                         // estado atual do personagem

    Ball();                             // construtor
    ~Ball();                            // destrutor

    void Update();                      // atualização do objeto
    void Draw();                        // desenho do objeto
};

// ---------------------------------------------------------------------------------
// Função Membro Inline

inline void Ball::Draw()
{
    anim->Draw(x, y, z);
}

// ---------------------------------------------------------------------------------

#endif
