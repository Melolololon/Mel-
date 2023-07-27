#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<memory>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>


#include"ModelData.h"
#include"PipelineState.h"
#include"Texture.h"
#include"Camera.h"
#include"RenderTarget.h"
#include"BufferData.h"
#include"CollisionDetectionData.h"
#include"Material.h"

#include"GuiValue.h"
//AddColorとかまとめよう!

namespace MelLib
{
	//モデルの座標などをまとめたもの
	class ModelObject
	{
	private:
		// 名前,生成数
		static std::unordered_map<std::string, int>createCount;

	private:

		static std::unordered_map<std::string, std::unique_ptr<ModelObject>>pModelObjects;

		static ID3D12Device* device;
		static std::vector<ID3D12GraphicsCommandList*>cmdLists;
		static ComPtr<ID3D12RootSignature>rootSignature;
	
		std::vector<std::string>objectNames;

		//[モデル内のオブジェクトごと]
		std::unordered_map < std::string, Material*>materials;

#pragma region ボーンとアニメーションの情報
		// ボーンの最大数
		static const UINT BONE_MAX = 64;
		struct SkinConstBufferData
		{
			DirectX::XMMATRIX bones[BONE_MAX];
		};

		struct FbxAnimationTimes
		{
			FbxTime startTime;
			FbxTime endTime;
			//1フレームの時間
			FbxTime frameTime;
		};

		//fbxモデルのアニメーション用の情報をまとめたもの
		struct FbxAnimationData
		{
			FbxAnimationTimes animationTimes;
			FbxTime currentTime;
			int timeMag = 1;
			std::string currentAnimationName;

			bool animationEndStop = false;
			bool animationReverse = false;
			bool animationEnd = false;
			bool isAnimation = false;
		};


		//[ボーンごと]
		// これおそらくobj用
		std::vector<BoneData>boneDatas;
		std::vector<ParentBoneData> parentBoneDatas;//親ボーンと影響度

		// こいつをボーン分リサイズする
		// ボーンごとのアニメーションデータ
		// ["オブジェクト名"]["ボーン名"]
		// 一旦オブジェクト名で管理する配列無くした
		std::unordered_map<std::string, FbxAnimationData> fbxAnimationDatas;

		//FbxAnimationData fbxAnimationData;
		//bool isAnimation = false;
		//bool animationEndStop = false;
		//bool animationReverse = false;
		//bool animationEnd = false;
#pragma endregion

		//定数バッファ
		static const int CONST_BUFFER_REGISTER = 0;
		std::vector<ComPtr<ID3D12Resource>> constBuffer;//メイン(基本的な情報)

		static const int MATERIAL_BUFFER_REGISTER = 2;

		static const int COLOR_MATERIAL_BUFFER_REGISTER = 4;

		static const int USER_BUFFER_REGISTER = 1;
		std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//ユーザー
		ConstBufferData userConstBufferData;

		static const int MODEL_BUFFER_REGISTER = 3;
		// モデルのオブジェクトごとに生成するように変更すること 2021 12/8
		std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//モデル特有(アニメーション関係の情報)
		ConstBufferData modelConstBufferData;

		static const int TEXURE_ROOTPARAM_NUM = 5;

		//定数にセットする座標などの値
		//[モデル内のオブジェクト数]
		std::unordered_map < std::string , ModelConstData > modelConstDatas;

		//モデル(クリエイトしたら割り当てる)
		ModelData* pModelData = nullptr;
		UINT modelFileObjectNum = 0;



		//テクスチャ未セット時にセットする透明のテクスチャバッファ
		static ComPtr<ID3D12Resource>colorZeroTexBuffer;

		//こいつ宣言してModelObjectのvectorとかresizeするとエラー出る。対策して
		//ModelDataを持たせないでモデルデータ渡した時に使うデータだけ持ってくればいい?
		//一時的にModelData作ってModelObjectにセットしてすぐ消しちゃう
		//渡すとなると、生成するごとに頂点のコピーが必要になる(コピーしないとカットしたモデルの頂点参照できない)
		//直接ModelObjectに頂点とか渡して作れるようにしてもいいかも
		//頂点とインデックス関係のものだけをまとめたクラスを作って、それをこれに持たせるのもあり
		
		// //ModelDataないとヒープ用意できないから、
		//マテリアルとかのコピーコンストラクタ作ったほうがいいかも
		std::unique_ptr<ModelData> catFrontModelData;
		std::unique_ptr<ModelData> catBackModelData;
		//ModelData* catFrontModelData;
		//ModelData* catBackModelData;

		std::string objectName;

