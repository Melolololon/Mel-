#include"PostEffectTestShaderHeader.hlsli"


Texture2D<float4> tex:register(t0);
Texture2D<float4> tex2:register(t1);
Texture2D<float4> tex3:register(t2);
Texture2D<float4> tex4:register(t3);
Texture2D<float4> tex5:register(t4);
Texture2D<float4> tex6:register(t5);

SamplerState smp:register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
	//Œ»İƒtƒŒ[ƒ€
	float4 texColor1 = tex.Sample(smp, input.uv);
	float4 texColor2 = tex2.Sample(smp, input.uv);
	


	float4 texColor = texColor1;
	float4 returnColor = (texColor + color + addColor - subColor) * mulColor;

	if(fmod(input.uv.y,0.1f) < 0.05f)
	{
		texColor = texColor2;
		returnColor = (texColor + color + addColor - subColor) * mulColor;
	
		//‚Ú‚©‚µ
	}
	else
	{
		//F”½“]
		//returnColor = float4(float3(1, 1, 1) - texColor.rgb, texColor.a);
	}



	return returnColor;
}