#include "TuringPattern.h"

#include"Random.h"
#include"Library.h"

//u(x)を赤成分、v(y)を青成分として考える?

void TuringPattern::CalcTuringPattern()
{
	//拡散係数
	static const Vector2 DIFFUSION_NUM = Vector2(0.2f, 0.1f);

	//書き換え前を移す
	preCalcField = field;

	for (int y = 0; y < CELL_NUM_Y; y++)
	{
		for (int x = 0; x < CELL_NUM_X; x++)
		{
			Vector2 dResult = CalcDiffusion(x, y) * DIFFUSION_NUM;
			
			if (dResult.x >= 1.0f)
			{
				int bubkiux = 0;
			}
			Vector2 rResult = CalcReaction(x, y);
			field[y][x] += dResult + rResult;

		
		}
	}


}

Vector2 TuringPattern::CalcDiffusion(int x, int y)
{
	

	//gの要素数
	static const int MAP_SIZE = 3;

	//隣接したマスから影響する度合
	static const float G[MAP_SIZE][MAP_SIZE] =
	{
		{0.0f, 1.0f,0.0f},
		{1.0f,-4.0f,1.0f},
		{0.0f, 1.0f,0.0f},
	};

	Vector2 sum = 0;

	for (int my = 0; my < MAP_SIZE; my++)
	{
		int iy = (y + my - 1 + CELL_NUM_Y) % CELL_NUM_Y;
		for (int mx = 0; mx < MAP_SIZE; mx++)
		{
			int ix = (x + mx - 1 + CELL_NUM_X) % CELL_NUM_X;
			sum += preCalcField[iy][ix] * G[my][mx];
		}
	}
	

	return sum;
}

Vector2 TuringPattern::CalcReaction(int x, int y)
{
	const float F = 0.046f;
	const float K = 0.063f;

	//反応項
	Vector2 uv = preCalcField[y][x];

	//fieldに格納されている濃度を取り出し、変化量(反応項)を求める
	Vector2 r;
	r.x = -(uv.x * uv.y * uv.y) + F * (1.0 - uv.x);
	r.y = +(uv.x * uv.y * uv.y) - (F + K) * uv.y;
	return r;
}

void TuringPattern::Initialize()
{
	//中心の方は数値高め、それ以外は適当に乱数セットするっぽい
	//そうじゃなくて、完全ランダムまたは中心のみのどちらかをやるっぽい
	//あと、色は1または0のどちらかをセットする

	static const Value2<int> RANGE = Value2<int>(5, 5);
	Value2<int>centerMin(CELL_NUM_X / 2 - RANGE.v1, CELL_NUM_Y / 2 - RANGE.v2);
	Value2<int>centerMax(CELL_NUM_X / 2 + RANGE.v1, CELL_NUM_Y / 2 + RANGE.v2);


	for(int y = 0; y < CELL_NUM_Y;y++)
	{
		for (int x = 0; x < CELL_NUM_X; x++)
		{
			field[y][x].x = 1.0f;
			field[y][x].y = 0.0f;

			//真ん中らへんは、0から1にして適当にセット
			if (x >= centerMin.v1 && x <= centerMax.v1 && y >= centerMin.v2 && y <= centerMax.v2)
			{
				field[y][x] = Random::GetRandomFloatNumberRangeSelect(0,1, 4);
			}
		

			turingPatternSpr[y][x] = std::make_unique<Sprite3D>(Color(0, 0, 0, 255));
			turingPatternSpr[y][x]->SetAngle(Vector3(90, 0, 0));

			const Vector2 SPR_SIZE = 13;
			turingPatternSpr[y][x]->SetPosition(Vector3(SPR_SIZE.x * (x - CELL_NUM_X / 2), -20, SPR_SIZE.y * (y - CELL_NUM_Y / 2)));
			turingPatternSpr[y][x]->SetScale(SPR_SIZE);


			/*static const Vector2 SPRITE_SIZE = Vector2
			((float)Library::GetWindowWidth() / (float)CELL_NUM_X, (float)Library::GetWindowHeight() / (float)CELL_NUM_Y);
			turingPatternSpr[y][x]->SetScale(SPRITE_SIZE);

			turingPatternSpr[y][x]->SetPosition(Vector2(SPRITE_SIZE.x * x, SPRITE_SIZE.y * y));*/
		}
	}
}


void TuringPattern::Update()
{
	CalcTuringPattern();

}

void TuringPattern::Draw()
{

	for (int y = 0; y < CELL_NUM_Y; y++)
	{
		for (int x = 0; x < CELL_NUM_X; x++)
		{
			float u = field[y][x].x;
			if (u < 0.0f)u = 0.0f;
			if (u > 1.0f)u = 1.0f;
			u *= 100.0f;

			UCHAR r = Color::ParToUChar(u);
			turingPatternSpr[y][x]->SetAddColor(Color(r, 0, 255 - r, 0));

			turingPatternSpr[y][x]->Draw();
		}
	}
}

