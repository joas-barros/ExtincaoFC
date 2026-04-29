#include "Grass.h"

Grass::Grass(float posX, float posY)
{
    MoveTo(posX, posY, Layer::UPPER);

    sprite = new Sprite("Resources/grass.png");

    // cria bounding box
    BBox(new Rect(-sprite->Width() / 2.0f,
        -sprite->Height() / 2.0f,
        sprite->Width() / 2.0f,
        sprite->Height() / 2.0f));
}

Grass::~Grass()
{
    delete sprite;
}

void Grass::Update()
{
    // lógica de atualização do objeto
}