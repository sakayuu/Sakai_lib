#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//法線にワールド行列によるスケーリング・回転を適用
	float4 wNormal = normalize(mul(world, float4(normal, 0)));
	float4 wPos = mul(world, pos);

	//ピクセルシェーダーに渡す値
	VSOutput output;
	output.svPos = mul(mul(viewproj, world), pos);
	output.worldPos = wPos;
	output.normal = wNormal.xyz;
	output.uv = uv;

	return output;
}