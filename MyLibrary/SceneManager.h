#pragma once
#include<vector>
#include<unordered_map>
#include"Scene.h"

namespace MelLib 
{
	class SceneManager final
	{

	private:

		SceneManager();
		~SceneManager();

		Scene* currentScene = nullptr;

		//bool stopUpdate = false;

		/// <summary>
		/// シーンの切り替え処理を行う
		/// </summary>
		void Change();
	public:

		SceneManager(const SceneManager& sceneManager) = delete;//コピーコンストラクタ
		SceneManager& operator=(const SceneManager& sceneManager) = delete;//コピー代入演算子
		static SceneManager* GetInstance();

		void SetStartScene(Scene* startScene);
		void Update();
		void Draw();
		void Finalize();


		/// <summary>
		/// シーンを切り替える処理を行います
		/// </summary>
		/// <param name="nextScene"></param>
		//void ChengeScene(Scene* nextScene);

		//void SetStopFlag(bool flag) { stopUpdate = flag; }
		//void ChangeStopFlag() { stopUpdate = !stopUpdate; }

		//void ResetScene();

		Scene* GetCurrentScene();
		std::string GetCurrentSceneName();

		/// <summary>
		/// シーンを再読み込みします。
		/// </summary>
		void ReLoadScene();
	};

}