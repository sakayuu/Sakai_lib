#include "Primitive3D.h"
using namespace DirectX;

using namespace std;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* Primitive3D::device = nullptr;
UINT Primitive3D::descriptorHandleIncrementSize = 0;

void Primitive3D::StaticInitialize(ID3D12Device* device)
{
	// �ď������`�F�b�N
	assert(!Primitive3D::device);

	Primitive3D::device = device;

	// ���b�V���̐ÓI������
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
	// �f�X�N���v�^�q�[�v�̔z��
	if (descHeap) {
		ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	}

	// �S���b�V����`��
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
	// ���b�V������
	Mesh* mesh = new Mesh;

	// ���b�V���ɖ��O���Z�b�g
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

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes) {
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	// ���b�V���̃o�b�t�@����
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials) {
		m.second->Update();
	}
}

void Primitive3D::SphereInitialize(const Sphere& sphere)
{
	// ���b�V������
	Mesh* mesh = new Mesh;

	// ���b�V���ɖ��O���Z�b�g
	mesh->SetName("Sphere");
	// ���̔��a
	float radius = sphere.radius;
	UINT vertical = 12;             //�ܓx����
	UINT horizontal = vertical * 2; //�o�x����

	//���_�f�[�^�쐬
	for (UINT i = 0; i <= vertical; i++) {
		//�ܓx
		float latitude = (i * XM_PI / vertical) - XM_PIDIV2; //���炭�����ŋ��߂Ă�i�l/�S�̐��j
		float dy = sin(latitude);
		float dxz = cos(latitude);
		for (UINT j = 0; j <= horizontal; j++) {
			//�o�x
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

	//���b�V�����i�C���f�b�N�X�j
	vector<unsigned short> indices;
	UINT stride = horizontal + 1;

	//�C���f�b�N�X�f�[�^�쐬
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

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes) {
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	// ���b�V���̃o�b�t�@����
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials) {
		m.second->Update();
	}

	// �f�X�N���v�^�q�[�v����
	//CreateDescriptorHeap();

	// �e�N�X�`���̓ǂݍ���
	//LoadTextures();
}

void Primitive3D::PlaneInitialize(const Plane& plane)
{
	// ���b�V������
	Mesh* mesh = new Mesh;

	// ���b�V���ɖ��O���Z�b�g
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

	//���b�V�����i�C���f�b�N�X�j
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

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes) {
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	// ���b�V���̃o�b�t�@����
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials) {
		m.second->Update();
	}
}

void Primitive3D::TriangleInitialize(const Triangle& triangle)
{
	// ���b�V������
	Mesh* mesh = new Mesh;

	// ���b�V���ɖ��O���Z�b�g
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

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes) {
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	// ���b�V���̃o�b�t�@����
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials) {
		m.second->Update();
	}

}

void Primitive3D::LayInitialize(const Lay& lay)
{
	// ���b�V������
	Mesh* mesh = new Mesh;

	// ���b�V���ɖ��O���Z�b�g
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

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes) {
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	// ���b�V���̃o�b�t�@����
	for (auto& m : meshes) {
		m->CreateBuffers();
	}
	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials) {
		m.second->Update();
	}
}
