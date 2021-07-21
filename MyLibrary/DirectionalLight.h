#pragma once
#include<string>
#include<unordered_map>
#include<memory>

#include"Vector.h"
#include"Color.h"
#include"Values.h"

//Lightクラス作ってまとめる?

//
class DirectionalLight
{
private:
	using UINT = unsigned int;
	static std::unordered_map<std::string, std::unique_ptr<DirectionalLight>>pLights;
	static UINT createCount;
	static std::string mainCameraName;

	Vector3 direction = Vector3(0, 0, 1);
	Color color = Color(255, 255, 255, 255);
	Value3<float> lightPower = Value3<float>(1.0f, 1.0f, 1.0f);

public:

	DirectionalLight() {}
	~DirectionalLight() {}

 
	static void Create(const std::string& name = "");
	static void Delete(const std::string& name);
	static DirectionalLight& Get(const std::string& name = mainCameraName) { return *pLights[name]; }

	//ライトの向きをセットします。
	void SetDirection(const Vector3& vector);
	void SetColor(const Color& color) { this->color = color; }

	Vector3 GetDirection()const { return direction; }
	Color GetColor()const { return color; }
};

