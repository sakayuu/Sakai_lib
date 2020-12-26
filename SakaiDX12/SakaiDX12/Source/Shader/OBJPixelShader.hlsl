#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float step(float a, float x)
{
	return (float)(x >= a) ? 1 : 0;
}

float smoothstep(int a, float b, float x)
{
	float t = saturate((x - a) / (b - a));
	return t * t * (3.0 - (2.0 * t));
}

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャマッピング
	float4 texColor = tex.Sample(smp,input.uv);

	//シェーディングによる色
	float4 shadeColor;

	//頂点から視点への方向ベクトル
	float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);

	///Light1
	//光沢度
	const float shininess = 15.0f;
	//ライトに向かうベクトルと法線の内積
	float dotLightNormal = dot(lightV, input.normal.xyz);
	//反射光ベクトル
	float3 reflect = normalize(-lightV + 2 * dotLightNormal * input.normal.xyz);
	///Light2
	const float shininess2 = 15.0f;
	float dotLightNormal2 = dot(lightV2, input.normal.xyz);
	float3 reflect2 = normalize(-lightV2 + 2 * dotLightNormal2 * input.normal.xyz);

	//環境反射光
	float3 ambient = m_ambient;
	//拡散反射光
	float3 diffuse = dotLightNormal * m_diffuse;
	float3 diffuse2 = dotLightNormal2 * m_diffuse;
	//鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
	float3 specular2 = pow(saturate(dot(reflect2, eyeDir)), shininess2) * m_specular;
	//トゥーン
	float intensity = smoothstep(0, 0.01f, dotLightNormal);
	float intensity2 = smoothstep(0, 0.01f, dotLightNormal2);
	float3 shadow = (0.2f, 0, 0) * (1 - intensity);
	float3 shadow2 = (0, 0, 0.2f) * (1 - intensity2);
	diffuse = intensity * m_diffuse + shadow;
	diffuse2 = intensity2 * m_diffuse + shadow2;
	float light = pow(dot(reflect, eyeDir), shininess);
	float light2 = pow(dot(reflect2, eyeDir), shininess2);
	float3 halfVec = normalize(lightV + eyeDir);
	float3 halfVec2 = normalize(lightV2 + eyeDir);
	//light = smoothstep(1, 2, light + dot(halfVec, input.normal.xyz));
	light = step(0.95f, light + dot(halfVec, input.normal.xyz));
	light = saturate(light);
	//light2 = smoothstep(1, 2, light2 + dot(halfVec2, input.normal.xyz));
	light2 = step(0.95f,light2 + dot(halfVec2, input.normal.xyz));
	light2 = saturate(light2);
	specular = light * m_specular;
	specular2 = light2 * m_specular;

	//全て加算する
	shadeColor.rgb = (ambient + lightColor * (diffuse + specular) + lightColor2 * (diffuse2 + specular2));
	//shadeColor.rgb = diffuse * lightColor;
	//shadeColor.rgb = specular * lightColor;
	shadeColor.a = m_alpha;

	//シェーディングによる色で描画
	return shadeColor * texColor;
}