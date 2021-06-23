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
	//現在フレーム
	float4 texColor1 = tex.Sample(smp, input.uv);
	float4 texColor2 = tex2.Sample(smp, input.uv);
	


	float4 texColor = texColor1;


	if(fmod(input.uv.y,0.1f) < 0.05f)
	{
		////ぼかし
		//float4 sum = float4(0, 0, 0, 0);
		//float num = 0.0025f;
		//sum += tex2.Sample(smp, float2(input.uv.x - num, input.uv.y + num));
		//sum += tex2.Sample(smp, float2(input.uv.x - num, input.uv.y));
		//sum += tex2.Sample(smp, float2(input.uv.x - num, input.uv.y - num));

		//sum += tex2.Sample(smp, float2(input.uv.x, input.uv.y + num));
		//sum += tex2.Sample(smp, float2(input.uv.x, input.uv.y));
		//sum += tex2.Sample(smp, float2(input.uv.x, input.uv.y - num));

		//sum += tex2.Sample(smp, float2(input.uv.x + num, input.uv.y + num));
		//sum += tex2.Sample(smp, float2(input.uv.x + num, input.uv.y));
		//sum += tex2.Sample(smp, float2(input.uv.x + num, input.uv.y - num));

		//sum /= 9;
		//texColor = sum;

		//移動量
		float uShift = 1.0f / 1280.0f;
		float vShift = 1.0f / 720.0f;
		float4 sum = float4(0, 0, 0, 0);

		//範囲(1で隣1ピクセル)
		int areaX = 2;
		int areaY = 2;

		//sumに加算
		for (int y = -areaY; y < areaY; y++)
		{
			for (int x = -areaX; x < areaX; x++)
			{
				sum += tex2.Sample(smp, float2(input.uv.x + (uShift * x), input.uv.y + (vShift * y)));
			}
		}

		//メモ
		//squaresNumXを1にした時に、中心からかいしする
		//もう資料通りでもいい?


		sum /= areaX * areaY;
		texColor = float4(sum.r, sum.g, sum.b, texColor.a);
	}
	else
	{
		//色反転
		texColor = float4(float3(1, 1, 1) - texColor.rgb, texColor.a);
	}



	return texColor;
}