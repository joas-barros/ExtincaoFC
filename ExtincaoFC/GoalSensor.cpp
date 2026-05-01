#include "GoalSensor.h"
#include "ExtincaoFC.h"

GoalSensor::GoalSensor(Goal* g)
{
    targetGoal = g;
    hasScored = false;
    type = SENSOR;

    // 1. O Sensor se move exatamente para o mesmo centro do Gol
    MoveTo(targetGoal->X(), targetGoal->Y());

    // 2. O Sensor pede ao Gol as medidas do gatilho lidas do arquivo .txt!
    Rect* sRect = targetGoal->SensorBBox();

    if (sRect != nullptr) {
        // Copia perfeitamente os limites definidos no texto
        BBox(new Rect(sRect->Left(), sRect->Top(), sRect->Right(), sRect->Bottom()));
    }
    else {
        // Prevenção de erro caso você esqueça de botar o "SENSOR" no txt
        BBox(new Rect(0, 0, 0, 0));
    }
}

GoalSensor::~GoalSensor()
{
}

void GoalSensor::Update()
{
}

void GoalSensor::OnCollision(Object* obj)
{
    if (obj->Type() == BALL)
    {
        if (!hasScored)
        {
            targetGoal->GetScorer()->IncreaseScore();
            hasScored = true;
        }
    }
}