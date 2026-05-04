#ifndef _ARENA3_H_
#define _ARENA3_H_

#include "Arena1.h"
#include "GameOver.h"

class Arena3 : public Arena1
{
public:
    void Init();
    void DrawSscoreBoard();
    void Draw();

    bool TreatMatchEnding();
};

#endif