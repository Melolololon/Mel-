//全オブジェクト共通のデータはmapを少なくしよう!


cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
	matrix normalMat;
	float4 mulColor;
	float4 addColor;
	float4 subColor;
	float ex;
	matrix worldMat;
};


cbuffer cbuff3 : register(b3)
{
	float4 lightColor;
	float4 light;
	float4 cameraPos;
	matrix lightMat;
};


struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};