#include "GoalSensor.h"
#include "Arena1.h"
#include "Ball.h"
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
            Ball* ball = (Ball*)obj;
            Circle* c = (Circle*)ball->BBox();
			Rect* sensorRect = (Rect*)this->BBox();

            // Bordas reais da bola
            float ballLeft = ball->X() - c->radius;
            float ballRight = ball->X() + c->radius;

            // Calcula a metade exata do retângulo do Sensor
            float sensorMiddle = (sensorRect->Left() + sensorRect->Right()) / 2.0f;

            bool completelyPassed = false;

            if (this->X() > window->CenterX())
            {
                // Verifica se a traseira da bola passou da metade do sensor
                if (ballLeft > sensorMiddle)
                {
                    completelyPassed = true;
                }
            }
            else
            {
                // Verifica se a traseira da bola passou da metade do sensor
                if (ballRight < sensorMiddle)
                {
                    completelyPassed = true;
                }
            }

            if (completelyPassed)
            {
                targetGoal->GetScorer()->IncreaseScore();
                hasScored = true;
                if (gAudio) gAudio->Play(GOAL_SCREAM);
            }
        }
    }
}