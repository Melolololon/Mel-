#include"ObjShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	float4 shaderColor;


	const float shininess = 4.0f;//光沢度

	float3 lightDir = normalize(mul(lightMat, light));

	//カメラ座標ちゃんと遅れてない?
	//なぜか突然正常に送れるようになった(cameraPosYがXに入ってたけどなぜか不明)
	//なぜか端に行くと明るさが変わるしかも片方だけ
	//eyeDirがfloat3じゃなくてfloatになってた
	//GPUにはfloat4型しか送れないのにfloat3にしてたから
	const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	//const float eyeDir = normalize(float3(0,11,-6) - input.worldPos.xyz);


	float3 dotlightnormal = dot(-lightDir, input.normal);
	float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;

	//ここがおかしい?
	float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;


	shaderColor.rgb = (ambient + diffuse + specular) * lightColor;
	shaderColor.a = m_alpha;

	texColor += addColor - subColor;
	texColor *= mulColor;

	return shaderColor * texColor;

}