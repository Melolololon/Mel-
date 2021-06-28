#pragma once
#include"Vector.h"
#include<vector>
#include<string>
#include<memory>
#include<unordered_map>
#include"DirectInput.h"

class Camera
{
public:
	enum class CameraMode
	{
		CAMERA_MODE_FPS,//1�l�̎��_
		CAMERA_MODE_TPS,//3�l�̎��_
	};

private:
	using UINT = unsigned int;

	static std::unordered_map<std::string, std::unique_ptr<Camera>>pCameras;
	static UINT createCount;
	static std::string mainCameraName;

	CameraMode cameraMode = CameraMode::CAMERA_MODE_FPS;
	

	//��p
	float fovY = 60.0f;

	//�ŋߓ_
	float nearNum = 0.01f;
	//�ŉ��_
	float farNum = 1000.0f;

	//�J�������W���璍���_�̋���
	float cameraToTargetDistance = 1.0f;

	//��]������Ƃ��̊�ʒu(FPS���[�h�ł̓J�������W�ATPS���_�ł͒����_���W)
	Vector3 rotatePosition = Vector3(0,0,-10);
	Vector3 angle = 0;

	Vector3 cameraPosition = rotatePosition;
	Vector3 targetPosition = rotatePosition + Vector3(0, 0, cameraToTargetDistance);
	Vector3 upVector = Vector3(0, 1, 0);

	void CalcCameraData();
public:
	Camera(){}
	~Camera(){}

	/// <summary>
	/// �J�����𐶐����܂��B
	/// </summary>
	/// <param name="name"></param>
	static void Create(const std::string& name = "");
	
	/// <summary>
	/// �J�������폜���܂��B
	/// </summary>
	/// <param name="name"></param>
	static void Delete(const std::string& name);

	/// <summary>
/// �J�����̃|�C���^���擾���܂��B
/// </summary>
/// <param name="name">�J�����̖��O(����)</param>
/// <returns></returns>
	static Camera* Get(const std::string& name = mainCameraName) { return pCameras[name].get(); }


#pragma region �Z�b�g

	/// <summary>
	/// �J��������]������Ƃ��̊�ʒu��ݒ肵�܂��BFPS���[�h���̓J�����̍��W���ATPS���_���͒����_�̍��W���Z�b�g���܂��B
	/// </summary>
	/// <param name="position">��]����Ƃ��̊�ƂȂ���W</param>
	void SetRotatePosition(const Vector3& position);
	

	/// <summary>
	/// �p�x���Z�b�g���܂��B�p�x��(0,0,0)�̏ꍇ�A�J������0,0,1�̕����������܂��BFPS���[�h���̓J�������W����ɒ����_���ATPS���_���͒����_����ɃJ�����̍��W����]�����܂��B
	/// </summary>
	/// <param name="angle">�J�����̊p�x</param>
	void SetAngle(const Vector3& angle);

	/// <summary>
	/// ��p���Z�b�g���܂��B
	/// </summary>
	/// <param name="fovY">��p</param>
	void SetFovY(const float fovY) { this->fovY = fovY; }

	/// <summary>
	/// �J��������J�����̕\���͈͂̈�ԋ߂��ꏊ�܂ł̋������Z�b�g���܂��B
	/// </summary>
	/// <param name="num">near�l</param>
	void SetNear(const float num) 
	{
		nearNum = num; 
		DirectInput::SetNear(nearNum);
	}

	/// <summary>
	/// �J��������J�����̕\���͈͂̈�ԉ����ꏊ�܂ł̋������Z�b�g���܂��B
	/// </summary>
	/// <param name="num">far�l</param>
	void SetFar(const float num) 
	{
		farNum = num; 
		DirectInput::SetFar(farNum);
	}

	/// <summary>
	/// �J�����ƒ����_�̋������Z�b�g���܂��B���TPS���_�̃J�������������邽�߂Ɏg�p���܂��B�����l��1.0f�ł��B
	/// </summary>
	/// <param name="distance">�J�����ƒ����_�̋���</param>
	void SetCameraToTargetDistance(const float distance);

	/// <summary>
	/// �J�����̃��[�h���Z�b�g���܂��B
	/// </summary>
	/// <param name="mode">�J�����̃��[�h</param>
	void SetCameraMode(const CameraMode mode);

#pragma endregion

#pragma region �Q�b�g

	
	/// <summary>
	/// ���C���J����(���C�u�����ŗp�ӂ��ꂽ�J����)�̖��O���擾���܂��B
	/// </summary>
	/// <returns></returns>
	static const std::string& GetMainCameraName() { return mainCameraName; }

	DirectX::XMMATRIX GetViewMatrix()const;
	DirectX::XMMATRIX GetProjectionMatrix()const;

	/// <summary>
	/// �r���[�s��ƃv���W�F�N�V�����s����|�����s����擾���܂��B
	/// </summary>
	/// <returns></returns>
	DirectX::XMMATRIX GetViewAndProjectionMat()const;

	/// <summary>
	/// �J�����̍��W���擾���܂��B
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	Vector3 GetCameraPosition()const { return cameraPosition; }

	/// <summary>
	/// �����_�̍��W���擾���܂��B
	/// </summary>
	/// <returns>�����_�̍��W</returns>
	Vector3 GetTargetPosition()const { return targetPosition; }

	/// <summary>
	/// ��x�N�g�����擾���܂��B
	/// </summary>
	/// <returns>��x�N�g��</returns>
	Vector3 GetUpVector()const { return upVector; }

	
	Vector3 GetCameraAngle()const { return angle; }

#pragma endregion
};

