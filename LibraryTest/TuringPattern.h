#pragma once
#include"Vector.h"
#include"Values.h"
#include"Sprite2D.h"
#include<memory>
class TuringPattern
{
private:

	//�}�X��
	static const int CELL_NUM_X = 100;
	static const int CELL_NUM_Y = 100;
	//�t�B�[���h��1�}�X1�}�X��u��v(����̏ꍇ��x��y)�̗v�f����������
	
	std::array<std::array<Vector2, CELL_NUM_X>, CELL_NUM_Y>field;
	std::array<std::array<Vector2, CELL_NUM_X>, CELL_NUM_Y>preCalcField;


	std::unique_ptr<Sprite2D>turingPatternSpr[CELL_NUM_Y][CELL_NUM_X];

	//field�̌v�Z
	void CalcTuringPattern();

	//�g�U���̌v�Z�֐�
	Vector2 CalcDiffusion(int x ,int y);

	//�������̌v�Z
	Vector2 CalcReaction(int x, int y);

public:
	void Initialize();
	void Update();
	void Draw();
};

