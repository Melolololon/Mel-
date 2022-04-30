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

	// �����o��


	file.close();
}

MelLib::SceneEditer* MelLib::SceneEditer::GetInstance()
{
	static SceneEditer s;
	return &s;
}

void MelLib::SceneEditer::RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject , const std::string& objectType)
{
	// C++20��contains�ɒu�������ł���
	//if (pObjects.find(objectType) == pObjects.end()) 
	//{
	//	std::string text = "�V�[���G�f�B�^�ɂ͊���"+ objectType + "�̖��O�ő��̃I�u�W�F�N�g���o�^����Ă��܂��B\n";
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
	// �ǉ�
	if (Input::KeyTrigger(DIK_SPACE)) 
	{
		// GetNewPtr�֐��Ń������m��
		std::shared_ptr<GameObject>pObject = pSelectObject->GetNewPtr();

		if (pObject) 
		{
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

}

void MelLib::SceneEditer::Draw()
{
	ImguiManager::GetInstance()->BeginDrawWindow("Edit");


	// �L�[�̐��������W�I�{�^���`��
	int count = 0;
	for (const auto& object : pObjects) 
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}

	// ���W�I�{�^���őI�������I�u�W�F�N�g�ꗗ�̎Q��
	std::vector < std::shared_ptr<MelLib::GameObject>>& refObjects = pObjects[objectOrderDatas[count]];

	// �����ɃX���C�_�[�쐬����
	int sliderNum = 0;
	ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	// �I�΂ꂽ�I�u�W�F�N�g�̃|�C���^��pSelectObject�ɑ��
	pSelectObject = refObjects[sliderNum].get();

	ImguiManager::GetInstance()->EndDrawWindow();
}
