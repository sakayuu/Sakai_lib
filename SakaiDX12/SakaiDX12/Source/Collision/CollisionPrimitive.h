#pragma once
/// <summary>
/// 当たり判定プリミティブ
/// </summary>

#include <DirectXMath.h>

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	// 中心座標
	DirectX::XMVECTOR center = {};
	// 半径
	float radius = 1.0f;
};

/// <summary>
/// 平面
/// </summary>
struct Plane
{
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

/// <summary>
/// 法線付き三角形（時計回りが表面）
/// </summary>
class Triangle
{
public:
	// 頂点座標3つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	// 法線ベクトル
	DirectX::XMVECTOR normal;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Lay
{
	// 視点座標
	DirectX::XMVECTOR start = { 0,0,0,1 };
	// 方向
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

struct Cube
{
	// サイズ1x1x1のボックス
	int box_numVertices = 8;
	int box_numIndices = 36;
	int box_vertices[24] = {
	-1,-1, 1,
	 1,-1, 1,
	-1, 1, 1,
	 1, 1, 1,
	-1, 1,-1,
	 1, 1,-1,
	-1,-1,-1,
	 1,-1,-1
	};
	unsigned short box_indices[36] = {
	0,1,2,2,1,3,2,3,4,4,3,5,4,5,6,6,5,7,6,7,0,0,7,1,1,7,3,3,7,5,6,0,4,4,0,2
	};
};