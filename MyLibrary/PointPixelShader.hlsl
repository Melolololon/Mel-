#include"PointShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);

float4 PSmain(GSOutput input) : SV_TARGET
{
	return float4(tex.Sample(smp, input.uv) * input.color) ;
	//return float4(input.svpos.x,0,input.svpos.y,1);
}