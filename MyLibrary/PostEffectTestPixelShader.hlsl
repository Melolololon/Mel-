#include"PostEffectTestShaderHeader.hlsli"


Texture2D<float4> tex:register(t0);
Texture2D<float4> tex2:register(t1);
Texture2D<float4> tex3:register(t2);
Texture2D<float4> tex4:register(t3);

SamplerState smp:register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
	//現在フレーム
	float4 texColor1 = tex.Sample(smp, input.uv);
	float4 texColor2 = tex2.Sample(smp, input.uv);
	
	//過去フレーム
	float4 texColor3 = tex3.Sample(smp, input.uv);
	float4 texColor4 = tex4.Sample(smp, input.uv);

	float4 blurSubColor = float4(0.0f, 0.0f, 0.0f, 0.5f);

	float4 texColor = texColor1 + saturate(texColor3 - blurSubColor);
	float4 returnColor = saturate(texColor + color + addColor - subColor) * mulColor;

	if(fmod(input.uv.y,0.1f) < 0.05f)
	{
		texColor = texColor2 + saturate(texColor4 - blurSubColor);
		returnColor = saturate(texColor + color + addColor - subColor) * mulColor;
		//ぼかし

	}
	else
	{
		//色反転
		//returnColor = float4(float3(1, 1, 1) - returnColor.rgb, returnColor.a);
	}


	return returnColor;
}