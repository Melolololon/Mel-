#pragma once
#include"Vector.h"
#include"ModelObject.h"

namespace MelLib 
{
	//�T�C�Y��������悤�ɂ���?
	//�R�X�g���i�s�s�\�I�u�W�F�N�g�Ƃ̔��莞�Ɍv�Z����悤�ɂ���

	//A�X�^�[�̃m�[�h
	struct AStarNode
	{

		using UINT = unsigned int;

		
		//���W
		Vector2 position = 0;

		//�R�X�g
		UINT cost = 1;

		//�v�Z����(�X�e�b�v + ���� + �R�X�g)
		UINT calcNum = UINT_MAX;
		

		//�ȉ��T���p�ϐ�

		//�z��̃C���f�b�N�X
		int indexX = INT_MAX;
		int indexY = INT_MAX;

		//close�z��̃C���f�b�N�X
		int closeIndex = INT_MAX;

		//�X�^�[�g����̋���
		int stepNum = 0;

		//�O�̃m�[�h
		AStarNode* previousNode = nullptr;

		bool openFlag = false;
		bool closeFlag = false;

		//�i�s�s�\�I�u�W�F�N�g�Əd�Ȃ��Ă���m�[�h
		bool hitObjectNode = false;

		
#ifdef _DEBUG

		/*AStarNode();
		~AStarNode();*/

		//static std::vector<AStarNode*>pNodes;

		////�}�X��
		//ModelObject nodeModel;

		//void Draw();
		//static void AllDraw();

#endif // _DEBUG


	};

	//�o�H�T���N���X
	class RouteSearch
	{
		using UINT = unsigned int;
		//���x�ł�������A�v���C���[��G�̑傫�����l�����Čo�H���v�Z�ł���悤�ɂ���


#pragma region �ŒZ�o�H

		/// <summary>
		/// ��������Ƀm�[�h�̍��W���Z�b�g���܂��B
		/// </summary>
		/// <param name="leftUpPos">������W</param>
		/// <param name="rightDownPos">�E�����W</param>
		/// <param name="nodeNumX">��������</param>
		/// <param name="nodeNumY">�c������</param>
		/// <param name="nodes">�m�[�h��vector(size��0�ł悢)</param>
		/// <param name="upPlus">��������v���X���ǂ���</param>
		static void SetAStarNodePosition
		(
			const Vector2& leftUpPos,
			const Vector2& rightDownPos,
			const UINT& nodeNumX,
			const UINT& nodeNumY,
			std::vector< std::vector<AStarNode>>& nodes,
			const bool upPlus
		);

		//���̊֐��ŃR�X�g��1����Ȃ��I�u�W�F�N�g�Ƃ��̃R�X�g��n���悤�ɂ���?
		/// <summary>
		/// �m�[�h���i�s�s�\�I�u�W�F�N�g�Ƀq�b�g���Ă邩���m�F���܂��B
		/// </summary>
		/// <param name="hitObjectsPos">��Q���̍��W��vector</param>
		/// <param name="hitObjectsSize">��Q���̃T�C�Y��vector</param>
		/// <param name="nodes">�m�[�h��vector(SetAStarNodePosition�ɓn������̔z��)</param>
		static void SetAStarNodeHitObjectNodeFlag
		(
			const std::vector<Vector2>& hitObjectsPos,
			const std::vector<Vector2>& hitObjectsSize,
			std::vector< std::vector<AStarNode>>& nodes
		);

		/// <summary>
		/// �n���ꂽ�f�[�^�����ƂɍŒZ�o�H�����߂܂��B
		/// </summary>
		/// <param name="startPos">�X�^�[�g�n�_�̍��W</param>
		/// <param name="endPos">�S�[���n�_�̍��W</param>
		/// <param name="nodes">�m�[�h��vector(SetAStarNodeHitObjectNodeFlag�ɓn������̔z��)</param>
		/// <param name="routeVector">�S�[���܂ł̃��[�g���i�[����vector(size��0�ł悢)</param>
		/// <returns>�T���������������ǂ���</returns>
		static bool GetAStarCalcResult
		(
			const Vector2& startPos,
			const Vector2& endPos,
			std::vector< std::vector<AStarNode>>& nodes,
			std::vector<Vector2>& routeVector
		);

#pragma endregion

	};
}

