#ifndef MeshManager_hpp
#define MeshManager_hpp
#pragma once
#include "RenderableMesh.hpp"
#include "map"

namespace my_eeng {
	class MeshManager {
	public:
		~MeshManager();
		void Load(const std::string& id, const std::string& adress);

		void Load(const std::string& id, const std::string& adress, std::vector<std::string> animations);

		void Insert(const std::string& id, std::shared_ptr<eeng::RenderableMesh> mesh);

		void LoadAnimations(const std::string& id, std::vector<std::string> animations);

		std::shared_ptr<eeng::RenderableMesh> Get(const std::string& id);

		void RemoveTranslationKeys(const std::string& id, const std::string& bone);

	private:
		std::map<std::string, std::shared_ptr<eeng::RenderableMesh>> meshes;




	};
}

#endif
