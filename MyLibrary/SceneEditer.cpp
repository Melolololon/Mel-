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
	// �I�u�W�F�N�g�}�l�[�W���[�ɒǉ������I�u�W�F�N�g��imgui�ł������悤��
	

	std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	name += EDIT_DATA_FORMAT;
	std::ofstream file(name);

	// �����o��


	file.close();
}

void MelLib::SceneEditer::Load()
{

	// �ۑ������ɓǂݍ���ł���������

	// �����Ɏw�肵���V�[����������
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

void MelLib::SceneEditer::SetDrawWindowFlag(const std::vector<std::string>& objectNames)
{
	for (const auto& name : objectNames) 
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

	// �o�^
	RegisterObject(object, GetObjectType(*object));
}

std::string MelLib::SceneEditer::GetObjectType(const GameObject& object)const
{
	std::string objectType = typeid(object).name();

	for (const auto& pObject : pObjects) 
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
	
	// C++20��contains�ɒu�������ł���
	if (pObjects.find(objectType) != pObjects.end()) 
	{
		std::string text = "�V�[���G�f�B�^�ɂ͊���"+ objectType + "�Ƃ������O�̃I�u�W�F�N�g���o�^����Ă��܂��B\n";
		OutputDebugStringA(text.c_str());
		return;
	}

	pObjects.try_emplace(objectType, std::vector<std::shared_ptr<MelLib::GameObject>>());
	pObjects[objectType].push_back(pObject);
	//test.push_back(pObject);
	objectOrderDatas.try_emplace(pObjects.size() - 1, objectType);

}

void MelLib::SceneEditer::Update()
{
	if (!isEdit)return;
	if (pObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

#pragma region �I��


	ImguiManager::GetInstance()->BeginDrawWindow("Edit");

	// �o�^�{�^���ǉ�
	bool push = false;
	push = ImguiManager::GetInstance()->DrawButton("RegisterSelectObject");
	if (push)RegisterSelectObject();

	// �L�[�̐��������W�I�{�^���`��
	int count = 0;
	for (const auto& object : pObjects)
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}

	// ���W�I�{�^���őI�������I�u�W�F�N�g�ꗗ�̎Q��
	std::vector < std::shared_ptr<MelLib::GameObject>>& refObjects = pObjects[objectOrderDatas[selectType]];

	// �����ɃX���C�_�[�쐬����
	int sliderNum = 0;
	ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	// �I�΂ꂽ�I�u�W�F�N�g�̃|�C���^��pSelectObject�ɑ��
	pEditSelectObject = refObjects[sliderNum].get();

	

#pragma endregion

	UpdateSelectObject();
	ImguiManager::GetInstance()->EndDrawWindow();

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
		}
		else 
		{
			std::string text = typeid(*pObject).name();
			text += "��GetNewPtr��nullptr��Ԃ��܂����B�I�u�W�F�N�g���ǉ��ł��܂���B\n";
			OutputDebugStringA(text.c_str());
		}
	}

	bool pushControl = Input::KeyState(DIK_LCONTROL) || Input::KeyState(DIK_RCONTROL);
	// �ۑ�
	if (pushControl  && Input::KeyTrigger(DIK_S))Save();
	// �Ǎ�
	if (pushControl && Input::KeyTrigger(DIK_L))Load();

	DrawObjectList();

}

void MelLib::SceneEditer::Draw()
{
	if (!isEdit)return;
	if (pObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

	pEditSelectObject->Draw();

}

void MelLib::SceneEditer::SetEditFlag(const bool flag)
{
	// Debug���̓����[�X�r���h�̃t���O�m�F����
#ifdef _DEBUG

#else
	if (!releaseEdit) return;

#endif // _DEBUG

	isEdit = flag;
}
