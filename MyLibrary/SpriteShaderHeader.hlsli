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




//
//cbuffer cbuff1 : register(b1)
//{
//	float4 colorS;
//	float4 addColorS;
//	matrix matS;
//};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};