#include"FbxShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);


static const float PI = 3.141592654f;
static float3 N;

float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

float3 BRDF(float3 L,float3 V)
{
	//–@ü‚Æƒ‰ƒCƒg‚Ì“àÏ
	float NdotL = dot(N, L);
	//–@ü‚ÆƒJƒƒ‰•ûŒü‚Ì“àÏ
	float NdotV = dot(N, V);

	//‚Ç‚¿‚ç‚©‚ª90“xˆÈã‚¾‚Á‚½‚ç^‚Á•
	if (NdotL < 0 || NdotV < 0) 
	{
		return float3(0, 0, 0); 
	}

	//ƒn[ƒtƒxƒNƒgƒ‹
	float3 H = normalize(L + V);
	float NdotH = dot(N, H);
	float LdotH = dot(L, H);
	
	//ŠgU”½Ë—¦
	float diffuseReflectance = 1.0f / PI;

	float energyBias = 0.5f * roughness;
	//“üËŠp‚ª90“x‚Ìê‡‚ÌŠgU”½Ë—¦
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * roughness;
	//“ü‚Á‚Ä‚¢‚­‚ÌŠgU”½Ë—¦
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//o‚Ä‚¢‚­‚Æ‚«‚ÌŠgU”½Ë—¦
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
	//“ü‚Á‚Äo‚Ä‚¢‚­‚Ü‚Å‚ÌŠgU”½Ë—¦
	float Fd = FL * FV * energyFactor;

	//ŠgU”½Ë€
	float3 diffuseColor = diffuseReflectance * Fd * baseColor * (1 - metalness);


	//‚±‚±‚É‹¾–Ê”½Ë‚Ì®


	return diffuseColor;
}


float4 main(GSOutput input) : SV_TARGET
{
	//float4 texColor = tex.Sample(smp, input.uv);
	//float4 shaderColor;


	//const float shininess = 4.0f;//Œõ‘ò“x

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