#include"NotShadowShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	return float4(float4(float3(tex.Sample(smp, input.uv).rgb) , tex.Sample(smp, input.uv).a) + addColor - subColor)*mulColor;
}