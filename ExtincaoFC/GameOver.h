#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "Arena1.h"
#include <string>

using std::string;

class GameOver : public Arena1
{
private:
    string winnerText; 
    string scoreText;  
    string goalsText;

public:
    void Init();
    void Update();
    void Draw();
    void Finalize(); 
};

#endif