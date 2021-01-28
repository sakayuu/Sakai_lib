#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Mesh.h"
#include "../../Math/Vector3D.h"
#include "../../Collision/CollisionPrimitive.h"

#define num_rows 32 //縦方向
#define num_cols 32 //横方向

#define cell_w 1
#define cell_h 1

#define num_vertsX (num_cols + 1)
#define num_vertsZ (num_rows + 1)

class Primitive3D
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	static Primitive3D* CreateFromShape(const Plane& plane);

	static Primitive3D* CreateFromShape(const Sphere& sphere);

	static Primitive3D* CreateFromShape(const Triangle& triangle);

	static Primitive3D* CreateFromShape(const Lay& lay);

	static Primitive3D* CreateFromShape(const Cube& cube);
public:

	~Primitive3D();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">命令発行先コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数

	// メッシュコンテナ
	std::vector<Mesh*> meshes;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials;
	// デフォルトマテリアル
	Material* defaultMaterial = nullptr;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;

private: // メンバ関数
/// <summary>
/// マテリアル読み込み
/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// マテリアル登録
	/// </summary>
	void AddMaterial(Material* material);

	/// <summary>
	/// デスクリプタヒープの生成
	/// </summary>
	void CreateDescriptorHeap();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTextures();

	void LineInitialize();

	void CubeInitialize(const Cube& cube);

	void SphereInitialize(const Sphere& sphere);

	void PlaneInitialize(const Plane& plane);

	void TriangleInitialize(const Triangle& triangle);

	void LayInitialize(const Lay& lay);
};
