#include "Physics.h"
#include"LibMath.h"


const float Physics::GRAVITATIONAL_ACCELERATION_MERCURY = 3.7f;
const float Physics::GRAVITATIONAL_ACCELERATION_VENUS = 8.87f;
const float Physics::GRAVITATIONAL_ACCELERATION_EARTH = 9.807f;
const float Physics::GRAVITATIONAL_ACCELERATION_MARS = 3.721f;
const float Physics::GRAVITATIONAL_ACCELERATION_JUPITER = 24.79f;
const float Physics::GRAVITATIONAL_ACCELERATION_SATURN = 10.44f;
const float Physics::GRAVITATIONAL_ACCELERATION_URANUS = 8.87f;
const float Physics::GRAVITATIONAL_ACCELERATION_NEPTUNE = 11.15f;
const float Physics::GRAVITATIONAL_ACCELERATION_PLUTO = 0.62f;
const float Physics::GRAVITATIONAL_ACCELERATION_MOON = 1.62f;

Value2<Vector3> Physics::CalcRepulsionVelocity
(
    const Value2<Vector3>& position, 
    const Value2<Vector3>& velocity, 
    const Value2<float>& mass, 
    const Value2<Vector3>& e
)
{
    if (mass.v1 < 0.0f || mass.v2 < 0.0f)return velocity;

    //計算前のvelocityを格納
    const Value2<Vector3> calcPreVel(velocity);

    Value2<Vector3> calcVel(0.0f, 0.0f);
    //Lenghtの部分違うかも?でも計算この前合ってたような
    Vector3 n = LibMath::OtherVector(position.v1, position.v2);
    calcVel.v1 = -(((((1 + e.v2) * mass.v1 * mass.v2) / (mass.v1 + mass.v2)) * (calcPreVel.v1 - calcPreVel.v2) * n) * n) / mass.v1 + calcPreVel.v1;
    calcVel.v2 = -(((((1 + e.v1) * mass.v1 * mass.v2) / (mass.v1 + mass.v2)) * (calcPreVel.v2 - calcPreVel.v1) * n) * n) / mass.v2 + calcPreVel.v2;
   
    //calcVel.v1 = -((calcPreVel.v1 - calcPreVel.v2) * n) * n + calcPreVel.v1;
    //calcVel.v2 = -((calcPreVel.v2 - calcPreVel.v1) * n) * n + calcPreVel.v2;

    return calcVel;
}

Vector3 Physics::CalcSpringVelocity
(
    const Vector3& currentPos,
    const Vector3& currentVel,
    const Vector3& rootPos,
    const float naturalDis,
    /*const float maxDis,*/
    const float mass,
    const float gravitationalAcceleration,
    const float springConstant,
    const float viscousDragCoefficient
)
{
    Vector3 acc;
    acc.y = -gravitationalAcceleration;

    Vector3 currentDisV3 = rootPos - currentPos;
    float currentDisF = currentDisV3.Length();
    Vector3 calcDis = 0.0f;
    if(currentDisF > naturalDis)
    {
        calcDis = currentDisV3 * (currentDisF - naturalDis) / naturalDis;
        acc += calcDis * springConstant / mass;
    }
    //else if(currentDisF < naturalDis)
    //{

    //}
    Vector3 vel = currentVel;
    vel += acc;
    vel -= vel * viscousDragCoefficient;
    return vel;
}
