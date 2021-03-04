#include"BasicShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	float3 getLight = normalize(mul(lightMat,light));
	//float3 getLight = normalize(light);
	float diffuse = saturate(dot(-getLight, input.normal));//拡散反射光
	float brightenss = (diffuse + 0.3f);//輝度

	float4 returnColor = float4(float4(float3(tex.Sample(smp, input.uv).rgb), tex.Sample(smp, input.uv).a) + addColor - subColor)*mulColor;
	returnColor.rgb *= brightenss * lightColor;
	return returnColor;
	//return float4(cameraPos.x, 0, 0, 1);
	//return float4(light.x, 0, 0, 1);

	//これだとおかしくなる。注意。
	//float4 a = float4(float3(tex.Sample(smp, input.uv).rgb)  * brightenss, tex.Sample(smp, input.uv).a);
	//return (a + addColor - subColor)* mulColor;

	//ぼかしは、本来の位置からずらして(unput.uvをいじる)不透明度を落としたデータを用意して、組み合わせる?
	//return float4(float3(tex.Sample(smp, input.uv).rgb), 1);
	
	
}