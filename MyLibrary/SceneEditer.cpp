#include "SceneEditer.h"

#include"ImguiManager.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"Input.h"

#include<fstream>

void MelLib::SceneEditer::Save()
{
	std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	name += ".mlsce";
	std::ofstream file(name);

	// 書き出し


	file.close();
}

MelLib::SceneEditer* MelLib::SceneEditer::GetInstance()
{
	static SceneEditer s;
	return &s;
}

void MelLib::SceneEditer::RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject , const std::string& objectType)
{
	// C++20のcontainsに置き換えできる
	//if (pObjects.find(objectType) == pObjects.end()) 
	//{
	//	std::string text = "シーンエディタには既に"+ objectType + "の名前で他のオブジェクトが登録されています。\n";
	//	OutputDebugStringA(text.c_str());
	//	return;
	//}

	pObjects.try_emplace(objectType, std::vector<std::shared_ptr<MelLib::GameObject>>());
	pObjects[objectType].push_back(pObject);
	//test.push_back(pObject);
	objectOrderDatas.try_emplace(pObjects.size(), objectType);
	int z = 0;
}

void MelLib::SceneEditer::Update()
{
	// 追加
	if (Input::KeyTrigger(DIK_SPACE)) 
	{
		// GetNewPtr関数でメモリ確保
		std::shared_ptr<GameObject>pObject = pSelectObject->GetNewPtr();

		if (pObject) 
		{
			// 追加
			GameObjectManager::GetInstance()->AddObject(pObject);
		}
		else 
		{
			std::string text = typeid(*pObject).name();
			text += "のGetNewPtrがnullptrを返しました。オブジェクトが追加できません。\n";
			OutputDebugStringA(text.c_str());
		}
	}

}

void MelLib::SceneEditer::Draw()
{
	ImguiManager::GetInstance()->BeginDrawWindow("Edit");


	// キーの数だけラジオボタン描画
	int count = 0;
	for (const auto& object : pObjects) 
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}

	// ラジオボタンで選択したオブジェクト一覧の参照
	std::vector < std::shared_ptr<MelLib::GameObject>>& refObjects = pObjects[objectOrderDatas[count]];

	// ここにスライダー作成処理
	int sliderNum = 0;
	ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	// 選ばれたオブジェクトのポインタをpSelectObjectに代入
	pSelectObject = refObjects[sliderNum].get();

	ImguiManager::GetInstance()->EndDrawWindow();
}
