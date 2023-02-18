#include "SceneEditer.h"

#include"ImguiManager.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"GuiValueManager.h"
#include"Input.h"

#include<fstream>
#include<filesystem>

const std::string MelLib::SceneEditer::EDIT_DATA_FORMAT = ".melsce";
const std::string MelLib::SceneEditer::TEST_START_EDIT_DATA_NAME = "TestStartEditData";
const std::string MelLib::SceneEditer::REGISTER_OBJECT_DATA_FORMAT = ".rod";
const std::string MelLib::SceneEditer::EDIT_WINDOW_NAME = "Edit";
const std::string MelLib::SceneEditer::CAMERA_WINDOW_NAME = "EditCamera";

void MelLib::SceneEditer::StartSave()
{

	if (!editorFlag || !ReleaseCheck())return;


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

void MelLib::SceneEditer::SaveEditData(const std::string& dataName)
{
	if (!editorFlag || !ReleaseCheck())return;
	std::ofstream file(dataName + EDIT_DATA_FORMAT, std::ios_base::binary);

	const size_t ADD_OBJECT_SIZE = addObjects.size();
	if(ADD_OBJECT_SIZE == 0)file.write("0", 1);
	else file.write("1", 1);

	// 書き出し
	for (size_t i = 0; i < ADD_OBJECT_SIZE ; i++)
	{
		GameObject* pObject = addObjects[i].get();

		// クラス名
		const std::string CLASS_NAME = typeid(*pObject).name();
		file.write(CLASS_NAME.c_str(), CLASS_NAME.size());
		file.write("\0", 1);

		// オブジェクト名
		const std::string OBJECT_NAME = pObject->GetObjectName();
		file.write(OBJECT_NAME.c_str(), OBJECT_NAME.size());
		file.write("\0", 1);

		// 登録名
		file.write(addObjectNames[i].c_str(), addObjectNames[i].size());
		file.write("\0", 1);
		

		//座標とか
		Vector3 position = pObject->GetPosition();
		file.write(reinterpret_cast<char*>(&position), sizeof(Vector3));
		Vector3 angle = pObject->GetAngle();
		file.write(reinterpret_cast<char*>(&angle), sizeof(Vector3));
		Vector3 scale = pObject->GetScale();
		file.write(reinterpret_cast<char*>(&scale), sizeof(Vector3));

		char c = -1;
		if (i == ADD_OBJECT_SIZE - 1)c = -2;
		file.write(&c, 1);
	}

	file.close();
}

void MelLib::SceneEditer::InputEditDataName()
{
	if (!editorFlag || !ReleaseCheck())return;
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Name", inputObjectName, 20);
	std::string s = "EditData";
	char c[21];
	for (int i = 0; i < 21; i++)
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\0';


	ImGui::Text("Input Edit Data Name");
	ImGui::InputText("", c, 21);



	if (Input::KeyTrigger(DIK_RETURN))
	{
		std::string inputEditDataName = c;
		inpttingEditDataName = false;
		SaveEditData(inputEditDataName);
	}
}

void MelLib::SceneEditer::SaveRegisterObject()
{
	// 書き出すもの
	// オブジェクト名(ファイル名)
	// クラス名
	// パラメーター
	if (!editorFlag || !ReleaseCheck())return;

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
	if (!editorFlag || !ReleaseCheck())return;
	// シーン名を格納
	for (const auto& dirEntry : std::filesystem::directory_iterator("."))
	{
		std::string name = dirEntry.path().string();
		if (name.find(EDIT_DATA_FORMAT) == std::string::npos)continue;
		name.erase(name.begin(), name.begin() + 2);
		name.erase(name.end() - EDIT_DATA_FORMAT.size(), name.end());
		
		if (name == TEST_START_EDIT_DATA_NAME)continue;
		sceneFileNames.push_back(name);
	}

	if (sceneFileNames.size() == 0)return;

	selectingEditData = true;
}


void MelLib::SceneEditer::LoadRegisterSelectObject()
{

	if (!editorFlag || !ReleaseCheck())return;
	for (const auto& dirEntry : std::filesystem::directory_iterator("."))
	{
		const std::string FILE_NAME = dirEntry.path().string();
		if (FILE_NAME.find(REGISTER_OBJECT_DATA_FORMAT) == std::string::npos)continue;

		std::ifstream file(FILE_NAME, std::ios_base::binary);

		ObjectData data;
		data.objectName = FILE_NAME;
		data.objectName.erase(data.objectName.begin(), data.objectName.begin() + 2);
		data.objectName.erase(data.objectName.end() - REGISTER_OBJECT_DATA_FORMAT.size(), data.objectName.end());

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
	if (!editorFlag || !ReleaseCheck())return;
	ImguiManager::GetInstance()->BeginDrawWindow("SelectScene");
	ImguiManager::GetInstance()->DrawList(selectEditDataNum, sceneFileNames);
	ImguiManager::GetInstance()->EndDrawWindow();

	if (Input::KeyTrigger(DIK_RETURN))
	{
		// 読み込み
		LoadEditData(sceneFileNames[selectEditDataNum]);

		sceneFileNames.clear();
		selectingEditData = false;


	}
}

void MelLib::SceneEditer::LoadEditData(const std::string& sceneName)
{
	std::ifstream file(sceneName + EDIT_DATA_FORMAT, std::ios_base::binary);

	// オブジェクトの削除
	addObjects.clear();
	GameObjectManager::GetInstance()->AllEraseObject();

	// オブジェクトがあるか確認
	char c = 0;
	file.read(&c,1);

	// 無かったら閉じて終了
	if (c == '0')
	{
		file.close();
		return;
	}

	// 読み込み
	while (1)
	{
		// クラス名
		std::string className;
		LoadFileName(file, className);

		// オブジェクト名
		std::string objectName;
		LoadFileName(file, objectName);
		addObjectNames.push_back(objectName);

		// 登録名
		std::string regName;
		LoadFileName(file, regName);

		// 同じクラス、同じオブジェクト名を探し、make_sharedを返してもらう
		std::shared_ptr<GameObject> pObject;
		for (const auto& m : pRegisterObjects)
		{
			for (const auto& pRegisterObject : m.second)
			{
				GameObject* p = pRegisterObject.second.get();

				if (regName == p->GetObjectName())
				{
					pObject = p->GetNewPtr();
					break;
				}
			}
			if (pObject)break;
		}

		// 管理クラスに追加
		GameObjectManager::GetInstance()->AddObject(pObject);
		// 追加オブジェクト一覧に追加
		addObjects.push_back(pObject);

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

		pObject->SetPreData();
		pObject->SetGUIData();
		pObject->SetPreDataPositions();

		char c;
		file.read(&c, 1);
		if (c == -2)break;
	}

	file.close();
}

void MelLib::SceneEditer::UpdateSelectObject()
{
	if (!editorFlag || !ReleaseCheck())return;
	if (!isEdit)return;
	

#ifdef _DEBUG

#else
	if (!releaseEdit)return;
#endif // _DEBUG


	/*MelLib::Vector3 pos = pEditSelectObject->GetPosition();
	ImguiManager::GetInstance()->DrawSliderVector3("Position", pos, -1000, 1000);
	pEditSelectObject->SetPosition(pos);

	MelLib::Vector3 angle = pEditSelectObject->GetAngle();
	ImguiManager::GetInstance()->DrawSliderVector3("Angle", angle, 0, 359);
	pEditSelectObject->SetAngle(angle);

	MelLib::Vector3 scale = pEditSelectObject->GetScale();
	ImguiManager::GetInstance()->DrawSliderVector3("Scale", scale, 0, 359);
	pEditSelectObject->SetScale(scale);*/
}

void MelLib::SceneEditer::UpdateCamera()
{
	if (!editorFlag || !ReleaseCheck())return;
	// エディター用のカメラを作る
	// メインカメラ使うと設定とか変数の切替がいちいちめんどくさそう
	// あと変数減る



	/*ImguiManager::GetInstance()->BeginDrawWindow(CAMERA_WINDOW_NAME);

	Vector3 cameraPosition = pEditerCamera->GetCameraPosition();
	ImguiManager::GetInstance()->DrawSliderVector3("CameraPosition", cameraPosition, -1000, 1000);

	Vector3 cameraAngle = pEditerCamera->GetAngle();
	ImguiManager::GetInstance()->DrawSliderVector3("CameraAngle", cameraAngle, -359, 359);

	ImguiManager::GetInstance()->EndDrawWindow();

	pEditerCamera->SetRotateCriteriaPosition(cameraPosition);
	pEditerCamera->SetAngle(cameraAngle);*/

}

void MelLib::SceneEditer::DrawObjectList()
{
	if (!editorFlag || !ReleaseCheck())return;
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
	objectNames.reserve(addObjects.size());
	for (auto& object : addObjects) 
	{
		objectNames.push_back(object->GetObjectName());
	}
	
	// ImguiManagerにvector私てリスト表示できるようにするから、GetObjectNameにcharの配列渡して受け取る処理作る必要ない
	const size_t OBJECT_SIZE = objectNames.size();
	
	if (OBJECT_SIZE == 0)return;
	
	ImguiManager::GetInstance()->BeginDrawWindow("ObjectList");
	ImguiManager::GetInstance()->DrawList(selectListObjectNum, objectNames);
	selectListObjectName = objectNames[selectListObjectNum];
	ImguiManager::GetInstance()->EndDrawWindow();

	SetDrawWindowFlag(objectNames);

	for (const auto& p : addObjects)
	{
		if (p->GetObjectName() == selectListObjectName)
		{
			pSelectListObject = p.get();
		}
	}
}

void MelLib::SceneEditer::SetDrawWindowFlag(const std::vector<std::string>& objNames)
{
	if (!editorFlag || !ReleaseCheck())return;
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
	if (!editorFlag || !ReleaseCheck())return;


	// コピーを生成
	std::shared_ptr<GameObject> object = pEditSelectObject->GetNewPtr();
	pEditSelectObject->CopyObjectData(*object, GameObject::CopyGameObjectContent::EDIT);
	
	// ウィンドウ名変更
	GuiValueManager::GetInstance()->ChangeWindowName(object->GetObjectName(), inputObjectName);
	// 入力された名前を設定
	object->SetObjectName(inputObjectName);
	// GUIの値のコピー
	GuiValueManager::GetInstance()->CopyGuiValue(pEditSelectObject->GetObjectName(), object->GetObjectName());


	// 名前の登録
	registerSelectObjectNames.push_back(inputObjectName);


	object->SetPreData();
	// 登録
	RegisterObject(object, inputObjectType);
}

void MelLib::SceneEditer::InputObjectName()
{
	if (!editorFlag || !ReleaseCheck())return;
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Name", inputObjectName, 20);
	std::string s = "Object";
	char c[21];
	for (int i = 0; i < 21; i++) 
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\0';
	
	ImGui::Text("Input Object Name");
	ImGui::InputText("",c, 21);

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
	if (!editorFlag || !ReleaseCheck())return;
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Type", inputObjectType, 20);

	std::string s = "Object";
	char c[21];
	for (int i = 0; i < 21; i++)
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\0';

	ImGui::Text("Input Object Type");
	ImGui::InputText("", c, 21);

	inputObjectType = c;

	if (Input::KeyTrigger(DIK_RETURN))
	{
		inpttingObjectType = false;
	}
}

void MelLib::SceneEditer::OtherCameraGuiDrawFlagFalse()
{
	if (!editorFlag || !ReleaseCheck())return;
	std::vector<std::string>cameraNames;
	Camera::GetCameraNames(cameraNames);

	for (const auto& name : cameraNames) 
	{
		if (name == pEditerCamera->GetCameraName())continue;
		GuiValueManager::GetInstance()->SetDrawWindowFlag(name, false);
	}
}

void MelLib::SceneEditer::Reset()
{
	if (!editorFlag || !ReleaseCheck())return;
}

bool MelLib::SceneEditer::ReleaseCheck()
{

#ifdef _DEBUG

#else
	return !releaseEdit
#endif // _DEBUG

	return true;
}



MelLib::SceneEditer* MelLib::SceneEditer::GetInstance()
{
	static SceneEditer s;
	return &s;
}

void MelLib::SceneEditer::RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject , const std::string& objectType)
{


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

	pObject->SetPreData();

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

		const std::string PRE_NAME = pRefObject->GetObjectName();

		// データをセット
		pRefObject->SetObjectName(data.objectName);
		pRefObject->SetAngle(data.angle);
		pRefObject->SetScale(data.scale);

		GuiValueManager::GetInstance()->ChangeWindowName(PRE_NAME, data.objectName);
		GuiValueManager::GetInstance()->LoadGUIFileData(data.objectName);

		pRefObject->SetPreData();
		
	}
	GuiValueManager::GetInstance()->ChangeWindowName(pObject->GetObjectName(),OBJECT_NAME);
	
}

