#include"PostEffectTestShaderHeader.hlsli"


Texture2D<float4> tex:register(t0);
Texture2D<float4> tex2:register(t1);
SamplerState smp:register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	float4 texColor2 = tex2.Sample(smp, input.uv);

	float4 retColor = texColor;
	if(fmod(input.uv.y,0.1f) < 0.05f)
	{
		retColor = texColor2;
	}


	return (retColor + color + addColor - subColor) * mulColor;

	//return float4(tex.Sample(smp, input.uv) + color + addColor - subColor) * mulColor;
}