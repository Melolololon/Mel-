#include"BasicShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	float3 getLight = normalize(mul(lightMat,light));
	float diffuse = saturate(dot(-getLight, input.normal));//拡散反射光
	float brightenss = (diffuse + 0.3f);//輝度
	//return float4(1, 1, 1, 1);
	//グレースケール
	//独自
	float4 asmColor = (addColor - subColor)* mulColor;
	float gray = (tex.Sample(smp, input.uv).r + asmColor.x) + (tex.Sample(smp, input.uv).g + asmColor.y) + (tex.Sample(smp, input.uv).b + asmColor.z);
	gray /= 3;
	return float4(float3(gray, gray, gray) * brightenss, tex.Sample(smp, input.uv).a + asmColor.a);
	//魔導書
	//float Y = dot(tex.Sample(smp, input.uv).rgb,float3(0.299,0.587,0.114));
	//return float4(Y, Y, Y, tex.Sample(smp, input.uv).a);


}