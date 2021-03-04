#pragma once
#include"Vector.h"
#include"Color.h"

enum SikibetuNumber 
{
	Not,
	Box,
	Circle,
};

struct PolyData
{
	//構造体に構造体は無理?
	//intが使えない?
	//posを四つのみにする
	Vector3 pos1, pos2, pos3, pos4;
	int num1, num2;
	float fNum1, fNum2;

	short katatiNum;
	//配列番号
	int sikibetuNum;
	int* sikibetuNumP;

	int dimention;

};

struct HeapData
{
	const wchar_t* path;

	int objectNum;

	Color color;
	Vector3 angle;


	int sikibetuNum;
	int* sikibetuNumP;
};
