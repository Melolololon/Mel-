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

	output.normal = normalize(mul(normalMat, normal));
	output.uv = uv;

	return output;
}
