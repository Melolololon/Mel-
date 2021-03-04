#include"BasicShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	float3 getLight = normalize(mul(lightMat,light));
	//float3 getLight = normalize(light);
	float diffuse = saturate(dot(-getLight, input.normal));//�g�U���ˌ�
	float brightenss = (diffuse + 0.3f);//�P�x

	float4 returnColor = float4(float4(float3(tex.Sample(smp, input.uv).rgb), tex.Sample(smp, input.uv).a) + addColor - subColor)*mulColor;
	returnColor.rgb *= brightenss * lightColor;
	return returnColor;
	//return float4(cameraPos.x, 0, 0, 1);
	//return float4(light.x, 0, 0, 1);

	//���ꂾ�Ƃ��������Ȃ�B���ӁB
	//float4 a = float4(float3(tex.Sample(smp, input.uv).rgb)  * brightenss, tex.Sample(smp, input.uv).a);
	//return (a + addColor - subColor)* mulColor;

	//�ڂ����́A�{���̈ʒu���炸�炵��(unput.uv��������)�s�����x�𗎂Ƃ����f�[�^��p�ӂ��āA�g�ݍ��킹��?
	//return float4(float3(tex.Sample(smp, input.uv).rgb), 1);
	
	
}