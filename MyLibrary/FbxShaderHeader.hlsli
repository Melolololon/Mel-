

#define NUM_CONTROL_POINTS 3
#define DOMEIN "tri"


//ハルシェーダー記述

//コントロールポイント = 頂点?

//入力コントロールポイント
struct VS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;


	//float4 worldPos:POSITION;
	//float4 svpos : SV_POSITION;
	float4 pos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

//Outputするコントロールポイント
struct HS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;

	/*float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;*/
	float4 pos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};


//出力パッチ定数データ
struct HS_CONSTANT_DATA_OUTPUT
{
	//パッチの各頂点のテッセレーション量(どのくらい分割するか?)(外側の辺をどのくらい分割するか?)
	//[]内は頂点数?
	//見た目だけ細かくしたい場合は、ここだけ値を大きくすればいい?
	float EdgeTessFactor[3] : SV_TessFactor;
	
	//パッチサーフェス内のテッセレーションの量(ポリゴンの内側の分割量?)
	//スプライトをばらばらにするときや波を作るときなどの平べったい部分(内側。面)も細かくしたいときはこれの値を大きくする?
	float InsideTessFactor : SV_InsideTessFactor;

};





//ドメインシェーダー記述
//ドメインシェーダーのアウトプット
struct DS_OUTPUT
{
	//float4 vPosition  : SV_POSITION;

	float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};







struct VSOutput
{
	/*float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;*/

	float4 pos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


//基本的な情報
static const int DIR_LIGTH_MAX = 20;
cbuffer cbuff0 : register(b0)
{
	float4 lightColor[DIR_LIGTH_MAX];
	float4 light[DIR_LIGTH_MAX];
	float4 cameraPos;
	matrix mat;
	matrix normalMat;
	matrix worldMat;
	float4 mulColor;
	float4 addColor;
	float4 subColor;
	float ex;
};

//マテリアル
cbuffer cbuff2 : register(b2)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);


};

//PBRマテリアル
//cbuffer cbuff4 : register(b4)
//{
//	float3 baseColor;
//	float metalness;
//	float specular;
//	float roughness;
//}

//色
cbuffer cbuff4 : register(b4)
{
	float4 baseColor;
}

//スキニング
cbuffer cbuff3 : register(b3)
{
	matrix matSkinning[64];
}

