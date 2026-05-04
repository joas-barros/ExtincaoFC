#ifndef _HELP_H_
#define _HELP_H_

#include "Game.h"
#include "Sprite.h"

class Help : public Game
{
private:
    Sprite* backg = nullptr;

public:
    void Init();
    void Update();
    void Draw();
    void Finalize();
};

#endif