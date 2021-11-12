#pragma once
#include"DirectXStruct.h"
#include"Texture.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"


#include<d3d12.h>
#include<dxgi.h>
#include<d3dx12.h>
#include<vector>
#include<array>

//頂点使いまわそうか考えたが、
//sizeとかuvとかfloat4にしないといけなくて、
//総合的に使用メモリ多そうだから毎回生成する
//使いまわさないとuvを定数バッファで指定しないといけないから、
//分けたほうがいいかも
//四角形GPUに送ってそこから分割したほうがいいかも
//スプライトだったらテッセレータで分割してバラバラにしたほうがいいかも


//これを継承して2D、3Dスプライトクラスを作る
//Drawは仮想関数。それぞれにmap処理を書く
//

//途中でテクスチャ変えたり、べた塗りにしたりできるようにする
//そうしないとスプライトフォントのスプライトを先に用意できない

//ピクセル単位で拡縮できるように(今からどのくらいピクセル加算するか決めたり、直接ピクセル数決められるようにしてもいいかも)

namespace MelLib
{
	class Sprite
	{
	private:

#pragma region 変数
		static const UINT MAX_TEXTURE_LOAD_NUM = 256 * 20;


#pragma endregion

#pragma region 関数


#pragma endregion

	protected:

		static ID3D12GraphicsCommandList* cmdList;
		static ID3D12Device* device;
		ComPtr<ID3D12PipelineState> pipeline;
		std::array<SpriteVertex, 4> vertices;
		static ComPtr<ID3D12RootSignature>rootSignature;

#pragma region バッファ

		//インデックスは、スプライトをバラバラにしたりする処理に使えそうだから実装しとく
		//ジオメトリでやるから消した
		VertexBufferSet vertexBufferSet;
		ComPtr<ID3D12Resource> constBuffer;

		static std::vector<ComPtr<ID3D12Resource>> textureBuffer;
		static ComPtr<ID3D12DescriptorHeap> textureHeap;
#pragma endregion


		//描画するときの左上の座標
		Vector2 drawLeftUpPosition = 0;
		//描画するときの右下の座標
		Vector2 drawRightDownPosition = 1;

		//座標などの情報をまとめた構造体
		SpriteConstData constData;


		void CreateBuffer();

		//単色スプライト生成時に色をセットする関数
		void SetOneColorSpriteColor(const Color& color);

		//ColorをAddColorとか分けずにまとめるためにMap関数でMapするので、変数用意した
		Color color;
		Texture* pTexture = nullptr;

#pragma region 関数
		void ConstDataMat();
		void SetCmdList(Texture* texture);

#pragma region マップ
		void MapVertexBuffer(void** data);
		void UnmapVertexBuffer();
#pragma endregion


		void InitializeVertices();



#pragma endregion
		virtual void Create(){}

		/// <summary>
		/// 生成します。
		/// </summary>
		/// <param name="color">色</param>
		virtual void Create(const Color& color) {}

		/// <summary>
		/// 生成します。レンダーターゲットの生成は行えません。
		/// </summary>
		/// <param name="pTexture">テクスチャのポインタ</param>
		virtual void Create(Texture* pTexture) {}

	public:
		Sprite();
		~Sprite();

		//こいつここに定義しなくていい
		//レンダーターゲットで使わない
		//Drawからレンダーターゲットセットなくして、Sprite2Dと3Dにセット関数作る?
		virtual void Draw(const std::string& rtName = "");


		void SetDrawArea(const Vector2& leftUpPos, const Vector2& rightDownPos)
		{
			drawLeftUpPosition = leftUpPos;
			drawRightDownPosition = rightDownPos;
		}
#pragma region 開発者用関数

		static void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd);

		static void CreateTextureBuffer(Texture* texture);



#pragma endregion

#pragma region 見た目操作
		void SetAddColor(const Color& color);
		void SetSubColor(const Color& color);
		void SetMulColor(const Color& color);
#pragma endregion

#pragma region Get
		Color GetColor()const { return color; }
		const Texture* GetTexture()const { return pTexture; }
#pragma endregion


#pragma region パイプライン

		void SetPipeline(PipelineState* pipelineState);

#pragma endregion



	};
}
