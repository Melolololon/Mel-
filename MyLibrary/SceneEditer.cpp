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
	// �I�u�W�F�N�g�}�l�[�W���[�ɒǉ������I�u�W�F�N�g��imgui�ł������悤��
	

	//std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	//name += EDIT_DATA_FORMAT;
	//std::ofstream file(name);

	//// �����o��


	//file.close();
}

void MelLib::SceneEditer::SaveEditData()
{
	std::ofstream file(inputEditDataName + EDIT_DATA_FORMAT, std::ios_base::binary);

	// �����o��
	for (const auto& pObject : addObjects) 
	{
		// �N���X��
		const std::string CLASS_NAME = typeid(*pObject).name();
		file.write(CLASS_NAME.c_str(), CLASS_NAME.size());
		file.write("\0", 1);

		// �I�u�W�F�N�g��
		const std::string OBJECT_NAME = pObject->GetObjectName();
		file.write(OBJECT_NAME.c_str(), OBJECT_NAME.size());
		file.write("\0", 1);



		//���W�Ƃ�
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
	// �����o������
	// �I�u�W�F�N�g��(�t�@�C����)
	// �N���X��
	// �p�����[�^�[


	for (const auto& o : pRegisterObjects) 
	{
		for (const auto& o2 : o.second) 
		{
			GameObject* pObject = o2.second.get();
			
			const std::string OBJECT_NAME = pObject->GetObjectName();

			// �I�������I�u�W�F�N�g����Ȃ�������ۑ����Ȃ�
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

			// ���W�͏����o���K�v�Ȃ�
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

	// �I�u�W�F�N�g�̍폜
	addObjects.clear();
	GameObjectManager::GetInstance()->AllEraseObject();

	// �ǂݍ���
	while(1)
	{
		// �N���X��
		std::string className;
		LoadFileName(file, className);

		// �I�u�W�F�N�g��
		std::string objectName;
		LoadFileName(file, objectName);

		// �����N���X��T���Amake_shared��Ԃ��Ă��炤
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
		// �Ǘ��N���X�ɒǉ�
		GameObjectManager::GetInstance()->AddObject(pObject);


		//���W�Ƃ�
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
		
		// ���W�Ƃ��ǂݍ���
		file.read(reinterpret_cast<char*>(&data.angle), sizeof(Vector3));
		file.read(reinterpret_cast<char*>(&data.scale), sizeof(Vector3));


		//// �f�[�^��ǉ�
		loadSelectRegisterObjectDatas.push_back(data);

		// ��Unullptr����Č�Ŋm��
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

	// �ۑ������ɓǂݍ���ł���������

	// �����Ɏw�肵���V�[����������
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
	// �I�u�W�F�N�g�}�l�[�W���[�ɖ��O�����擾����֐�����Ă���������
	// ����string�̔z��ɓ����Ə����x���Ȃ邩��I�u�W�F�N�g�ǉ����ɖ��O�ǉ����Ă�������
	// ���O�ύX�������ɕύX���鏈������Ȃ��Ƃ���o���Ȃ�
	// �Q�[�����ɂ͎g��Ȃ����낤���炠����x�x���Ă����Ċ����͂���
	// umap�ɂ��ăL�[��GameObject�ɂ���΃A�N�Z�X�y����
	// 
	// �������O�̃I�u�W�F�N�g���������疼�O�ɔԍ��t������
	// OBJECT_NAME��萔����ϐ��ɕς��Ȃ��Ƃ����Ȃ�
	// �ϐ��ɂ����炠�Ƃ��疼�O�ς�����ł��邩��ϐ��ɂ�������Ă���
	

	// �����\�[�g���Ă���������
	// ����const�ɂ�����
	// GameObjectManager�Ŗ��O�̔z�����ĊǗ��o����悤�ɂ��Ă���const�ɕύX����
	std::vector<std::string>objectNames;
	GameObjectManager::GetInstance()->GetObjectNames(objectNames);
	
	// ImguiManager��vector���ă��X�g�\���ł���悤�ɂ��邩��AGetObjectName��char�̔z��n���Ď󂯎�鏈�����K�v�Ȃ�
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
	// �ǂݍ��ނƂ��́AGetNewPtr�֐����g����New����΂���
	// �������������o���Ȃ�?
	// �N���X�����������o���āARegisterObject�œo�^�����I�u�W�F�N�g��GetNewPtr���Ăяo����OK



	// �R�s�[�𐶐�
	std::shared_ptr<GameObject> object = pEditSelectObject->GetNewPtr();
	pEditSelectObject->CopyObjectData(*object, GameObject::CopyGameObjectContent::EDIT);

	// ���͂��ꂽ���O��ݒ�
	object->SetObjectName(inputObjectName);

	// ���O�̓o�^
	registerSelectObjectNames.push_back(inputObjectName);

	// �o�^
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
		// ���O�����ɓo�^����Ă��邩�`�F�b�N
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
	// C++20��contains�ɒu�������ł���
	if (pRegisterObjects[objectType].find(OBJECT_NAME) != pRegisterObjects[objectType].end())
	{
		std::string text = "�V�[���G�f�B�^�ɂ͊���"+ OBJECT_NAME + "�Ƃ������O�̃I�u�W�F�N�g���o�^����Ă��܂��B\0";
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
		// ��v���Ȃ��܂��͊��Ƀ��������m�ۂ��Ă����玟��
		if (data.className != typeid(*pObject).name() 
			|| pRegisterObjects.at(data.typeName).at(data.objectName))continue;

		// �Q�Ƃ���
		std::shared_ptr<GameObject>&pRefObject = pRegisterObjects[data.typeName][data.objectName];

		// �������m�ۂ��Ċi�[
		pRefObject = pObject->GetNewPtr();

		// �f�[�^���Z�b�g
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


#pragma region �I��


	ImguiManager::GetInstance()->BeginDrawWindow("Edit");

	// �o�^�{�^���ǉ�
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


	// �L�[�̐��������W�I�{�^���`��
	int count = 0;
	for (const auto& object : pRegisterObjects)
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}


	// ���W�I�{�^���őI�������I�u�W�F�N�g�ꗗ�̎Q��
	std::map <std::string, std::shared_ptr<MelLib::GameObject>>& refObjects = pRegisterObjects[registerObjectTypes[selectType]];


	// ���X�g����
	ImguiManager::GetInstance()->DrawList(registerObjectListNum, registerObjectNames[registerObjectTypes[selectType]]);

	//// �����ɃX���C�_�[�쐬����
	//int sliderNum = 0;
	//ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	//// �I�΂ꂽ�I�u�W�F�N�g�̃|�C���^��pSelectObject�ɑ��
	const std::string OBJECT_NAME = registerObjectOrderDatas[registerObjectTypes[selectType]][registerObjectListNum];

	pEditSelectObject = refObjects[OBJECT_NAME].get();



#pragma endregion

	UpdateSelectObject();

	// �ǉ�
	if (Input::KeyTrigger(DIK_SPACE))
	{
		// GetNewPtr�֐��Ń������m��
		std::shared_ptr<GameObject>pObject = pEditSelectObject->GetNewPtr();

		if (pObject)
		{
			// �ǉ����ɍ��W�Ƃ��Z�b�g�������̂߂�ǂ�����
			// pSelectObject���Ǘ��N���X�ɒǉ�����
			// �߂�l�̂�V����map�ɓ���Ă���������
			// �ǂ����݂��Z�b�g���Ȃ��Ƃ����Ȃ�����
			pObject->SetPosition(pEditSelectObject->GetPosition());
			pObject->SetAngle(pEditSelectObject->GetAngle());
			pObject->SetScale(pEditSelectObject->GetScale());

			// �ǉ�
			GameObjectManager::GetInstance()->AddObject(pObject);
			addObjects.push_back(pObject.get());
		}
		else
		{
			std::string text = typeid(*pObject).name();
			text += "��GetNewPtr��nullptr��Ԃ��܂����B�I�u�W�F�N�g���ǉ��ł��܂���B\0";
			OutputDebugStringA(text.c_str());
		}
	}

	bool pushControl = Input::KeyState(DIK_LCONTROL) || Input::KeyState(DIK_RCONTROL);
	// �ۑ�
	if (pushControl && Input::KeyTrigger(DIK_S) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName)StartSave();
	if (inpttingEditDataName)InputEditDataName();
	// �Ǎ�
	if (pushControl && Input::KeyTrigger(DIK_L) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName)Load();

	DrawObjectList();

	// �V�[���̍X�V�I���I�t����
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
	// Debug���̓����[�X�r���h�̃t���O�m�F����
#ifdef _DEBUG

#else
	if (!releaseEdit)
	{
		return;
	}
#endif // _DEBUG

	isEdit = flag;
}
