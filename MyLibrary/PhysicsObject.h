#pragma once
#include "GameObject.h"

//GameObject�ɏd���Ȃǂ̃p�����[�^��������������
class PhysicsObject :
    public GameObject
{
private:

    //�����x
    Vector3 acceleration = 0;
    //���̂�������
    Vector3 force = 0;
    //�d��
    float mass = 0;
public:

    Vector3 GetAcceleration() { return acceleration; }
    Vector3 GetForce() { return force; }
    float GetMass() { return mass; }

};

