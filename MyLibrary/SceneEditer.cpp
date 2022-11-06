#include "SceneEditer.h"

#include"ImguiManager.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"GuiValueManager.h"
#include"Input.h"

#include<fstream>
#include<filesystem>

const std::string MelLib::SceneEditer::EDIT_DATA_FORMAT = ".melsce";
const std::string MelLib::SceneEditer::REGISTER_OBJECT_DATA_FORMAT = ".rod";

void MelLib::SceneEditer::StartSave()
{

	SaveRegisterObject();
	inpttingEditDataName = true;
	// 2022_05_01
	// オブジェクトマネージャーに追加したオブジェクトをimguiでいじれるように
	

	//std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	//name += EDIT_DATA_FORMAT;
	//std::ofstream file(name);

	//// 書き出し


	//file.close();
}

void MelLib::SceneEditer::SaveEditData()
{
	std::ofstream file(inputEditDataName + EDIT_DATA_FORMAT, std::ios_base::binary);

	// 書き出し
	for (const auto& pObject : addObjects) 
	{
		// クラス名
		const std::string CLASS_NAME = typeid(*pObject).name();
		file.write(CLASS_NAME.c_str(), CLASS_NAME.size());
		file.write("\0", 1);

		// オブジェクト名
		const std::string OBJECT_NAME = pObject->GetObjectName();
		file.write(OBJECT_NAME.c_str(), OBJECT_NAME.size());
		file.write("\0", 1);



		//座標とか
		Vector3 position = pObject->GetPosition();
		file.write(reinterpret_cast<char*>(&position), sizeof(Vector3));
		Vector3 angle = pObject->GetAngle();
		file.write(reinterpret_cast<char*>(&angle), sizeof(Vector3));
		Vector3 scale = pObject->GetScale();
		file.write(reinterpret_cast<char*>(&scale), sizeof(Vector3));

		file.write("-1", 1);
	}

	file.write("-2", 1);

	file.close();
}

void MelLib::SceneEditer::InputEditDataName()
{
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Name", inputObjectName, 20);
	std::string s = "EditData";
	char c[21];
	for (int i = 0; i < 21; i++)
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\0';

	ImGui::InputText("Input Edit Data Name", c, 21);

	inputEditDataName = c;

	if (Input::KeyTrigger(DIK_RETURN))
	{
		inpttingEditDataName = false;
		SaveEditData();
	}
}

void MelLib::SceneEditer::SaveRegisterObject()
{
	// 書き出すもの
	// オブジェクト名(ファイル名)
	// クラス名
	// パラメーター


	for (const auto& o : pRegisterObjects) 
	{
		for (const auto& o2 : o.second) 
		{
			GameObject* pObject = o2.second.get();
			
			const std::string OBJECT_NAME = pObject->GetObjectName();

			// 選択したオブジェクトじゃなかったら保存しない
			bool selectObject = false;
			for (const auto& name : registerSelectObjectNames) 
			{
				if (name == OBJECT_NAME) 
				{
					selectObject = true;
					break;
				}
			}
			if (!selectObject)break;


			std::ofstream registerObjectFile(OBJECT_NAME + REGISTER_OBJECT_DATA_FORMAT,std::ios_base::binary);
			
			registerObjectFile.write(o.first.c_str() , o.first.size());
			registerObjectFile.write("\0" , 1);

			std::string className = typeid(*pObject).name();
			registerObjectFile.write(className.c_str(), className.size());
			registerObjectFile.write("\0", 1);

			// 座標は書き出す必要なし
			Vector3 angle = pObject->GetAngle();
			registerObjectFile.write(reinterpret_cast<char*>(&angle), sizeof(Vector3));
			Vector3 scale = pObject->GetScale();
			registerObjectFile.write(reinterpret_cast<char*>(&scale), sizeof(Vector3));

			registerObjectFile.close();
		}
	}

}

void MelLib::SceneEditer::Load()
{
	
}

