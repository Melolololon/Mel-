#include "Curve.h"

void Curve::SetPoints(const std::vector<Vector2>& points)
{
	auto pointsSize = points.size();
	this->points.resize(pointsSize);
	for (int i = 0; i < pointsSize; i++)
		this->points[i] = points[i];

}

void Curve::SetPoints(const std::vector<Vector3>& points)
{
	this->points = points;
}


Vector2 Curve::GetVector2Position()
{
	//ŒvZ—Ê‘‚â‚³‚È‚¢‚½‚ß‚ÉAVector2‚Æ3‚Å‘‚­‚Ì•ª‚¯‚Ä‚é

	Vector2 pos = 0;

	auto pointsSize = points.size();
	int powNum = static_cast<int>(pointsSize) - 1;
	float mulT = 1.0f;
	for(int i = 0; i < pointsSize - 1;i++)
	{
		float mulNum = 1.0f;
		if (i != 0)
			mulNum = pointsSize - 1.0f;

		float calcPowNum = 1.0f;
		for (int j = 0; j < powNum; j++)
			calcPowNum *= (1.0f - t);
		
		pos += mulNum * points[i] * calcPowNum * mulT;

		mulT *= t;
		powNum--;
	}

	pos += points[pointsSize - 1] * mulT;

	return pos;
}

Vector3 Curve::GetVector3Position()
{
	Vector3 pos = 0;

	auto pointsSize = points.size();
	int powNum = static_cast<int>(pointsSize) - 1;
	float mulT = 1.0f;
	for (int i = 0; i < pointsSize - 1; i++)
	{
		float mulNum = 1.0f;
		if (i != 0)
			mulNum = pointsSize - 1.0f;

		float calcPowNum = 1.0f;
		for (int j = 0; j < powNum; j++)
			calcPowNum *= (1.0f - t);

		pos += mulNum * points[i] * calcPowNum * mulT;

		mulT *= t;
		powNum--;
	}

	pos += points[pointsSize - 1] * mulT;

	return pos;
}

void Curve::AddNumber(const float num)
{
	t += num;

	if (t >= 1.0f) 
	{
		if (isMaxNumberReset)
			t = 0.0f;
		else
			t = 1.0f;
	}
}
