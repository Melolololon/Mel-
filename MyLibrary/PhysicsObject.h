#pragma once
#include "GameObject.h"

//GameObjectに重さなどのパラメータを持たせたもの
class PhysicsObject :
    public GameObject
{
private:

    //加速度
    Vector3 acceleration = 0;
    //物体が動く力
    Vector3 force = 0;
    //重さ
    float mass = 0;
public:

    Vector3 GetAcceleration() { return acceleration; }
    Vector3 GetForce() { return force; }
    float GetMass() { return mass; }

};

