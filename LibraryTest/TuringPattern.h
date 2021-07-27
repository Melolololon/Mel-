#pragma once
#include"Vector.h"
#include"Values.h"
#include"Sprite3D.h"
#include<memory>
class TuringPattern
{
private:

	//マス数
	static const int CELL_NUM_X = 40;
	static const int CELL_NUM_Y = 40;
	//フィールドの1マス1マスにuとv(今回の場合はxとy)の要素を持たせる
	
	std::array<std::array<Vector2, CELL_NUM_X>, CELL_NUM_Y>field;
	std::array<std::array<Vector2, CELL_NUM_X>, CELL_NUM_Y>preCalcField;


	std::unique_ptr<Sprite3D>turingPatternSpr[CELL_NUM_Y][CELL_NUM_X];

	//fieldの計算
	void CalcTuringPattern();

	//拡散項の計算関数
	Vector2 CalcDiffusion(int x ,int y);

	//反応項の計算
	Vector2 CalcReaction(int x, int y);

public:
	void Initialize();
	void Update();
	void Draw();
};

