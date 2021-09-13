#pragma once
#include<d3d12.h>
#include<wrl.h>

#include<string>

#include"Vector.h"
#include"Color.h"

#include"imgui/imgui.h"

//Imguiを管理するクラス

namespace MelLib 
{

	class ImguiManager
	{
	public:

	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3D12DescriptorHeap>desHeap = nullptr;

		bool releaseDrawFlag = true;

		ID3D12Device* pDevice = nullptr;
		ID3D12GraphicsCommandList* pCmdList = nullptr;
	private:
		ImguiManager() {}
		~ImguiManager() {}

		/// <summary>
		/// リリースかつreleaseDrawFlagがfalseだったらtrue
		/// </summary>
		/// <returns></returns>
		bool CheckReleaseDrawFlag();
	public:
		ImguiManager(ImguiManager& i) = delete;
		ImguiManager& operator=(ImguiManager& i) = delete;
		static ImguiManager* GetInstance();

		bool Initialize(ID3D12Device* pDev, ID3D12GraphicsCommandList* pCmdList);
		void Finalize();

		void Begin();
		void Draw();
		
		/// <summary>
		/// ウィンドウを描画する準備を行います。
		/// </summary>
		/// <param name="name">ウィンドウ名</param>
		/// <param name="pos">座標</param>
		/// <param name="size">サイズ</param>
		void BeginDrawWindow
		(
			const std::string& name,
			const Vector2& pos,
			const Vector2& size
		);

		/// <summary>
		/// 描画処理を終了します。BeginDrawWindowを呼び出し、ウィンドウの項目の設定を行った後に必ず呼び出してください。
		/// 複数ウィンドウを作成する場合、各ウィンドウの項目の設定が終るごとに呼び出してください。
		/// </summary>
		void EndDrawWindow();

		/// <summary>
		/// リリース時にウィンドウを描画するかを設定します。設定する場合は、ライブラリの初期化より前で呼び出してください。
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseDrawFlag(const bool flag) { releaseDrawFlag = flag; }


#pragma region 生成
		/// <summary>
		/// ラジオボタンをウィンドウに生成します。
		/// </summary>
		/// <param name="label">ラベル名(チェックボックスの隣に表示される文字)</param>
		/// <param name="pInt">選択されているときにnumの値を代入する変数のポインタ</param>
		/// <param name="num">選択されているときにpIntが参照している変数に代入する値</param>
		/// <returns>現在のフレームで選択されたらtrue(正確には、選択されて値の代入処理が行われたらtrue)</returns>
		bool DrawRadioButton(const std::string& label, int* pInt, const int num);

		/// <summary>
		/// チェックボックスをウィンドウに表示します。
		/// </summary>
		/// <param name="label">ラベル名(チェックボックスの隣に表示される文字)</param>
		/// <param name="flag">押されているかどうかのフラグを代入する変数のポインタ</param>
		/// <returns></returns>
		bool DrawCheckBox(const std::string& label, bool* flag);

		bool DrawSliderInt(const std::string& label, int* pInt, const int numMin, const int numMax);
		bool DrawSliderFloat(const std::string& label, float* pFloat, const float numMin, const float numMax);

		bool DrawColorPocker(const std::string& label, Color* pColor,const ImGuiColorEditFlags flag);
#pragma endregion

	};
}

