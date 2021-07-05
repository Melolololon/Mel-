//#pragma once
//#include "ModelObject.h"
//#include"ObjModelData.h"
//
//class ObjModelObject :
//	public ModelObject
//{
//private:
//	static PipelineState defaultPipeline;
//
//	UINT boneNum = 0;
//	std::vector<BoneData>boneDatas;
//	std::vector<ParentBoneData> parentBoneDatas;//�e�{�[���Ɖe���x
//
//	//�{�[�����擾�p
//	ObjModelData* pObjModelData = nullptr;
//	void MapBoneMatrix();
//public:
//	ObjModelObject(ObjModelData* pModelData, ConstBufferData* userConstBufferData);
//	~ObjModelObject();
//
//#pragma region �Z�b�g
//
//
//	void SetBoneMoveVector(const Vector3& vector, const int boneNum) { boneDatas[boneNum].moveVector = vector.ToXMFLOAT3(); }
//	void SetBoneAngle(const Vector3& angle, const int boneNum) { boneDatas[boneNum].angle = angle.ToXMFLOAT3(); }
//	void SetBoneScale(const Vector3& scale, const int boneNum) { boneDatas[boneNum].scale = scale.ToXMFLOAT3(); }
//
//	void SetParentBone
//	(
//		const int boneNum,
//		const int parentBoneNum
//	)
//	{
//		parentBoneDatas[boneNum].parentBoneNum = parentBoneNum;
//	}
//
//	/// <summary>
//	/// �e�̉e�����ǂ̂��炢�󂯂邩��ݒ肵�܂�
//	/// </summary>
//	/// <param name="impact">�{��</param>
//	/// <param name="boneNum"></param>
//	void SetMoveVectorImpact
//	(
//		const Vector3& impact,
//		const int boneNum
//	)
//	{
//		parentBoneDatas[boneNum].moveVectorImpact = impact.ToXMFLOAT3();
//	}
//
//
//	void SetAngleImpact
//	(
//		const Vector3& impact,
//		const int boneNum
//	)
//	{
//		parentBoneDatas[boneNum].angleImpact = impact.ToXMFLOAT3();
//	}
//
//	void SetScaleImpact
//	(
//		const Vector3& impact,
//		const int boneNum
//	)
//	{
//		parentBoneDatas[boneNum].scaleImpact = impact.ToXMFLOAT3();
//	}
//#pragma endregion
//
//	static bool Initialize();
//	void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama())override;
//};
//
