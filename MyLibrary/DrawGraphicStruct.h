#pragma once
#include"Vector.h"
#include"Color.h"

enum SikibetuNumber 
{
	Not,
	Box,
	Circle,
};

struct PolygonData
{
	//�\���̂ɍ\���͖̂���?
	//int���g���Ȃ�?
	//pos���l�݂̂ɂ���
	Vector3 pos1, pos2, pos3, pos4;
	int num1, num2;
	float fNum1, fNum2;

	short katatiNum;
	//�z��ԍ�
	int sikibetuNum;
	int* sikibetuNumP;

	int dimention;

};