void MelLib::SceneEditer::LoadEditData()
{
	std::ifstream file(selectEditFileName + EDIT_DATA_FORMAT, std::ios_base::binary);

	// オブジェクトの削除
	addObjects.clear();
	GameObjectManager::GetInstance()->AllEraseObject();

	// 読み込み
	while(1)
	{
		// クラス名
		std::string className;
		LoadFileName(file, className);

		// オブジェクト名
		std::string objectName;
		LoadFileName(file, objectName);

		// 同じクラスを探し、make_sharedを返してもらう
		std::shared_ptr<GameObject> pObject;
		for (const auto& m : pRegisterObjects) 
		{
			for (const auto& pRegisterObject : m.second) 
			{
				GameObject* p = pRegisterObject.second.get();

				if (typeid(*pObject) == typeid(*p)) 
				{
					pObject = p->GetNewPtr();
					break;
				}
			}
			if (pObject)break;
		}
		// 管理クラスに追加
		GameObjectManager::GetInstance()->AddObject(pObject);


		//座標とか
		Vector3 position;
		file.read(reinterpret_cast<char*>(&position), sizeof(Vector3));
		pObject->SetPosition(position);
		Vector3 angle;
		file.read(reinterpret_cast<char*>(&angle), sizeof(Vector3));
		pObject->SetAngle(angle);
		Vector3 scale;
		file.read(reinterpret_cast<char*>(&scale), sizeof(Vector3));
		pObject->SetScale(scale);

		char c;
		file.read(&c, 1);
		if (c == -2)break;
	}


	file.close();
}

void MelLib::SceneEditer::LoadRegisterSelectObject()
{
	
	for (const auto& dirEntry : std::filesystem::directory_iterator("."))
	{
		const std::string FILE_NAME = dirEntry.path().string();
		if (FILE_NAME.find(REGISTER_OBJECT_DATA_FORMAT) == std::string::npos)continue;

		std::ifstream file(FILE_NAME, std::ios_base::binary);

		ObjectData data;
		data.objectName = FILE_NAME;
		data.objectName.erase(data.objectName.begin(), data.objectName.begin() + 2);
		data.objectName.erase(data.objectName.end() - 4, data.objectName.end());

		//for(size_t i = objectName.size() - 1 ;;)

		LoadFileName(file, data.typeName);
		LoadFileName(file, data.className);
		
		// 座標とか読み込み
		file.read(reinterpret_cast<char*>(&data.angle), sizeof(Vector3));
		file.read(reinterpret_cast<char*>(&data.scale), sizeof(Vector3));


		//// データを追加
		loadSelectRegisterObjectDatas.push_back(data);

		// 一旦nullptr入れて後で確保
		pRegisterObjects[data.typeName].emplace(data.objectName, nullptr);


		file.close();
	}

}

void MelLib::SceneEditer::LoadFileName(std::ifstream& stream, std::string& str)
{
	while (1)
	{
		char c;
		stream.read(&c, 1);
		if (c == '\0')break;
		str += c;
	}
}

void MelLib::SceneEditer::SelectEditData()
{
}

void MelLib::SceneEditer::Load(const std::string& sceneName)
{

	// 保存せずに読み込んでいいか聞く

	// ここに指定したシーン名を入れる
	std::string name = sceneName;
	name += EDIT_DATA_FORMAT;
	std::ifstream file;
	file.open(name, std::ios_base::binary);

	


	file.close();

	

}

