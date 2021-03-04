

cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix billboardMat;
};


struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 scale : TEXCOORD;
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};