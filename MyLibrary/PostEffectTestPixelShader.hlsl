#include"PostEffectTestShaderHeader.hlsli"


Texture2D<float4> tex:register(t0);
Texture2D<float4> tex2:register(t1);
SamplerState smp:register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	float4 texColor2 = tex2.Sample(smp, input.uv);

	float4 returnColor = texColor;
	if(fmod(input.uv.y,0.1f) < 0.05f)
	{
		returnColor = texColor2;
	}


	return (returnColor + color + addColor - subColor) * mulColor;
}