#pragma once
#include<string>
#include<unordered_map>
#include<memory>

#include"Vector.h"
#include"Color.h"

class DirectionalLight
{
private:
	using UINT = unsigned int;
	static std::unordered_map<std::string, std::unique_ptr<DirectionalLight>>pLights;
	static UINT createCount;
	static std::string mainCameraName;

	Vector3 direction = Vector3(0, 0, 1);
	Color color = Color(255, 255, 255, 255);
public:

	DirectionalLight() {}
	~DirectionalLight() {}


	static void Create(const std::string& name = "");
	static void Delete(const std::string& name);
	static DirectionalLight& Get(const std::string& name = mainCameraName) { return *pLights[name]; }

	//���C�g�̌������Z�b�g���܂��B
	void SetDirection(const Vector3& vector);
	void SetColor(const Color& color) { this->color = color; }

	Vector3 GetDirection()const { return direction; }
	Color GetColor()const { return color; }
};
