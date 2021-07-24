#include "Physics.h"
#include"LibMath.h"

Vector3 Physics::CalcCoefficientOfRestitution
(
    const Value2<Vector3>& position,
    const Value2<Vector3>& hitPreviousVelocity,
    const Value2<Vector3>& hitAfterVelocity
)
{
    Vector3 n = (position.v2 - position.v1) / (Vector3::Length(position.v2) - Vector3::Length(position.v1));
    return ((hitAfterVelocity.v2 - hitAfterVelocity.v1) * n).Abs() / ((hitPreviousVelocity.v2 - hitPreviousVelocity.v1) * n).Abs();
}

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
