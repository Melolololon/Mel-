#include"FbxShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);


static const float PI = 3.141592654f;
static float3 N;

float3 BRDF(float3 L,float3 V)
{
	//法線とライトの内積
	float NdotL = dot(N, L);
	//法線とカメラ方向の内積
	float NdotV = dot(N, V);

	//どちらかが90度以上だったら真っ黒
	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }
	/*else{ return float3(0, 0, 1); }*/

	//拡散反射率
	float diffuseReflectance = 1.0f / PI;
	//拡散反射項
	float3 diffuseColor = diffuseReflectance * NdotL * baseColor * (1 - metalness);


	//ここに鏡面反射の式


	return diffuseColor;
}

float4 main(GSOutput input) : SV_TARGET
{
	//float4 texColor = tex.Sample(smp, input.uv);
	//float4 shaderColor;


	//const float shininess = 4.0f;//光沢度

	//float3 lightDir = normalize(mul(lightMat, light));
	//const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);

	//float3 dotlightnormal = dot(-lightDir, input.normal);
	//float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

	//float3 ambient = m_ambient;
	//float3 diffuse = dotlightnormal * m_diffuse;
	//float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;


	//shaderColor.rgb = (ambient + diffuse + specular) * lightColor;
	//shaderColor.a = m_alpha;

	//texColor += addColor - subColor;
	//texColor *= mulColor;

	//return shaderColor * texColor;

	//return shaderColor * float4(baseColor, 1);


	N = input.normal;
	float3 finalRGB = float3(0, 0, 0);
	float3 eyedir = normalize(cameraPos.xyz - input.worldPos.xyz);

	finalRGB += BRDF(-light.xyz, eyedir) * lightColor;

	return float4(finalRGB, 1);
	
	
}