#include "Arena3.h"

void Arena3::Init() {
    // 1. Inicializa tudo como na Arena 1 (posições originais)
    Arena1::Init();

    // 2. Apenas troca o fundo para o novo cenário
    delete backg;
    backg = new Sprite("Resources/Sky.png");

    ResetMatch();
}