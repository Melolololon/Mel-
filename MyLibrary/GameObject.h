#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

#ifdef _DEBUG
#include"PipelineState.h"
#include"ModelObject.h"
#include"Material.h"
#endif // _DEBUG


//�I�u�W�F�N�g�}�l�[�W���[�ǉ����ɔ����I�Ԃ悤�ɂ���?(���育�Ƃɒǉ��֐��Ɣz����)

//GameObject���p��������PhysicsObject���ƁAHit�֐��ŏd���Ƃ��󂯎��Ȃ��Ȃ邩��܂Ƃ߂�

//velocity�Ƃ�������ɏ�����������ƃo�O��B
//GameObject��position�Ƃ���private�ɂ��Ċ֐��ŃZ�b�g����悤�ɂ���?(velocity�͏����������Ȃ��悤�ɂ���)
//��������΁AUnity�݂����ɂȂ�
//position��velocity���Z�b�g�ł���悤�ɂ���B(�ϐ��p�ӂ���̂߂�ǂ�����)

//SetPosition�����z�֐��ɂ��āA���f���ɍ��W�Z�b�g���鏈���Ƃ�������悤�ɂ���
namespace MelLib 
{
	class GameObject
	{
	private:


#ifdef _DEBUG
		//����m�F�p���f���̃p�C�v���C��
		static PipelineState collisionCheckModelPipelineState;

		//����m�F�p���f���̃}�e���A��
		static ADSAMaterial material;

		//����m�F�p���f��

		std::vector<ModelObject>sphereModelObjects;
		std::vector<ModelObject>boxModelObjects;

		//�J�v�Z�������L�΂��ƌ`����邩��A���Ɖ~���g�ݍ��킹�ĕ\�������ق�����������
		//����̃f�[�^�̕ϐ��͍��̂܂�(�p�x�����Ɖ~����g�ݍ��킹��̂���ςɂȂ�)
		std::array<std::vector<ModelObject>, 3>capsuleModelObjects;
#endif // _DEBUG

		


	protected:

		Vector3 position = 0;
		Vector3 velocity = 0;
		Vector3 speed = 0;



#pragma region �����֌W

		//�����I�ȋ������s�����߂̌v�Z���s�����ǂ����B
		bool calcPhysics = false;

		//�����x
		Vector3 acceleration = 0;
		//���̂�������
		Vector3 force = 0;
		//�d��
		float mass = 100.0f;
#pragma endregion

#pragma region ����f�[�^
		CollisionDetectionFlag collisionFlag;

		std::vector<SphereData> sphereData;
		

		std::vector<BoxData> boxData;


		std::vector<Segment3DData> segment3DData;

		std::vector<RayData> layData;
		std::vector<PlaneData>planeData;

		std::vector<BoardData>boardData;

		std::vector<CapsuleData>capsuleData;


#pragma endregion


		//�\�[�g�p���l�B�\�[�g���������Œ������邽�߂̕ϐ�
		short sortNumber = 0;

		//�����t���O(���ꂪtrue�ɂȂ�ƁA�I�u�W�F�N�g�}�l�[�W���[���珜�O�����)
		bool eraseManager = false;
		

	public:

		//�R���X�g���N�^
		GameObject();
		//�f�X�g���N�^
		virtual ~GameObject();

		//�X�V
		virtual void Update();
		//�`��
		virtual void Draw();

		/// <summary>
		/// �����������̏���
		/// </summary>
		/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
		/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
		/// <param name="arrayNum">�����̉��ڂ̔���ɓ���������</param>
		/// <param name="hitObjColType">����̂ǂ̔���ɓ���������</param>
		/// <param name="hitObjArrayNum">����̉��ڂ̔���ɓ���������</param>
		virtual void Hit
		(
			const GameObject* const  object,
			const ShapeType3D collisionType,
			const int arrayNum,
			const ShapeType3D hitObjColType,
			const int hitObjArrayNum
		);




		//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

		//virtual const void* GetPtr()const;



		//�m���ɏ����l�����܂��Ă���ϐ�(eraseManager�Ȃ�)������������ϐ�(�������Y��h�~�p)
		//����ŏ����������Ƀw�b�_�ŏ���������?
		//�Ēǉ������Ƃ��ɏ��������������炱�̂܂܂ł���
		void ObjectInitialize();

		//�����Ɋ�Â����ړ��֌W�̌v�Z���s���֐�
		void CalcMovePhysics();

#pragma region �Z�b�g


		void SetPosition(const Vector3& position) { this->position = position; }
		void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
#pragma endregion

#pragma region �Q�b�g


		Vector3 GetPosition()const { return position; }
		Vector3 GetVelocity()const { return velocity; }
		Vector3 GetAcceleration()const { return acceleration; }
		Vector3 GetForce()const { return force; }
		float GetMass()const { return mass; }
		bool GetCalcPhysicsFlag()const { return calcPhysics; }
		short GetSortNumber() const { return sortNumber; }

		//�I�u�W�F�N�g�}�l�[�W���[����폜���邩�ǂ����̃t���O��Ԃ�
		bool GetEraseManager() { return eraseManager; }
#pragma endregion


#pragma region ����p�֐�


		//����p�֐�
		CollisionDetectionFlag GetCollisionFlag() { return collisionFlag; }
		std::vector<SphereData> GetSphereData() { return sphereData; }
		std::vector<BoxData> GetBoxData() { return boxData; }
		std::vector<Segment3DData> GetSegmentData() { return segment3DData; }
		std::vector<PlaneData> GetPlaneData() { return planeData; }
		std::vector<BoardData> GetBoardData() { return boardData; }
		std::vector<CapsuleData>GetCapsuleData() { return capsuleData; }

		//�����Q�Ǝ擾����Ȃ���Set�ɂ���?
		//���������������Ȃ��BHit�֐��œn��
		void SetSphereCalcResult(const SphereCalcResult& result, const UINT index) { sphereData[index].SetCalcResult(result); }
		void SetBoxCalcResult(const BoxCalcResult& result, const UINT index) { boxData[index].SetCalcResult(result); }
		void SetSegmentCalcResult(const Segment3DCalcResult& result, const UINT index) { segment3DData[index].SetCalcResult(result); }
		void SetBoardCalcResult(const BoardCalcResult& result, const UINT index) { boardData[index].SetCalcResult(result); }

		//Vector3& GetLineSegmentHitPosition(const int num);
		//Vector3& GetBoardHitPosition(const int num);
		//BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
		//BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }



#ifdef _DEBUG
		static void CreateCollisionCheckModelPipelineState();

		//�Փˊm�F�p���f�������A�폜
		void CreateCollisionCheckModel();
		//�Փˊm�F�p���f���̃f�[�^�Z�b�g
		void SetCollisionCheckModelData();
		//�Փˊm�F�p���f���̕`��
		void DrawCollisionCheckModel();
#endif // _DEBUG

#pragma endregion


	};
}