#pragma once
#include"Vector.h"
#include"LibMath.h"
class Camera
{
	
private:
	Vector3 position = 0;
	Vector3 angle = 0;
	float fovY = 60.0f;
	float nearNum = 0.01f;
	float farNum = 1000.0f;
	
public:

#pragma region �Z�b�g

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

	void SetNear(const float num) { nearNum = num; }
	void SetFar(const float num) { farNum = num; }

#pragma endregion


	/// <summary>
	/// �r���[�ƃv���W�F�N�V�������|�����s����擾�B
	/// </summary>
	/// <returns></returns>
	DirectX::XMMATRIX GetViewAndProjectionMat();
};

