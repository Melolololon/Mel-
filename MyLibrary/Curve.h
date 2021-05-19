#pragma once
#include<vector>
#include"Vector.h"
//ベジェ、スプラインなどの曲線クラス
class Curve
{
	
private:
	float t = 0.0f;
	std::vector<Vector3> points;
	bool isMaxNumberReset = false;
public:
	Curve(){}
	~Curve(){}

	void SetPoints(const std::vector<Vector2>& points);
	void SetPoints(const std::vector<Vector3>& points);

	void AddNumber(const float num);

	Vector2 GetVector2Position();
	Vector3 GetVector3Position();

	void SetIsMaxNumberReset(const bool flag) { isMaxNumberReset = flag; }
};

