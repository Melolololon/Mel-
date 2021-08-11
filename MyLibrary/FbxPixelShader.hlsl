#include"FbxShaderHeader.hlsli"
#include"ADSAHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);



float4 main(GSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv) + baseColor;
	float4 shaderColor = float4(0, 0, 0, 0);


	const float shininess = 4.0f;//光沢度


	const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	for (int i = 0; i < DIR_LIGTH_MAX; i++)
	{

		float3 lightDir = light[i];

		float3 dotlightnormal = dot(-lightDir, input.normal);
		float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

		float3 ambient = m_ambient;
		float3 diffuse = dotlightnormal * m_diffuse;
		float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
	

		shaderColor.rgb += (ambient + diffuse + specular) * lightColor[i];
	}
	saturate(shaderColor.rgb);
	shaderColor.a = m_alpha;

	texColor += addColor - subColor;
	texColor *= mulColor;

	//べた塗りテクスチャ使うと単色マテリアル用意するごとにテクスチャバッファ用意しないといけないからこうする。
	float alphaSum = (shaderColor.a + texColor.a) / 2;

	return float4(shaderColor.rgb * texColor.rgb, alphaSum);




	//以下PBRの処理
	/*N = input.normal;
	float3 finalRGB = float3(0, 0, 0);
	float3 eyedir = normalize(cameraPos.xyz - input.worldPos.xyz);

	for (int i = 0; i < 3; i++) 
	{
		finalRGB += BRDF(-light.xyz, eyedir) * lightColor;
	}
	return float4(finalRGB, 1);*/
	
	
}