#pragma once
#include "Object3d.h"

class Obj3DManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �����o�֐�
	Obj3DManager();
	~Obj3DManager();

	void Update();

	void Draw();

	void CreateLight(UINT num);

	void SetLightColor(UINT num, XMFLOAT3 color) { light[num]->SetLightColor(color); }

	void SetLightDir(UINT num, XMVECTOR dir) { light[num]->SetLightDir(dir); }

	void LightUpdate();

	// ���f���̒ǉ�
	void AddModel(const std::string& modelName, const std::string& modelPath, bool smoothing);

	// ���f���̃Z�b�g
	void SetModel(const std::string& name, const std::string& modelName);

	// ���f���̍폜
	void DeleteModel(const std::string& modelName);

	// �I�u�W�F�N�g�̒ǉ�
	void AddObj3D(const std::string& name, const std::string& modelName);

	// �I�u�W�F�N�g�̍폜
	void DeleteObj3D(const std::string& name);

public: // �Q�b�^�[
	// ���W�̎擾
	const XMFLOAT3& GetPosition(const std::string& name) { return objects[name]->GetPosition(); }

	// ��]�p�̎擾
	const XMFLOAT3& GetRotation(const std::string& name) { return objects[name]->GetRotation(); }

public: // �Z�b�^�[
	// ���W�̐ݒ�
	void SetPosition(const std::string& name, XMFLOAT3 position) { objects[name]->SetPosition(position); }

	// ��]�p�̐ݒ�
	void SetRotation(const std::string& name, XMFLOAT3 rotation) { objects[name]->SetRotation(rotation); }

	// �X�P�[���̐ݒ�
	void SetScale(const std::string& name, XMFLOAT3 scale) { objects[name]->SetScale(scale); }

	// �r���{�[�h�t���O�̃Z�b�g
	void SetBillboard(const std::string& name, bool isBillboard) { objects[name]->SetBillboard(isBillboard); }

private:
	std::vector<Light*> light;
	std::unordered_map <std::string, Model*> models;
	std::unordered_map <std::string, Object3d*> objects;

};
