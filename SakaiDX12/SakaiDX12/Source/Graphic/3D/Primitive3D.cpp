#include "Primitive3D.h"
using namespace DirectX;

using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Primitive3D::device = nullptr;
UINT Primitive3D::descriptorHandleIncrementSize = 0;

void Primitive3D::StaticInitialize(ID3D12Device* device)
{
	// 再初期化チェック
	assert(!Primitive3D::device);

	Primitive3D::device = device;

	// メッシュの静的初期化
	Mesh::StaticInitialize(device);
}

Primitive3D* Primitive3D::CreateFromShape(const Sphere& sphere)
{
	Primitive3D* instance = new Primitive3D;
	instance->SphereInitialize(sphere);

	return instance;
}

Primitive3D* Primitive3D::CreateFromShape(const Triangle& triangle)
{
	Primitive3D* instance = new Primitive3D;
	instance->TriangleInitialize(triangle);

	return instance;
}

Primitive3D* Primitive3D::CreateFromShape(const Lay& lay)
{
	Primitive3D* instance = new Primitive3D;
	instance->LayInitialize(lay);

	return instance;
}

Primitive3D* Primitive3D::CreateFromShape(const Cube& cube)
{
	Primitive3D* instance = new Primitive3D;
	instance->CubeInitialize(cube);

	return instance;
}

Primitive3D* Primitive3D::CreateFromShape(const Plane& plane)
{
	Primitive3D* instance = new Primitive3D;
	instance->PlaneInitialize(plane);

	return instance;
}

Primitive3D::~Primitive3D()
{
	for (auto m : meshes) {
		delete m;
	}
	meshes.clear();

	for (auto m : materials) {
		delete m.second;
	}
	materials.clear();
}

void Primitive3D::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// デスクリプタヒープの配列
	if (descHeap) {
		ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	}

	// 全メッシュを描画
	for (auto& mesh : meshes) {
		mesh->Draw(cmdList);
	}
}

void Primitive3D::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
}

void Primitive3D::AddMaterial(Material* material)
{
}

void Primitive3D::CreateDescriptorHeap()
{

}

void Primitive3D::LoadTextures()
{
}

void Primitive3D::LineInitialize()
{
}

void Primitive3D::CubeInitialize(const Cube& cube)
{
	// メッシュ生成
	Mesh* mesh = new Mesh;

	// メッシュに名前をセット
	mesh->SetName("Cube");

	for (int i = 0; i < (int)cube.box_vertices; i++)
	{
		Mesh::VertexPosNormalUv data;
		data.pos = XMFLOAT3(cube.box_vertices[i * 3], cube.box_vertices[(i * 3) + 1], cube.box_vertices[(i * 3) + 2]);
		data.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		mesh->AddVertex(data);
	}

	for (auto& i : cube.box_indices)
		mesh->AddIndex(i);

	meshes.emplace_back(mesh);

	// メッシュのマテリアルチェック
	for (auto& m : meshes) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials) {
		m.second->Update();
	}
}

void Primitive3D::SphereInitialize(const Sphere& sphere)
{
	// メッシュ生成
	Mesh* mesh = new Mesh;

	// メッシュに名前をセット
	mesh->SetName("Sphere");
	// 球の半径
	float radius = sphere.radius;
	UINT vertical = 12;             //緯度方向
	UINT horizontal = vertical * 2; //経度方向

	//頂点データ作成
	for (UINT i = 0; i <= vertical; i++) {
		//緯度
		float latitude = (i * XM_PI / vertical) - XM_PIDIV2; //恐らく割合で求めてる（値/全体数）
		float dy = sin(latitude);
		float dxz = cos(latitude);
		for (UINT j = 0; j <= horizontal; j++) {
			//経度
			float longitude = j * XM_2PI / horizontal;
			float dx = sin(longitude) * dxz;
			float dz = cos(longitude) * dxz;
			Vector3D norm(dx, dy, dz);
			Mesh::VertexPosNormalUv date;
			date.pos = XMFLOAT3(norm.x * radius, norm.y * radius, norm.z * radius);
			date.normal = XMFLOAT3(norm.Normalized().x, norm.Normalized().y, norm.Normalized().z);
			mesh->AddVertex(date);
		}
	}

	//メッシュ情報（インデックス）
	vector<unsigned short> indices;
	UINT stride = horizontal + 1;

	//インデックスデータ作成
	for (UINT i = 0; i < vertical; i++) {
		for (UINT j = 0; j < horizontal; j++) {
			UINT NextI = i + 1;
			UINT NextJ = (j + 1) % stride;
			indices.push_back((uint16_t)(i * stride + NextJ));
			indices.push_back((uint16_t)(NextI * stride + j));
			indices.push_back((uint16_t)(i * stride + j));

			indices.push_back((uint16_t)(NextI * stride + NextJ));
			indices.push_back((uint16_t)(NextI * stride + j));
			indices.push_back((uint16_t)(i * stride + NextJ));
		}
	}
	for (auto& i : indices)
		mesh->AddIndex(i);

	meshes.emplace_back(mesh);

	// メッシュのマテリアルチェック
	for (auto& m : meshes) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials) {
		m.second->Update();
	}

	// デスクリプタヒープ生成
	//CreateDescriptorHeap();

	// テクスチャの読み込み
	//LoadTextures();
}

