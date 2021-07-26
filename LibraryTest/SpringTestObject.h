#pragma once
#include <GameObject.h>
#include"ModelObject.h"

//ï°êîÇ¬Ç»Ç∞ÇÊÇ§
class SpringTestObject :
    public GameObject
{
private:
	std::unique_ptr<ModelObject>model;
	std::unique_ptr<ModelObject>rootModel;
	Vector3 rootPos;

public:
	SpringTestObject(const Vector3& pos, const Vector3& rootPos);
	void Update()override;
	void Draw()override;

	void SetRootPosition(const Vector3& pos);
};

