#include"ObjShaderHeader.hlsli"
#include"ObjAnimationShaderHeader.hlsli"

VSOutput VSmain
(
	float4 pos : POSITION, 
	float2 uv : TEXCOORD, 
	float3 normal : NORMAL,
	unsigned int bone : BONENUM
)
{
	matrix boneMat = boneMatArray[bone];

	VSOutput output;


	//ボーンの行列の乗算
	output.worldPos = mul(boneMat, pos);
	output.worldPos = mul(worldMat, output.worldPos);

	//ボーンの行列の乗算
	output.svpos = mul(boneMat, pos);
	output.svpos = mul(mat, output.svpos);

	//法線にもボーンの行列掛けないといけないのでは
	output.normal = normalize(mul(normalMat, normal));
	output.uv = uv;

	return output;

/*
	VSOutput output;
	output.worldPos = mul(worldMat, pos);
	output.svpos = mul(mat, pos);
	output.normal = normalize(mul(normalMat, normal));
	output.uv = uv;

	return output;*/
}

//
//VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL)
//{
//
//	VSOutput output;
//	output.worldPos = mul(worldMat, pos);
//	output.svpos = mul(mat, pos);
//	output.normal = normalize(mul(normalMat, normal));
//	output.uv = uv;
//
//	return output;
//}
