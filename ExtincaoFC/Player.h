/**********************************************************************************
// Player (Arquivo de Cabeçalho)
// 
// Criação:     20 Abr 2012
// Atualização: 02 Set 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Define uma classe para o jogador 
//
**********************************************************************************/

#ifndef _PLATFORMER_PLAYER_H_
#define _PLATFORMER_PLAYER_H_

// ---------------------------------------------------------------------------------
// Inclusões

#include "Types.h"                      // tipos específicos da engine
#include "Object.h"                     // interface de Object
#include "Animation.h"                  // animação de sprites


// ------------------------------------------------------------------------------

enum Dinasaur {TREX, TRICERATOPS};        

// ---------------------------------------------------------------------------------

class Player : public Object
{
private:
    Sprite* sprite;
	Dinasaur specie;
    uint score;

	// Guarda os limites da bounding box para cada espécie, para facilitar a resolução de colisão
    float bLeft, bRight, bTop, bBottom;

	void drawBBox(Dinasaur specie);

public:
    float velX;                   
    float velY;
    bool onGround;

    Player(Dinasaur t, float posX, float posY);                           // construtor
    ~Player();                          // destrutor

    void OnCollision(Object * obj);     // resolução da colisão
    void Update();                      // atualização do objeto
    void Draw();                        // desenho do objeto

	uint Specie() const { return specie; } 

    float Left() const { return x + bLeft; }
    float Right() const { return x + bRight; }
    float Top() const { return y + bTop; }
    float Bottom() const { return y + bBottom; }
};

// ---------------------------------------------------------------------------------
// Função Membro Inline

inline void Player::Draw()
{
    sprite->Draw(x, y, z); 
}

// ---------------------------------------------------------------------------------

#endif