#include"BasicShaderHeader.hlsli"

VSOutput VSmain( float4 pos : POSITION ,float2 uv : TEXCOORD,float3 normal : NORMAL)
{
	VSOutput output;
	output.svpos = mul(mat, pos);//�s��ƍ��W����Z
	output.normal = mul(normalMat, normal);
	//output.normal = normal;
	output.uv = uv;

	return output;
}