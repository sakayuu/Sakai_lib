#pragma once
#include <DirectXMath.h>
#include <d3dx12.h>

class Light
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB2
	{
		XMVECTOR lightV;     //���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightColor; //���C�g�̐F
	};

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Light* Create();

private: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���C�g���������i�P�ʃx�N�g���j
	XMVECTOR lightDir = { 1,0,0,0 };
	// ���C�g�F
	XMFLOAT3 lightColor = { 1,1,1 };
	// �_�[�e�B�t���O
	bool dirty = false;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ���C�g�������Z�b�g
	/// </summary>
	/// <param name="lightDir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightDir);

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	/// <param name="lightColor">���C�g�F</param>
	void SetLightColor(const XMFLOAT3& lightColor);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootParameterIndex">���[�g�p�����[�^</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
};
