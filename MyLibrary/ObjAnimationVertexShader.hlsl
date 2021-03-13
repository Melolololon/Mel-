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


	//�{�[���̍s��̏�Z
	output.worldPos = mul(boneMat, pos);
	output.worldPos = mul(worldMat, output.worldPos);

	//�{�[���̍s��̏�Z
	output.svpos = mul(boneMat, pos);
	output.svpos = mul(mat, output.svpos);

	//�@���ɂ��{�[���̍s��|���Ȃ��Ƃ����Ȃ��̂ł�
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
