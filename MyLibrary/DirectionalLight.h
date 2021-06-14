#pragma once
#include<string>
#include<unordered_map>
#include<memory>

#include"Vector.h"

class DirectionalLight
{
private:
	using UINT = unsigned int;
	static std::unordered_map<std::string, std::unique_ptr<DirectionalLight>>pLights;
	static UINT createCount;
	static std::string mainCameraName;

	Vector3 direction = Vector3(0, 0, 1);

public:
	
	DirectionalLight(){}
	~DirectionalLight(){}


	static void Create(const std::string& name = "");
	static void Delete(const std::string& name);
	static DirectionalLight& Get(const std::string& name = mainCameraName) { return *pLights[name]; }

	//ライトの向きをセットします。
	void SetDirection(const Vector3& vector);

	Vector3 GetDirection()const { return direction; }

};

