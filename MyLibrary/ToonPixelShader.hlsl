#include"ToonShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);
float4 PSmain(VSOutput input) : SV_TARGET
{
	float egge = 0.0015f;
float alpha = tex.Sample(smp, input.uv).a;
	float2 uv = input.uv;
	float4 retColor = float4(0, 0, 0, 0);
	retColor += tex.Sample(smp, uv + float2(0,egge)) * -1;
	retColor += tex.Sample(smp, uv + float2(0, -egge)) * -1;
	retColor += tex.Sample(smp, uv) * 4;
	retColor += tex.Sample(smp, uv + float2(egge , 0)) * -1;
	retColor += tex.Sample(smp, uv + float2(-egge,0)) * -1;

	float grayColor = retColor.r + retColor.g + retColor.b;
	float rgb = pow(1.0f - grayColor, 10.0f);
	rgb = step(0.2, rgb);
	retColor.rgb = float3(rgb, rgb, rgb);
	if (retColor.r == 1 && retColor.g == 1 && retColor.b == 1)alpha = 0;

	/*if (retColor.r == 0 && retColor.g == 0 && retColor.b == 0)alpha = 0;
	else retColor.rgb = float3(0, 0, 0);
	retColor = step(0.2, retColor);*/


	return float4(float3(retColor.rgb), alpha);

}