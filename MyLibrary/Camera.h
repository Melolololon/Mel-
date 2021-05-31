#pragma once
#include"Vector.h"
#include"LibMath.h"
class Camera
{
	
private:
	Vector3 position;
	Vector3 angle;
	float fovY;
	
public:
	/// <summary>
	/// ���W���Z�b�g���܂��B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& position) { this->position = position; }

	/// <summary>
	/// �p�x���Z�b�g���܂��B�p�x��0,0,0�̏ꍇ�A0,0,1�̕����������܂��B
	/// </summary>
	/// <param name="angle"></param>
	void SetAngle(const Vector3& angle) { this->angle = angle; }

	/// <summary>
	/// ��p���Z�b�g���܂��B
	/// </summary>
	/// <param name="fovY"></param>
	void SetFovY(const float fovY) { this->fovY = fovY; }


};

