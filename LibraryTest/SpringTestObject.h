#pragma once
#include <GameObject.h>
#include"ModelObject.h"

//•¡”‚Â‚È‚°‚æ‚¤
class SpringTestObject :
    public GameObject
{
private:
	std::unique_ptr<ModelObject>model;

	SpringTestObject* nextObject = nullptr;
	SpringTestObject* preObject = nullptr;
	const float DISTANCE = 5.0f;
	Vector3 modelScale = 1;
public:
	SpringTestObject(SpringTestObject* preObject);
	void Update()override;
	void Draw()override;
	void CalcSpring();

	void SetNextObject(SpringTestObject* nextObject) { this->nextObject = nextObject; }
};