		/*GuiVector3 guiPosition;
		GuiVector3 guiAngle;
		GuiVector3 guiScale;*/
		// 自動でGUIにデータをセットするかどうか(コピーコンストラクタやコピー代入演算子を使う場合コピー先の名前を使いたいため、falseにする)
		//bool autoSetGuiData = true;

	private:

		void CreateConstBuffer();

		void DrawCommonProcessing(const std::string& rtName);
		void MapConstData(const Camera* camera);
		void SetCmdList();

		void FbxAnimation(const std::string& boneName);

		void GetFbxAnimationData(const std::string& boneName, FbxAnimationData& data)const;

	public:

		//nullptr渡される可能性を考えると、boolをreturnできるようにしたほうがいい?
		ModelObject() 
		{
		}

		ModelObject(ModelObject& obj,const std::string& name);
		ModelObject(ModelObject& obj);
		//ModelObject& operator= (ModelObject& obj);
		~ModelObject() {}

		enum class CopyModelObjectContent 
		{
			ALL,
			NUMBER_FLAG,
		};
		bool CopyModelObject(const ModelObject& obj, const std::string& name, CopyModelObjectContent copyContent);


		static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);


		static ModelObject* Get(const std::string& name) { return pModelObjects[name].get(); }
		static void Delete(const std::string& name);

		/// <summary>
		/// アニメーションなどの更新処理を行います
		/// </summary>
		void Update();
		void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());

#pragma region 

#pragma region モデルで生成
		ModelObject(ModelData* pModelData, const std::string& objectName,  ConstBufferData* userConstBufferData = nullptr);
		static bool Create(ModelData* pModelData, const std::string& objectName , ConstBufferData* userConstBufferData, const std::string& name);
		bool Create(ModelData* pModelData,const std::string& objectName , ConstBufferData* userConstBufferData = nullptr);
#pragma endregion 


#pragma region プリミティブモデル生成
	

#pragma endregion 


#pragma endregion 

#pragma region メッシュカット
		/// <summary>
		/// モデルを平面に応じて切断します。
		/// </summary>
		/// <param name="plane">平面</param>
		/// <param name="pFront">平面の表側にあるモデル情報を格納するModelDataのポインタ</param>
		/// <param name="pBack">平面の裏側にあるモデル情報を格納するModelDataのポインタ<</param>
		/// <param name="createCrossSection">断面を形成するかどうか</param>
		/// <returns>切断できたかどうか</returns>
		bool MeshCat(const PlaneData& plane,ModelData*& pFront, ModelData*& pBack,const bool createCrossSection);
#pragma endregion


#pragma region セット

#pragma region 操作見た目変更

#pragma region 操作


		void SetPosition(const Vector3& position,const std::string& name = "");
		void SetScale(const Vector3& scale, const std::string& name = "");
		void SetAngle(const Vector3& angle, const std::string& name = "");

		void SetAddUV(const MelLib::Vector2& uv, const std::string& name = "");

#pragma endregion

#pragma region 色
		/// <summary>
		/// 色を加算します。
		/// </summary>
		/// <param name="color">色</param>
		/// <param name="name">モデル内のオブジェクト名(指定しない場合、1番目のオブジェクトを指定)</param>
		void SetAddColor(const Color& color, const std::string& name = "");
		void SetSubColor(const Color& color, const std::string& name = "");
		void SetMulColor(const Color& color, const std::string& name = "");
#pragma endregion


#pragma region アニメーション

		void SetAnimationPlayFlag(const bool flag, const std::string& boneName = "");//{ isAnimation = flag; }

		/// <summary>
		/// アニメーションをリセットします。
		/// </summary>
		void SetAnimationFrameStart(const std::string& boneName = "");//{fbxAnimationData.currentTime = 0;}

		/// <summary>
		/// アニメーションの現在のフレームをアニメーション終了時のフレームにします。
		/// </summary>
		void SetAnimationFrameEnd(const std::string& boneName = "");//{ fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime; }

		/// <summary>
		/// アニメーションのフレームをセットします。
		/// </summary>
		/// <param name="fream"></param>
		void SetCurrentFream(const UINT fream, const std::string& boneName = "");

		/// <summary>
		/// アニメーションの再生速度をセットします。
		/// </summary>
		/// <param name="magnification"></param>
		void SetAnimationSpeedMagnification(const unsigned int magnification, const std::string& boneName = "");// { fbxAnimationData.timeMag = magnification; }

		/// <summary>
		/// アニメーションを逆再生するかどうかを設定します。
		/// </summary>
		/// <param name="flag"></param>
		void SetAnimationReversePlayBack(const bool flag, const std::string& boneName = "");

		/// <summary>
		/// アニメーションを指定します。
		/// </summary>
		/// <param name="name"></param>
		void SetAnimation(const std::string& animationName, const std::string& boneName = "");

		/// <summary>
		/// アニメーション終了時に再生を終了するかを指定します。
		/// </summary>
		/// <param name="flag"></param>
		void SetAnimationEndStopFlag(const bool flag, const std::string& boneName = "");//{ animationEndStop = flag; }

		/// <summary>
		/// アニメーションのフレームをセットします。
		/// </summary>
		/// <param name="frame"></param>
		void SetAnimationFrame(const unsigned int frame, const std::string& boneName = "") ;//{ fbxAnimationData.currentTime.SetFrame(frame, FbxTime::eFrames60); }

