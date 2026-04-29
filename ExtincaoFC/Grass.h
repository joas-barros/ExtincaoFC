
#ifndef _EXTINCAOFC_GRASS_H_
#define _EXTINCAOFC_GRASS_H_

// ---------------------------------------------------------------------------------

#include "Types.h"                      // tipos específicos da engine
#include "Object.h"                     // interface de Object
#include "Sprite.h"                     // background é composto por sprites
#include "Grass.h"

// ---------------------------------------------------------------------------------

class Grass : public Object
{
private:
    Sprite* sprite;                  // sprite da plataforma

public:
    Grass(float posX, float posY);
    ~Grass();

    void Update();                      // atualização do objeto
    void Draw();                        // desenho do objeto
};

// ---------------------------------------------------------------------------------
// Função Membro Inline

inline void Grass::Draw()
{
    sprite->Draw(x, y, z);
}

// ---------------------------------------------------------------------------------

#endif
