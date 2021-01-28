#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Mesh.h"
#include "../../Math/Vector3D.h"
#include "../../Collision/CollisionPrimitive.h"

#define num_rows 32 //�c����
#define num_cols 32 //������

#define cell_w 1
#define cell_h 1

#define num_vertsX (num_cols + 1)
#define num_vertsZ (num_rows + 1)

class Primitive3D
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;

public: // �ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	static Primitive3D* CreateFromShape(const Plane& plane);

	static Primitive3D* CreateFromShape(const Sphere& sphere);

	static Primitive3D* CreateFromShape(const Triangle& triangle);

	static Primitive3D* CreateFromShape(const Lay& lay);

	static Primitive3D* CreateFromShape(const Cube& cube);
public:

	~Primitive3D();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">���ߔ��s��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�

	// ���b�V���R���e�i
	std::vector<Mesh*> meshes;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;

private: // �����o�֐�
/// <summary>
/// �}�e���A���ǂݍ���
/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �}�e���A���o�^
	/// </summary>
	void AddMaterial(Material* material);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̐���
	/// </summary>
	void CreateDescriptorHeap();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTextures();

	void LineInitialize();

	void CubeInitialize(const Cube& cube);

	void SphereInitialize(const Sphere& sphere);

	void PlaneInitialize(const Plane& plane);

	void TriangleInitialize(const Triangle& triangle);

	void LayInitialize(const Lay& lay);
};
