#include "SceneEditer.h"

#include"ImguiManager.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"GuiValueManager.h"
#include"Input.h"

#include<fstream>

const std::string MelLib::SceneEditer::EDIT_DATA_FORMAT = ".melsce";

void MelLib::SceneEditer::Save()
{

	// 2022_05_01
	// オブジェクトマネージャーに追加したオブジェクトをimguiでいじれるように
	

	std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	name += EDIT_DATA_FORMAT;
	std::ofstream file(name);

	// 書き出し


	file.close();
}

void MelLib::SceneEditer::Load()
{

	// 保存せずに読み込んでいいか聞く

	// ここに指定したシーン名を入れる
	std::string name = "";
	name += EDIT_DATA_FORMAT;
	std::ofstream file;
	file.open(name);

	file.close();
}

void MelLib::SceneEditer::UpdateSelectObject()
{
	if (!isEdit)return;
	
	MelLib::Vector3 pos = pEditSelectObject->GetPosition();
	ImguiManager::GetInstance()->DrawSliderVector3("Position", pos, -1000, 1000);
	pEditSelectObject->SetPosition(pos);

	MelLib::Vector3 angle = pEditSelectObject->GetAngle();
	ImguiManager::GetInstance()->DrawSliderVector3("Angle", angle, 0, 359);
	pEditSelectObject->SetAngle(angle);

	MelLib::Vector3 scale = pEditSelectObject->GetScale();
	ImguiManager::GetInstance()->DrawSliderVector3("Scale", scale, 0, 359);
	pEditSelectObject->SetScale(scale);
}

void MelLib::SceneEditer::DrawObjectList()
{
	// オブジェクトマネージャーに名前だけ取得する関数作ってもいいかも
	// 毎回stringの配列に入れると処理遅くなるからオブジェクト追加時に名前追加していいかも
	// 名前変更した時に変更する処理入れないとそれ出来ない
	// ゲーム中には使わないだろうからある程度遅くてもって感じはする
	// umapにしてキーをGameObjectにすればアクセス楽かも
	// 
	// 同じ名前のオブジェクトがあったら名前に番号付け足す
	// OBJECT_NAMEを定数から変数に変えないといけない
	// 変数にしたらあとから名前変えたりできるから変数にしちゃっていい
	

	// ここソートしてもいいかも
	// ここconstにしたい
	// GameObjectManagerで名前の配列作って管理出来るようにしてからconstに変更末う
	std::vector<std::string>objectNames;
	GameObjectManager::GetInstance()->GetObjectNames(objectNames);
	
	// ImguiManagerにvector私てリスト表示できるようにするから、GetObjectNameにcharの配列渡して受け取る処理作る必要ない
	const size_t OBJECT_SIZE = objectNames.size();
	
	if (OBJECT_SIZE == 0)return;
	
	ImguiManager::GetInstance()->BeginDrawWindow("ObjectList");
	ImguiManager::GetInstance()->DrawList(selectListObjectNum, objectNames);
	selectListObjectName = objectNames[selectListObjectNum];
	ImguiManager::GetInstance()->EndDrawWindow();

	SetDrawWindowFlag(objectNames);

}

void MelLib::SceneEditer::SetDrawWindowFlag(const std::vector<std::string>& objNames)
{
	for (const auto& name : objNames) 
	{
		bool drawFlag = false;
		if (name == selectListObjectName) drawFlag = true;
		GuiValueManager::GetInstance()->SetDrawWindowFlag(name, drawFlag);
	}
}

void MelLib::SceneEditer::RegisterSelectObject()
{
	// 読み込むときは、GetNewPtr関数を使ってNewすればいい
	// そもそも書き出せない?
	// クラス名だけ書き出して、RegisterObjectで登録したオブジェクトのGetNewPtrを呼び出せばOK

	// 登録するオブジェクトの種類と名前を入力させる
	std::string inputObjectName;


	// コピーを生成
	std::shared_ptr<GameObject> object = pEditSelectObject->GetNewPtr();
	pEditSelectObject->CopyObjectData(*object, GameObject::CopyGameObjectContent::EDIT);

	// 入力された名前を設定
	object->SetObjectName(inputObjectName);

	// 登録
	RegisterObject(object, inputObjectType);
}

void MelLib::SceneEditer::InputObjectName()
{
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Name", inputObjectName, 20);
	std::string s = "Object";
	char c[21];
	for (int i = 0; i < 21; i++) 
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\n';
	
	ImGui::InputText("Input Object Name",c, 21);

	inputObjectName = c;

	if (Input::KeyTrigger(DIK_RETURN)) inpttingObjectName = false;
}

void MelLib::SceneEditer::InputObjectType()
{
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Type", inputObjectType, 20);

	std::string s = "Object";
	char c[21];
	for (int i = 0; i < 21; i++)
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\n';

	ImGui::InputText("Input Object Type", c, 21);

	inputObjectType = c;

	if (Input::KeyTrigger(DIK_RETURN)) inpttingObjectType = false;
}

