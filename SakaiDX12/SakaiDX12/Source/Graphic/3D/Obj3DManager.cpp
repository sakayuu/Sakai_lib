#include "Obj3DManager.h"


using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

Obj3DManager::Obj3DManager()
{
}

Obj3DManager::~Obj3DManager()
{
	for (auto& m : models) {
		//models.erase(m);
		delete(m.second);
		m.second = nullptr;
	}
	models.clear();

	for (auto& o : objects) {
		delete(o.second);
		o.second = nullptr;
	}
	objects.clear();

}

void Obj3DManager::Update()
{
	LightUpdate();

	for (auto& o : objects)
		o.second->Update();
}

void Obj3DManager::Draw()
{
	for (auto& o : objects)
		o.second->Draw(light);
}

void Obj3DManager::CreateLight(UINT num)
{
	if (num <= 0)
		return;
	for (int i = 0; i < num; i++) {
		Light* l;
		l = l->Create();
		light.push_back(l);
	}
}

void Obj3DManager::LightUpdate()
{
	if (light.size() <= 0)
		return;
	for (auto& l : light)
		l->Update();
}


void Obj3DManager::AddModel(const std::string& modelName, const std::string& modelPath, bool smoothing)
{
	models.emplace(modelName, Model::CreateFromOBJ(modelPath, smoothing));
}

void Obj3DManager::SetModel(const std::string& name, const std::string& modelName)
{
	if (objects.empty())
		return;

	objects[name]->SetModel(models[modelName]);
}

void Obj3DManager::DeleteModel(const std::string& modelName)
{
	if (models.empty() || models[modelName] == nullptr)
		return;

	delete(models[modelName]);
	models[modelName] = nullptr;
	models.erase(modelName);
}

void Obj3DManager::AddObj3D(const std::string& name, const std::string& modelName)
{
	if (models[modelName] == nullptr)
		return;

	objects.emplace(name, Object3d::Create(models[modelName]));
}

void Obj3DManager::DeleteObj3D(const std::string& name)
{
	if (objects.empty() || objects[name] == nullptr)
		return;

	delete(objects[name]);
	objects[name] = nullptr;
	objects.erase(name);
}

