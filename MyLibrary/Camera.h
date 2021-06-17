#pragma once
#include"Vector.h"
#include<vector>
#include<string>
#include<memory>
#include<unordered_map>
#include"DirectInput.h"

class Camera
{

private:
	using UINT = unsigned int;

	static std::unordered_map<std::string, std::unique_ptr<Camera>>pCameras;
	static UINT createCount;
	static std::string mainCameraName;

	Vector3 position = Vector3(0,0,-10);
	Vector3 angle = 0;
	float fovY = 60.0f;
	float nearNum = 0.01f;
	float farNum = 1000.0f;
	
	
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
/// <param name="name"></param>
/// <returns></returns>
	static Camera* Get(const std::string& name = mainCameraName) { return pCameras[name].get(); }


#pragma region �Z�b�g

	/// <summary>
	/// ���W���Z�b�g���܂��B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& position) 
	{
		this->position = position; 

		//��
		DirectInput::SetViewMatrix(GetViewMatrix());
		DirectInput::SetProjectionMatrix(GetProjectionMatrix());
	}

	/// <summary>
	/// �p�x���Z�b�g���܂��B�p�x��0,0,0�̏ꍇ�A�J������0,0,1�̕����������܂��B
	/// </summary>
	/// <param name="angle"></param>
	void SetAngle(const Vector3& angle)
	{
		this->angle = angle; 

		//��
		DirectInput::SetViewMatrix(GetViewMatrix());
		DirectInput::SetProjectionMatrix(GetProjectionMatrix());
	}

	/// <summary>
	/// ��p���Z�b�g���܂��B
	/// </summary>
	/// <param name="fovY"></param>
	void SetFovY(const float fovY) { this->fovY = fovY; }

	/// <summary>
	/// �J��������J�����̕\���͈͂̈�ԋ߂��ꏊ�܂ł̋������Z�b�g���܂��B
	/// </summary>
	/// <param name="num"></param>
	void SetNear(const float num) 
	{
		nearNum = num; 
		DirectInput::SetNear(nearNum);
	}

	/// <summary>
	/// �J��������J�����̕\���͈͂̈�ԉ����ꏊ�܂ł̋������Z�b�g���܂��B
	/// </summary>
	/// <param name="num"></param>
	void SetFar(const float num) 
	{
		farNum = num; 
		DirectInput::SetFar(farNum);
	}


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

	Vector3 GetCameraAngle()const { return angle; }

#pragma endregion
};

