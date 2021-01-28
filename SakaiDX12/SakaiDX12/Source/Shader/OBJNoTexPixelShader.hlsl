#include "OBJNoTexShaderHeader.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	float3 light = normalize(float3(1,-1,1)); // 右下奥　向きのライト
	float light_diffuse = saturate(dot(-light, input.normal));
	float3 shade_color;
	shade_color = m_ambient; // アンビエント項
	shade_color += m_diffuse * light_diffuse;	// ディフューズ項
	//return float4(shade_color, m_alpha);
	float3 wireLine = { 0,1,0 };
	return float4(wireLine, m_alpha);
}