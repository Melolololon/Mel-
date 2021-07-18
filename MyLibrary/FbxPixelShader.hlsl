#include"FbxShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);


static const float PI = 3.141592654f;
static float3 N;

float3 BRDF(float3 L,float3 V)
{
	//�@���ƃ��C�g�̓���
	float NdotL = dot(N, L);
	//�@���ƃJ���������̓���
	float NdotV = dot(N, V);

	//�ǂ��炩��90�x�ȏゾ������^����
	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }
	/*else{ return float3(0, 0, 1); }*/

	//�g�U���˗�
	float diffuseReflectance = 1.0f / PI;
	//�g�U���ˍ�
	float3 diffuseColor = diffuseReflectance * NdotL * baseColor * (1 - metalness);


	//�����ɋ��ʔ��˂̎�


	return diffuseColor;
}

float4 main(GSOutput input) : SV_TARGET
{
	//float4 texColor = tex.Sample(smp, input.uv);
	//float4 shaderColor;


	//const float shininess = 4.0f;//����x

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