void Primitive3D::PlaneInitialize(const Plane& plane)
{
	// メッシュ生成
	Mesh* mesh = new Mesh;

	// メッシュに名前をセット
	mesh->SetName("Plane");

	Vector3D pos(plane.normal.m128_f32[0], plane.normal.m128_f32[1], plane.normal.m128_f32[2]);
	pos *= plane.distance;

	for (UINT z = 0; z < num_vertsZ; ++z) {
		for (UINT x = 0; x < num_vertsX; ++x) {
			Mesh::VertexPosNormalUv date;
			date.pos.x = (float)x * cell_w;
			date.pos.z = (float)z * cell_h;
			date.pos.y = pos.y;

			Vector3D norm(date.pos.x, date.pos.y, date.pos.z);
			date.normal = XMFLOAT3(norm.Normalized().x, norm.Normalized().y, norm.Normalized().z);
			mesh->AddVertex(date);
		}
	}

	//メッシュ情報（インデックス）
	unsigned short indices[num_vertsX * num_vertsZ * 6];
	UINT curIndex = 0;

	for (UINT z = 0; z < num_rows; z++) {
		for (UINT x = 0; x < num_cols; x++) {
			UINT curVertex = x + (z * num_vertsX);

			indices[curIndex] = curVertex;
			indices[curIndex + 1] = curVertex + num_vertsX;
			indices[curIndex + 2] = curVertex + 1;

			indices[curIndex + 3] = curVertex + 1;
			indices[curIndex + 4] = curVertex + num_vertsX;
			indices[curIndex + 5] = curVertex + num_vertsX + 1;

			curIndex += 6;
		}
	}
	for (auto& i : indices)
		mesh->AddIndex(i);

	meshes.emplace_back(mesh);

	// メッシュのマテリアルチェック
	for (auto& m : meshes) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials) {
		m.second->Update();
	}
}

void Primitive3D::TriangleInitialize(const Triangle& triangle)
{
	// メッシュ生成
	Mesh* mesh = new Mesh;

	// メッシュに名前をセット
	mesh->SetName("Triangle");

	Mesh::VertexPosNormalUv date;
	date.pos = XMFLOAT3(triangle.p0.m128_f32[0], triangle.p0.m128_f32[1], triangle.p0.m128_f32[2]);
	date.normal = XMFLOAT3(triangle.normal.m128_f32[0], triangle.normal.m128_f32[1], triangle.normal.m128_f32[2]);
	mesh->AddVertex(date);
	date.pos = XMFLOAT3(triangle.p1.m128_f32[0], triangle.p1.m128_f32[1], triangle.p1.m128_f32[2]);
	date.normal = XMFLOAT3(triangle.normal.m128_f32[0], triangle.normal.m128_f32[1], triangle.normal.m128_f32[2]);
	mesh->AddVertex(date);
	date.pos = XMFLOAT3(triangle.p2.m128_f32[0], triangle.p2.m128_f32[1], triangle.p2.m128_f32[2]);
	date.normal = XMFLOAT3(triangle.normal.m128_f32[0], triangle.normal.m128_f32[1], triangle.normal.m128_f32[2]);
	mesh->AddVertex(date);

	mesh->AddIndex(0);
	mesh->AddIndex(1);
	mesh->AddIndex(2);

	meshes.emplace_back(mesh);

	// メッシュのマテリアルチェック
	for (auto& m : meshes) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials) {
		m.second->Update();
	}

}

void Primitive3D::LayInitialize(const Lay& lay)
{
	// メッシュ生成
	Mesh* mesh = new Mesh;

	// メッシュに名前をセット
	mesh->SetName("Lay");

	Mesh::VertexPosNormal date;

	//date.pos = XMFLOAT3(lay.start.m128_f32[0], lay.start.m128_f32[1], lay.start.m128_f32[2]);
	date.pos = XMFLOAT3(0, 0, 0);
	auto norm = XMVector3Normalize(lay.start);
	date.normal = XMFLOAT3(norm.m128_f32[0], norm.m128_f32[1], norm.m128_f32[2]);
	mesh->AddVertex(date);

	Vector3D dir(lay.dir.m128_f32[0], lay.dir.m128_f32[1], lay.dir.m128_f32[2]);
	dir = dir.Normalized();
	date.pos = XMFLOAT3(lay.start.m128_f32[0] + dir.x * 10, lay.start.m128_f32[1] + dir.y * 10, lay.start.m128_f32[2] + dir.z * 10);
	norm = XMVector3Normalize(lay.dir);
	date.normal = XMFLOAT3(norm.m128_f32[0], norm.m128_f32[1], norm.m128_f32[2]);
	mesh->AddVertex(date);

	mesh->AddIndex(0);
	mesh->AddIndex(1);
	mesh->AddIndex(0);

	meshes.emplace_back(mesh);

	// メッシュのマテリアルチェック
	for (auto& m : meshes) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial);
		}
	}

	// メッシュのバッファ生成
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials) {
		m.second->Update();
	}
}