#pragma endregion

		void SetGUIValueData(const std::string& name);

#pragma endregion

		/// <summary>
		/// シェーダーで汎用的に使えるパーセントの値を変更します。
		/// </summary>
		void SetPar(float par, const std::string& name = "");

		/// <summary>
		/// マテリアルをセットします。
		/// </summary>
		/// <param name="mtl"></param>
		/// <param name="name"></param>
		void SetMaterial(Material* mtl,const std::string& name = "");



		void SetGUIParamDrawFlag(const bool flag,const std::string& name = "");

#pragma endregion

#pragma region ゲット

		//この辺constにする

		ModelData* GetPModelData() { return pModelData; }
		Material* GetPMaterial(const std::string& name = "");

#pragma region 操作見た目変更

#pragma region 操作
		
		//今はとりあえず全部値が一緒なので、0のやつを返してる
		Vector3 GetPosition(const std::string& name = "")const;

		Vector3 GetAngle(const std::string& name = "")const;
		Vector3 GetScale(const std::string& name = "")const;

	    MelLib::Vector2 GetAddUV( const std::string& name = "")const;
#pragma endregion
#pragma endregion


#pragma region アニメーション


		std::string GetCurrentAnimationName(const std::string& boneName = "")const;//{ return fbxAnimationData.currentAnimationName; }

		/// <summary>
		/// アニメーションが逆生成中かどうかを取得します。
		/// </summary>
		/// <returns></returns>
		bool GetAnimationReversePlayBack(const std::string& boneName = "")const;//{ return animationReverse; }

		/// <summary>
		/// アニメーションが終了しているかどうかを取得します。
		/// </summary>
		/// <returns></returns>
		bool GetAnimationEndFlag(const std::string& boneName = "")const;//{ return animationEnd; }
		//{ return fbxAnimationData.currentTime == fbxAnimationData.animationTimes.endTime; }


		/// <summary>
		///	アニメーションの現在のフレームを取得します。
		/// </summary>
		/// <returns></returns>
		unsigned int GetAnimationFrame(const std::string& boneName = "")const;//{ return static_cast<unsigned int>(fbxAnimationData.currentTime.GetFrameCount(FbxTime::eFrames60)); }
		
		/// <summary>
		/// アニメーションのフレーム数を取得します。
		/// </summary>
		/// <returns></returns>
		unsigned int GetAnimationFrameCount(const std::string& boneName = "")const;//{ return static_cast<unsigned int>(fbxAnimationData.animationTimes.endTime.GetFrameCount(FbxTime::eFrames60)); }
#pragma endregion

		//コンピュートシェーダーで計算したほうがいい。
		//できそうなら描画時に頂点シェーダーで計算した結果を持ってきたほうがいい?
		// 戻り値がumap版のやつ作ってもいいかも
		/// <summary>
		/// 頂点座標を取得します。
		/// </summary>
		/// <param name="scaleImpact"></param>
		/// <param name="angleImpact"></param>
		/// <param name="transformImpact"></param>
		/// <param name=""></param>
		/// <returns></returns>
		std::vector<std::vector<Vector3>>GetVerticesData
		(
			const bool scaleImpact,
			const bool angleImpact,
			const bool transformImpact
		);

		/// <summary>
		/// モデルのポリゴンを当たり判定に使えるデータに変換して返します。
		/// </summary>
		void GetModelTriangleData(std::vector<std::vector<TriangleData>>& vec)const;
		
#pragma endregion

		/// <summary>
		/// 指定したボーンとメッシュの行列を渡した座標に乗算して返します。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="boneName">ボーン名</param>
		/// <param name="meshName">メッシュ名</param>
		/// <returns>行列乗算後の座標</returns>
		Vector3 CalcAnimationPosition
		(
			const Vector3& pos,float weigth, const std::string& boneName,const std::string& meshName
		, const MelLib::Vector3& startPos, const MelLib::Vector3& startAngle, const MelLib::Vector3& startScale)const;

	};
}
