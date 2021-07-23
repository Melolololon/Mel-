#include "Physics.h"


float Physics::CalcCoefficientOfRestitution(const Vector3& hitPreviousVelocity, const Vector3& hitAfterVelocity)
{
    return 1;
}

Value2<Vector3> Physics::CalcRepulsionVelocity
(
    const Value2<Vector3>& position, 
    const Value2<Vector3>& velocity, 
    const Value2<float>& mass, 
    const Value2<Vector3>& e
)
{
    //�v�Z�O��velocity���i�[
    const Value2<Vector3> calcPreviousVelocity(velocity);

    Value2<Vector3> calcVel(0.0f, 0.0f);
    //Lenght�̕����Ⴄ����?�ł��v�Z���̑O�����Ă��悤��
    Vector3 n = (position.v2 - position.v1) / (Vector3::Length(position.v2) - Vector3::Length(position.v1));
    calcVel.v1 = -(((((1 + e.v1) * mass.v1 * mass.v2) / (mass.v1 + mass.v2)) * (velocity.v1 - velocity.v2) * n) * n) / mass.v1 + velocity.v1;

    return calcVel;
}
