#include "MeshManager.h"

namespace my_eeng {

	MeshManager::~MeshManager() {
		meshes.clear();
	}

	void MeshManager::Load(const std::string& id, const std::string& adress) {
		auto mesh = std::make_shared<eeng::RenderableMesh>();
		mesh->load(adress, false);
		Insert(id, mesh);
	}

	void MeshManager::Load(const std::string& id, const std::string& adress, std::vector<std::string> animations) {
		Load(id, adress);
		LoadAnimations(id, animations);
	}

	void MeshManager::Insert(const std::string& id, std::shared_ptr<eeng::RenderableMesh> mesh) {
		meshes[id] = mesh;
	}

	void  MeshManager::LoadAnimations(const std::string& id, std::vector<std::string> animations) {
		auto mesh = meshes[id];
		if (!mesh)
			return;

		for (auto animation : animations) {
			mesh->load(animation, true);
		}

	}

	void MeshManager::RemoveTranslationKeys(const std::string& id, const std::string& bone) {
		Get(id)->removeTranslationKeys(bone);
	}




	std::shared_ptr<eeng::RenderableMesh> MeshManager::Get(const std::string& id) {
		return meshes[id];
	}


}