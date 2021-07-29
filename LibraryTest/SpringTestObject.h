#pragma once
#include <GameObject.h>
#include"ModelObject.h"
#include"FrameTimer.h"
//ï°êîÇ¬Ç»Ç∞ÇÊÇ§
class SpringTestObject :
    public GameObject
{
private:
	std::unique_ptr<ModelObject>model;

	SpringTestObject* nextObject = nullptr;
	SpringTestObject* preObject = nullptr;
	const float DISTANCE = 7.0f;
	Vector3 modelScale = 4;


	static UINT score;
	static UINT preScore;


	FrameTimer speedUpTimer;
public:
	SpringTestObject(SpringTestObject* preObject);
	void Update()override;
	void Draw()override;
	void CalcSpring();
	void Hit
	(
		const GameObject* const  object,
		const CollisionType collisionType,
		const int arrayNum,
		const CollisionType hitObjColType,
		const int hitObjArrayNum
	)override;

	void SetNextObject(SpringTestObject* nextObject) { this->nextObject = nextObject; }

	static UINT GetScore() { return score; }
	static void ResetScore() { score = 0; }
};

