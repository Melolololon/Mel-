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
    //åvéZëOÇÃvelocityÇäiî[
    const Value2<Vector3> calcPreviousVelocity(velocity);

    Value2<Vector3> calcVel(0.0f, 0.0f);
    //LenghtÇÃïîï™à·Ç§Ç©Ç‡?Ç≈Ç‡åvéZÇ±ÇÃëOçáÇ¡ÇƒÇΩÇÊÇ§Ç»
    Vector3 n = LibMath::OtherVector(position.v1, position.v2);
    calcVel.v1 = -(((((1 + e.v2) * mass.v1 * mass.v2) / (mass.v1 + mass.v2)) * (calcPreviousVelocity.v1 - calcPreviousVelocity.v2) * n) * n) / mass.v1 + calcPreviousVelocity.v1;
    calcVel.v2 = -(((((1 + e.v1) * mass.v1 * mass.v2) / (mass.v1 + mass.v2)) * (calcPreviousVelocity.v2 - calcPreviousVelocity.v1) * n) * n) / mass.v2 + calcPreviousVelocity.v2;

    return calcVel;
}