std::string MelLib::SceneEditer::GetObjectType(const GameObject& object)const
{
	std::string objectType = typeid(object).name();

	for (const auto& pObject : pRegisterObjects) 
	{
		if (pObject.first == objectType)return pObject.first;
	}
	return objectType;
}

MelLib::SceneEditer* MelLib::SceneEditer::GetInstance()
{
	static SceneEditer s;
	return &s;
}

void MelLib::SceneEditer::RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject , const std::string& objectType)
{
	if (!isEdit)return;
	
	// C++20のcontainsに置き換えできる
	if (pRegisterObjects.find(objectType) != pRegisterObjects.end()) 
	{
		std::string text = "シーンエディタには既に"+ objectType + "という名前のオブジェクトが登録されています。\n";
		OutputDebugStringA(text.c_str());
		return;
	}

	pRegisterObjects.try_emplace(objectType, std::map<std::string,std::shared_ptr<MelLib::GameObject>>());
	pRegisterObjects[objectType].emplace(pObject->GetObjectName(),pObject);
	//test.push_back(pObject);
	//registerObjectOrderDatas.try_emplace(pRegisterObjects.size() - 1, objectType);

	registerObjectNames.clear();
	registerObjectNames.reserve(pRegisterObjects.size());

	registerObjectTypes.clear();
	registerObjectOrderDatas.clear();

	for (const auto& m : pRegisterObjects) 
	{
		registerObjectTypes.push_back(m.first);
		int i = 0;
		for (const auto& object : m.second)
		{
			registerObjectNames.push_back(object.first);
			registerObjectOrderDatas.emplace(i, object.first);

			i++;
		}
	}
}

void MelLib::SceneEditer::Update()
{
	if (!isEdit)return;
	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

#pragma region 選択


	ImguiManager::GetInstance()->BeginDrawWindow("Edit");

	// 登録ボタン追加
	bool push = false;
	push = ImguiManager::GetInstance()->DrawButton("RegisterSelectObject");
	//if (push)RegisterSelectObject();
	if (push && !inpttingObjectName && !inpttingObjectType)
	{
		inpttingObjectName = true;
		inputObjectName = "Object";
		inputObjectType = "Object";
	}

	if (inpttingObjectType)
	{
		InputObjectType();
		if (!inpttingObjectType)
		{
			inpttingObjectType = false;
			RegisterSelectObject();
		}
	}
	if (inpttingObjectName)
	{
		InputObjectName();
		if (!inpttingObjectName)
		{
			inpttingObjectName = false;
			inpttingObjectType = true;
		}
	}


	// キーの数だけラジオボタン描画
	int count = 0;
	for (const auto& object : pRegisterObjects)
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}

	// ラジオボタンで選択したオブジェクト一覧の参照
	std::map <std::string, std::shared_ptr<MelLib::GameObject>>& refObjects = pRegisterObjects[registerObjectTypes[selectType]];
	

	// リスト処理
	int listNum = 0;
	ImguiManager::GetInstance()->DrawList(listNum, registerObjectNames);
	
	//// ここにスライダー作成処理
	//int sliderNum = 0;
	//ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	//// 選ばれたオブジェクトのポインタをpSelectObjectに代入
	pEditSelectObject = refObjects[registerObjectOrderDatas[listNum]].get();

	

#pragma endregion

	UpdateSelectObject();
	ImguiManager::GetInstance()->EndDrawWindow();

	// 追加
	if (Input::KeyTrigger(DIK_SPACE)) 
	{
		// GetNewPtr関数でメモリ確保
		std::shared_ptr<GameObject>pObject = pEditSelectObject->GetNewPtr();

		if (pObject) 
		{
			// 追加時に座標とかセットし直すのめんどいから
			// pSelectObjectを管理クラスに追加して
			// 戻り値のを新たにmapに入れてもいいかも
			// どっちみちセットしないといけないから
			pObject->SetPosition(pEditSelectObject->GetPosition());
			pObject->SetAngle(pEditSelectObject->GetAngle());
			pObject->SetScale(pEditSelectObject->GetScale());

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

	bool pushControl = Input::KeyState(DIK_LCONTROL) || Input::KeyState(DIK_RCONTROL);
	// 保存
	if (pushControl  && Input::KeyTrigger(DIK_S))Save();
	// 読込
	if (pushControl && Input::KeyTrigger(DIK_L))Load();

	DrawObjectList();

}

void MelLib::SceneEditer::Draw()
{
	if (!isEdit)return;
	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

	pEditSelectObject->Draw();

}

void MelLib::SceneEditer::SetEditFlag(const bool flag)
{
	// Debug時はリリースビルドのフラグ確認無視
#ifdef _DEBUG

#else
	if (!releaseEdit) return;

#endif // _DEBUG

	isEdit = flag;
}
