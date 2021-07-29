#pragma once
#include"Scene.h"
#include"TuringPattern.h"
#include"SpringTestObject.h"
#include<memory>
#include"FrameTimer.h"

#include"Player.h"
class HurikoGame :public Scene
{
public:
	enum class GameState
	{
		TITLE,
		PLAY_PRE,
		PLAY,
		RESULT
	};

private:
	TuringPattern tPattern;

	Vector3 topPos = Vector3(0, 50, 0);
	Vector3 topRootPos = topPos + Vector3(0, 10, 0);
	Vector3 movePos = 0;
	static const int SPRING_OBJECT_NUM = 4;
	std::array<std::shared_ptr<SpringTestObject>, SPRING_OBJECT_NUM>springObjects;


	static const int GAME_TIME = 60 * 30;
	static const int START_PRE_TIME = 60 * 1;
	 FrameTimer gameTimer;
	 FrameTimer resultPushTimer;


	Vector3 angle = 0;

	std::shared_ptr<Player>player;

	std::array<std::unique_ptr<Sprite3D>, 4 >wallSpr;

	static const Vector3 FIELD_SIZE;

	static GameState gameState;

	Vector2 titlePos = Vector2(765, -100);
	float titleT = 0.0f;
public:
	HurikoGame();
	~HurikoGame();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finitialize()override;
	Scene* GetNextScene()override;

	static void LoadResources();

	static Vector3 GetFieldSize() { return FIELD_SIZE; }
	static GameState GetGameState() { return gameState; }
};

