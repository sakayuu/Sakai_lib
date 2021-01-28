#include "OBJNoTexShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);
	output.normal = normal;
	return output;
}