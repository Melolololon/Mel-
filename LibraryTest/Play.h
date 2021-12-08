#pragma once
#include"Scene.h"
#include"Sprite2D.h"
#include"ModelObject.h"

class Play:public MelLib::Scene
{
private:
	std::unique_ptr<MelLib::Sprite2D>sprite2DTest;
	MelLib::Vector2 scale = 1;
	MelLib::Vector3 angle;

#pragma region 判定テスト


	MelLib::Vector2 testScale = 1;
	void CollisionTestInitialize();
	void CollisionTestUpdate();
	void CollisionTestDraw();
#pragma endregion

#pragma region レンダーターゲットテスト
	MelLib::ModelObject rtTestObject;
	MelLib::ModelObject frontObj;
	MelLib::Vector3 frontPos;
	MelLib::ModelObject backObj;
	MelLib::Vector3 backPos;
	bool isCat = false;

	bool isNormalize = false;
	bool isReset = false;
	MelLib::PlaneData planeData;

	void CatInitialize();
	void CatTestUpdate();
	void CatTestDraw();


#pragma endregion


	void ParticleTestInitialize();
	void ParticleTestUpdate();
	void ParticleTestDraw();

#pragma region アニメーションテスト

	void AniTestInitialize();
	void AniTestUpdate();
	void AniTestDraw();


	MelLib::ModelObject aniTest;
	MelLib::ModelObject aniCollTest;
	MelLib::Vector3 aniCollInitPos = MelLib::Vector3(-0.9, 2, 0.785);
#pragma endregion



public:
	Play();
	~Play();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;
	Scene* GetNextScene()override;

	static void LoadResources();
};

