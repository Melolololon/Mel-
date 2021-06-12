#pragma once
#include"Vector.h"
#include"LibMath.h"
class Camera
{
	
private:
	Vector3 position = 0;
	Vector3 angle = 0;
	float fovY = 60.0f;
	float nearNum = 0.01f;
	float farNum = 1000.0f;
	
public:

#pragma region セット

	/// <summary>
	/// 座標をセットします。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& position) { this->position = position; }

	/// <summary>
	/// 角度をセットします。角度が0,0,0の場合、0,0,1の方向を向きます。
	/// </summary>
	/// <param name="angle"></param>
	void SetAngle(const Vector3& angle) { this->angle = angle; }

	/// <summary>
	/// 画角をセットします。
	/// </summary>
	/// <param name="fovY"></param>
	void SetFovY(const float fovY) { this->fovY = fovY; }

	void SetNear(const float num) { nearNum = num; }
	void SetFar(const float num) { farNum = num; }

#pragma endregion


	/// <summary>
	/// ビューとプロジェクションを掛けた行列を取得。
	/// </summary>
	/// <returns></returns>
	DirectX::XMMATRIX GetViewAndProjectionMat();
};