void MelLib::SceneEditer::Initialize()
{

	if (!editorFlag || !ReleaseCheck())return;
	LoadRegisterSelectObject();

	SaveEditData(TEST_START_EDIT_DATA_NAME);

	Camera::Create(CAMERA_WINDOW_NAME);
	pEditerCamera = Camera::Get(CAMERA_WINDOW_NAME);
	RenderTarget::Get()->SetCamera(pEditerCamera);

	
	OtherCameraGuiDrawFlagFalse();
	isEdit = true;
}

void MelLib::SceneEditer::Update()
{
	if (!editorFlag || !ReleaseCheck())return;


	// シーンの更新オンオフ処理
	if (Input::KeyTrigger(DIK_F5))
	{
		isEdit = !isEdit;

		if (isEdit) 
		{
			// スコアなどをリセットするために読み込みなおす
			SceneManager::GetInstance()->ReLoadScene();
			LoadEditData(TEST_START_EDIT_DATA_NAME);


			RenderTarget::RenderTargetDrawData data;
			data.rt = RenderTarget::Get();
			std::vector<RenderTarget::RenderTargetDrawData>datas;
			datas.push_back(data);

			data.rt->SetCamera(pEditerCamera);
		}
		else 
		{
			SceneManager::GetInstance()->ReLoadScene();
			// オブジェクト全部消したのに追加したら0番とかにならない不具合修正する
			// 前回の番号の状態を実行時に反映させるようにする
			// エディット中は利用者が作ったレンダーターゲットを使用しないようにする

			// 開始時点のデータを書き出す
			SaveEditData(TEST_START_EDIT_DATA_NAME);


			// 切り替えた瞬間のカメラの名前を保存しておき、エディットオフにしたときにそれに切り替えるようにする
			// 切り替えた瞬間のカメラが最初に使われるカメラとは限らないのでよくない
			// 
			// エディターで開始時のカメラをセットできるようにすればよさそう
			
			// スライダーだけじゃなくて直接入力もできるようにした方がよさそう

			RenderTarget::Get()->SetCamera(Camera::Get());

			// オブジェクト追加
			for (auto& object : addObjects) 
			{
				GameObjectManager::GetInstance()->AddObject(object);
			}
			
		}

	}
	if (!isEdit)return;



	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

#pragma region 選択

	ImguiManager::GetInstance()->BeginDrawWindow(EDIT_WINDOW_NAME);

	// スライダーとボックス切り替えられるようにする
	bool pushChangeButton = ImguiManager::GetInstance()->DrawButton("ChangeSlider Box");
	

	// 登録ボタン追加
	bool push = false;
	push = ImguiManager::GetInstance()->DrawButton("RegisterSelectObject");
	//if (push)RegisterSelectObject();
	if (push && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)
	{
		inpttingObjectName = true;
		inputObjectName = "Object";
		inputObjectType = "Object";
	}

	if (inpttingObjectType)
	{
		InputObjectType();
		
		// これ上の関数のエンター押したら入るifの中に書いていいかも
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

	if (registerObjectListNum >= registerObjectOrderDatas[registerObjectTypes[selectType]].size())
	{
		registerObjectListNum = registerObjectOrderDatas[registerObjectTypes[selectType]].size() - 1;
	}

	//// 選ばれたオブジェクトのポインタをpSelectObjectに代入
	const std::string OBJECT_NAME = registerObjectOrderDatas[registerObjectTypes[selectType]][registerObjectListNum];

	pEditSelectObject = refObjects[OBJECT_NAME].get();
	pEditSelectObject->SetGUIData();

	// pEditSelectObjectのウィンドウ描画設定
	for (const auto& p : pRegisterObjects) 
	{
		for (const auto& object : p.second)
		{
			if(object.first == OBJECT_NAME)GuiValueManager::GetInstance()->SetDrawWindowFlag(object.first, true);
			else GuiValueManager::GetInstance()->SetDrawWindowFlag(object.first, false);
		}
	}


	if (pushChangeButton && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)
	{
		typingInputFlag = !typingInputFlag;

		GuiValueManager::GetInstance()->ChangeTypingInputFlag(pEditSelectObject->GetObjectName());
		GuiValueManager::GetInstance()->ChangeTypingInputFlag(CAMERA_WINDOW_NAME);

		if (pSelectListObject)GuiValueManager::GetInstance()->ChangeTypingInputFlag(pSelectListObject->GetObjectName());



	}

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
			/*pObject->SetPosition(pEditSelectObject->GetPosition());
			pObject->SetAngle(pEditSelectObject->GetAngle());
			pObject->SetScale(pEditSelectObject->GetScale());*/

			// コピー
			pEditSelectObject->CopyObjectData(*pObject, GameObject::CopyGameObjectContent::EDIT);

			// GUIのコピー
			// これやるなら上のコピーいらないかも
			GuiValueManager::GetInstance()->CopyGuiValue(pEditSelectObject->GetObjectName(), pObject->GetObjectName());

			// 追加
			//GameObjectManager::GetInstance()->AddObject(pObject);

			// 全て削除
			// (登録したオブジェクトのコンストラクタにオブジェクトを追加する処理がある場合、追加されたオブジェクトが表示されるため)
			GameObjectManager::GetInstance()->AllEraseObject();
			addObjects.push_back(pObject);
			addObjectNames.push_back(pEditSelectObject->GetObjectName());

			// フラグセット
			GuiValueManager::GetInstance()->SetTypingInputFlag(pObject->GetObjectName(),typingInputFlag);
		}
		else
		{
			std::string text;
			text = "オブジェクト追加時にGetNewPtr()がnullptrを返しました。オブジェクトを追加できません。\n";
			OutputDebugStringA(text.c_str());
		}
	}

	ImguiManager::GetInstance()->EndDrawWindow();

	bool pushControl = Input::KeyState(DIK_LCONTROL) || Input::KeyState(DIK_RCONTROL);

	


	// 保存
	if (pushControl && Input::KeyTrigger(DIK_S) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)StartSave();
	if (inpttingEditDataName)InputEditDataName();
	// 読込
	if (pushControl && Input::KeyTrigger(DIK_L) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)Load();
	if (selectingEditData)SelectEditData();

	// オブジェクト一覧の描画
	DrawObjectList();

	// 削除
	if (pushControl && Input::KeyTrigger(DIK_D) && pSelectListObject)
	{
		pSelectListObject->TrueEraseManager();

		const size_t SIZE = addObjects.size();
		for (size_t i = 0; i < SIZE; i++)
		{
			if (pSelectListObject == addObjects[i].get())
			{
				addObjects.erase(addObjects.begin() + i);
				addObjectNames.erase(addObjectNames.begin() + i);
				//GameObjectManager::GetInstance()->EraseObject(pSelectListObject);
				pSelectListObject = nullptr;
			}
		}
	}

	// リセット
	if (Input::KeyTrigger(DIK_ESCAPE)) 
	{
		// 開始時のオブジェクトのデータと途中追加したオブジェクトのデータ分ける
		// それかファイルひらきっぱにしておく
		// 開きっぱなしの方がテストプレイ中にファイル消して止まるとかそういうことなくなるかも
	}

	//// 描画設定
	//for (auto& obj : addObjects) 
	//{
	//	if (pEditSelectObject == obj)pEditSelectObject->SetDrawGUIFlag(true);
	//	else obj->SetDrawGUIFlag(false);
	//}

	pEditSelectObject->SetPreData();


	UpdateCamera();
}

void MelLib::SceneEditer::Draw()
{
	if (!editorFlag || !ReleaseCheck())return;

	if (!isEdit)return;

#ifdef _DEBUG

#else
	if (!releaseEdit)return;
#endif // _DEBUG

	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

	pEditSelectObject->Draw();

	for (auto& addObject : addObjects) 
	{
		addObject->Draw();
	}
}

void MelLib::SceneEditer::SetReleaseEditFlag(const bool flag)
{
	releaseEdit = flag;
	if (!flag)isEdit = false;
}

//
//void MelLib::SceneEditer::SetEditFlag(const bool flag)
//{
//	// Debug時はリリースビルドのフラグ確認無視
//#ifdef _DEBUG
//
//#else
//	if (!releaseEdit)
//	{
//		return;
//	}
//#endif // _DEBUG
//
//	isEdit = flag;
//}
