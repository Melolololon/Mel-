#pragma once
#include"GameObject.h"
#include"Vector.h"

// メモエリア
// 2022 08 08
// ボーンの構造体またはクラスを新たに作りたい
// 当たり判定を持たせたり、位置を変えられるようにしたい
// おっぱいを動かしたいんじゃ!
// あと地形に足がめり込まないようにしたい

// 親ボーンとの最大距離を設定可能にしてそれ以上離れないように
// 貧乳は値を小さくして動かせないように、巨乳は値を大きくして動かせるように

namespace MelLib 
{

	class ModelBone :public MelLib::GameObject
	{
	private:
		//ボーン名
		std::string boneName;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;

		// 前のボーン
		ModelBone* parentBone = nullptr;

		//ボーン情報をまとめたもの
		FbxCluster* fbxCluster = nullptr;

		// 加算地
		Vector3 addPosition;
		Vector3 addAngle;
		Vector3 addScale;

	public:

		void SetAddPosition(const Vector3& vector) { addPosition = vector; }
		void SetAddAngle(const Vector3& vector) { addAngle = vector; }
		void SetAddScale(const Vector3& vector) { addScale = vector; }

		Vector3 GetAddPosition()const { return addPosition; }
		Vector3 GetAddAngle() const { return addAngle; }
		Vector3 GetAddScale()const { return addScale; }


		// 関数で外部から当たり判定をセットできるようにする

	};

}