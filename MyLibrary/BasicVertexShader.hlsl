#include"BasicShaderHeader.hlsli"

VSOutput VSmain( float4 pos : POSITION ,float2 uv : TEXCOORD,float3 normal : NORMAL)
{
	VSOutput output;
	output.svpos = mul(mat, pos);//s—ñ‚ÆÀ•W‚ğæZ
	output.normal = mul(normalMat, normal);
	//output.normal = normal;
	output.uv = uv;

	return output;
}