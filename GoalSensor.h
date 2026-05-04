#ifndef _EXTINCAOFC_GOAL_SENSOR_H_
#define _EXTINCAOFC_GOAL_SENSOR_H_

#include "Types.h"
#include "Object.h"
#include "Goal.h" 

class GoalSensor : public Object
{
private:
    Goal* targetGoal; 
    bool hasScored;

public:
    GoalSensor(Goal* g);
    ~GoalSensor();

    void OnCollision(Object* obj);
    void Update();
    void Draw();

    void Reset() { hasScored = false; }
};

inline void GoalSensor::Draw() {}

#endif