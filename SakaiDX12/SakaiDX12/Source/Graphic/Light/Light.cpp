#include "Light.h"

using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* Light::device = nullptr;

void Light::StaticInitialize(ID3D12Device* device) {
	//�ď������`�F�b�N
	assert(!Light::device);
	//nullptr�`�F�b�N
	assert(device);
	//�ÓI�����o�ϐ��̃Z�b�g
	Light::device = device;
}

Light* Light::Create() {
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Light* instance = new Light();
	// ������
	instance->Initialize();
	// ���������C���X�^���X��Ԃ�
	return instance;
}

void Light::Initialize() {
	HRESULT result;

	// �����Ɠ����Ȃ�
	auto HEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto RESOURCE_DESC_Buffer = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB2) + 0xff) & ~0xff);
	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC_Buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	// �萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void Light::TransferConstBuffer() {
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB2* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightV = -lightDir;
		constMap->lightColor = lightColor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::SetLightDir(const XMVECTOR& lightDir) {
	// ���K�����ăZ�b�g
	this->lightDir = XMVector3Normalize(lightDir);
	dirty = true;
}

void Light::SetLightColor(const XMFLOAT3& lightColor) {
	this->lightColor = lightColor;
	dirty = true;
}

void Light::Update() {
	// �l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) {
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}
