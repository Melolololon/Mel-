#pragma once
#include<vector>
#include<unordered_map>
#include"Scene.h"
class SceneManager final
{

private:

	SceneManager();
	~SceneManager();

	std::unordered_map <std::string, Scene*>scenes;//シーン
	std::unordered_map <std::string, Scene*>smallScenes;//シーンで呼び出すシーン()
	std::string currentScene;//現在のシーン
public:

	SceneManager(const SceneManager& sceneManager) = delete;//コピーコンストラクタ
	SceneManager& operator=(const SceneManager& sceneManager) = delete;//コピー代入演算子
	static SceneManager* getInstace();

	void initialize();
	void update();
	void draw();
	void end();

	/// <summary>
	/// シーンの追加
	/// </summary>
	/// <param name="key">シーン名</param>
	/// <param name="scene">シーンクラス</param>
	void addScene(std::string key, Scene* scene);

	/// <summary>
	/// シーンを取得します
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	void* getScenePtr(std::string key);
};