void MelLib::SceneEditer::UpdateSelectObject()
{
	if (!isEdit)return;
	

#ifdef _DEBUG

#else
	if (!releaseEdit)return;
#endif // _DEBUG


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



	// コピーを生成
	std::shared_ptr<GameObject> object = pEditSelectObject->GetNewPtr();
	pEditSelectObject->CopyObjectData(*object, GameObject::CopyGameObjectContent::EDIT);

	// 入力された名前を設定
	object->SetObjectName(inputObjectName);

	// 名前の登録
	registerSelectObjectNames.push_back(inputObjectName);

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
	c[20] = '\0';
	
	ImGui::InputText("Input Object Name",c, 21);

	inputObjectName = c;

	if (Input::KeyTrigger(DIK_RETURN))
	{
		// 名前が既に登録されているかチェック
		for (const auto& m : pRegisterObjects) 
		{
			if (m.second.find(inputObjectName) == m.second.end())inpttingObjectName = false;
		}

	}
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
	c[20] = '\0';

	ImGui::InputText("Input Object Type", c, 21);

	inputObjectType = c;

	if (Input::KeyTrigger(DIK_RETURN))
	{
		inpttingObjectType = false;
	}
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
	

#ifdef _DEBUG

#else
	if (!releaseEdit)return;
#endif // _DEBUG

	const std::string OBJECT_NAME = pObject->GetObjectName();
	// C++20のcontainsに置き換えできる
	if (pRegisterObjects[objectType].find(OBJECT_NAME) != pRegisterObjects[objectType].end())
	{
		std::string text = "シーンエディタには既に"+ OBJECT_NAME + "という名前のオブジェクトが登録されています。\0";
		OutputDebugStringA(text.c_str());
		return;
	}

	pRegisterObjects.try_emplace(objectType, std::map<std::string,std::shared_ptr<MelLib::GameObject>>());
	pRegisterObjects[objectType].emplace(OBJECT_NAME,pObject);
	//test.push_back(pObject);
	//registerObjectOrderDatas.try_emplace(pRegisterObjects.size() - 1, objectType);

	registerObjectNames.clear();
	
	registerObjectTypes.clear();
	registerObjectOrderDatas.clear();

	for (const auto& m : pRegisterObjects) 
	{
		registerObjectTypes.push_back(m.first);
		int i = 0;

		registerObjectOrderDatas.emplace(m.first, std::unordered_map<int, std::string>());
		registerObjectNames.emplace(m.first, std::vector<std::string>());

		for (const auto& object : m.second)
		{
			registerObjectOrderDatas[m.first].emplace(i, object.first);
			registerObjectNames[m.first].push_back(object.first);

			i++;
		}
	}


	for (const auto& data : loadSelectRegisterObjectDatas) 
	{
		// 一致しないまたは既にメモリを確保していたら次へ
		if (data.className != typeid(*pObject).name() 
			|| pRegisterObjects.at(data.typeName).at(data.objectName))continue;

		// 参照する
		std::shared_ptr<GameObject>&pRefObject = pRegisterObjects[data.typeName][data.objectName];

		// メモリ確保して格納
		pRefObject = pObject->GetNewPtr();

		// データをセット
		pRefObject->SetObjectName(data.objectName);
		pRefObject->SetAngle(data.angle);
		pRefObject->SetScale(data.scale);

	}

}

void MelLib::SceneEditer::Initialize()
{
	LoadRegisterSelectObject();
}

void MelLib::SceneEditer::Update()
{
	if (!isEdit)return;

#ifdef _DEBUG

#else
	if (!releaseEdit)return;
#endif // _DEBUG


	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;


#pragma region 選択


	ImguiManager::GetInstance()->BeginDrawWindow("Edit");

	// 登録ボタン追加
	bool push = false;
	push = ImguiManager::GetInstance()->DrawButton("RegisterSelectObject");
	//if (push)RegisterSelectObject();
	if (push && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName)
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
	ImguiManager::GetInstance()->DrawList(registerObjectListNum, registerObjectNames[registerObjectTypes[selectType]]);

	//// ここにスライダー作成処理
	//int sliderNum = 0;
	//ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	//// 選ばれたオブジェクトのポインタをpSelectObjectに代入
	const std::string OBJECT_NAME = registerObjectOrderDatas[registerObjectTypes[selectType]][registerObjectListNum];

	pEditSelectObject = refObjects[OBJECT_NAME].get();



#pragma endregion

	UpdateSelectObject();

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
			addObjects.push_back(pObject.get());
		}
		else
		{
			std::string text = typeid(*pObject).name();
			text += "のGetNewPtrがnullptrを返しました。オブジェクトが追加できません。\0";
			OutputDebugStringA(text.c_str());
		}
	}

	bool pushControl = Input::KeyState(DIK_LCONTROL) || Input::KeyState(DIK_RCONTROL);
	// 保存
	if (pushControl && Input::KeyTrigger(DIK_S) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName)StartSave();
	if (inpttingEditDataName)InputEditDataName();
	// 読込
	if (pushControl && Input::KeyTrigger(DIK_L) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName)Load();

	DrawObjectList();

	// シーンの更新オンオフ処理
	if (Input::KeyTrigger(DIK_F5))
	{
		isEdit = !isEdit;
		SceneManager::GetInstance()->ChangeStopFlag();
	}



	ImguiManager::GetInstance()->EndDrawWindow();
}

void MelLib::SceneEditer::Draw()
{

	if (!isEdit)return;

#ifdef _DEBUG

#else
	if (!releaseEdit)return;
#endif // _DEBUG

	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

	pEditSelectObject->Draw();

}

void MelLib::SceneEditer::SetEditFlag(const bool flag)
{
	// Debug時はリリースビルドのフラグ確認無視
#ifdef _DEBUG

#else
	if (!releaseEdit)
	{
		return;
	}
#endif // _DEBUG

	isEdit = flag;
}
