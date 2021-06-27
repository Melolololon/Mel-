#pragma once
#include"Vector.h"
#include<vector>
#include<string>
#include<memory>
#include<unordered_map>
#include"DirectInput.h"

class Camera
{
public:
	//カメラのモード
	enum class CameraMode
	{
		CAMERA_MODE_FPS,//1人称視点
		CAMERA_MODE_TPS,//3人称視点
	};

private:
	using UINT = unsigned int;

	static std::unordered_map<std::string, std::unique_ptr<Camera>>pCameras;
	static UINT createCount;
	static std::string mainCameraName;

	CameraMode cameraMode = CameraMode::CAMERA_MODE_FPS;
	
	//位置(FPSモードではカメラ座標、TPS視点では注視点座標)
	Vector3 position = Vector3(0,0,-10);
	Vector3 angle = 0;

	//画角
	float fovY = 60.0f;
	
	//最近点
	float nearNum = 0.01f;
	//最遠点
	float farNum = 1000.0f;
	
	//カメラ座標から注視点の距離
	float cameraToTargetDistance = 1.0f;
	
	
public:
	Camera(){}
	~Camera(){}

	/// <summary>
	/// カメラを生成します。
	/// </summary>
	/// <param name="name"></param>
	static void Create(const std::string& name = "");
	
	/// <summary>
	/// カメラを削除します。
	/// </summary>
	/// <param name="name"></param>
	static void Delete(const std::string& name);

	/// <summary>
/// カメラのポインタを取得します。
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
	static Camera* Get(const std::string& name = mainCameraName) { return pCameras[name].get(); }


#pragma region セット

	/// <summary>
	/// FPSモード時はカメラの座標を、TPS視点時は注視点の座標をセットします。
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position) 
	{
		this->position = position; 

		//仮
		DirectInput::SetViewMatrix(GetViewMatrix());
		DirectInput::SetProjectionMatrix(GetProjectionMatrix());
	}

	/// <summary>
	/// 角度をセットします。角度が0,0,0の場合、カメラは0,0,1の方向を向きます。FPSモード時はカメラ座標を基準に注視点を、TPS視点時は注視点を基準にカメラの座標を回転させます。
	/// </summary>
	/// <param name="angle"></param>
	void SetAngle(const Vector3& angle)
	{
		this->angle = angle; 

		//仮
		DirectInput::SetViewMatrix(GetViewMatrix());
		DirectInput::SetProjectionMatrix(GetProjectionMatrix());
	}

	/// <summary>
	/// 画角をセットします。
	/// </summary>
	/// <param name="fovY"></param>
	void SetFovY(const float fovY) { this->fovY = fovY; }

	/// <summary>
	/// カメラからカメラの表示範囲の一番近い場所までの距離をセットします。
	/// </summary>
	/// <param name="num"></param>
	void SetNear(const float num) 
	{
		nearNum = num; 
		DirectInput::SetNear(nearNum);
	}

	/// <summary>
	/// カメラからカメラの表示範囲の一番遠い場所までの距離をセットします。
	/// </summary>
	/// <param name="num"></param>
	void SetFar(const float num) 
	{
		farNum = num; 
		DirectInput::SetFar(farNum);
	}

	/// <summary>
	/// カメラと注視点の距離をセットします。主にTPS視点のカメラを実装するために使用します。初期値は1.0fです。
	/// </summary>
	/// <param name="distance">カメラと注視点の距離</param>
	void SetCameraToTargetDistance(const float distance)
	{
		cameraToTargetDistance = distance;
	}

	/// <summary>
	/// カメラモードをセットします。
	/// </summary>
	/// <param name="mode">カメラのモード</param>
	void SetCameraMode(const CameraMode mode)
	{
		cameraMode = mode;
	}

#pragma endregion

#pragma region ゲット

	
	/// <summary>
	/// メインカメラ(ライブラリで用意されたカメラ)の名前を取得します。
	/// </summary>
	/// <returns></returns>
	static const std::string& GetMainCameraName() { return mainCameraName; }


	DirectX::XMMATRIX GetViewMatrix()const;
	DirectX::XMMATRIX GetProjectionMatrix()const;

	/// <summary>
	/// ビュー行列とプロジェクション行列を掛けた行列を取得します。
	/// </summary>
	/// <returns></returns>
	DirectX::XMMATRIX GetViewAndProjectionMat()const;

	Vector3 GetCameraAngle()const { return angle; }

#pragma endregion
};

