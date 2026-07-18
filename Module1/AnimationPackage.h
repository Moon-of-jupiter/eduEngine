#ifndef AnimationPackage_hpp
#define AnimationPackage_hpp
#pragma once

#include <entt/fwd.hpp>
#include "RenderableMesh.hpp"
#include "ForwardRenderer.hpp"
#include "ShapeRenderer.hpp"


struct RenderableMesh_Component {
	std::shared_ptr<eeng::RenderableMesh> _renderable_mesh;
	eeng::AABB _tight_geometry;
};

struct Animation_Component {
	int baseAnimation = 0;
	int secondaryAnimation = 0;

	bool useLayering = false;
	
	float blendFactor = 0.0f;
	eeng::AnimationBranchDesc filter;

	float time0 = 0;
	float time1 = 0;
};

struct Animation_Component_New {
private:
	int _baseAnimation;
	int _secondaryAnimation;

	bool _useLayering;

	float _blendFactor;
	eeng::AnimationBranchDesc _filter;

	float _time0 = 0;
	float _time1 = 0;

public:
	Animation_Component_New
	(
		int baseAnimation		= 0,
		int secondaryAnimation	= 0,

		bool useLayering		= true,
		float blendFactor		= 0.0f,

		eeng::AnimationBranchDesc filter,

		float startingTime_0	= 0,
		float startingTime_1	= 0

	): 
		_baseAnimation(baseAnimation),
		_secondaryAnimation(secondaryAnimation),

		_useLayering(useLayering),
		_blendFactor(blendFactor),

		_filter(filter),

		_time0(startingTime_0),
		_time1(startingTime_1)
	{}


	// animation
	auto BaseAnimation()			  ->		int&	{ return _baseAnimation; }
	auto BaseAnimation()		const -> const	int&	{ return _baseAnimation; }

	auto SecondaryAnimation()		  ->		int&	{ return _secondaryAnimation; }
	auto SecondaryAnimation()	const -> const	int&	{ return _secondaryAnimation; }

	// other!!
};

struct Animation_Basic_Component {
	float speedMult0 = 1.0f;
	float speedMult1 = 1.0f;
};

struct BlendSpaceElement {
	int animation = -1;
	float value = 0;
	float speedMult = 1;
	float time = 0;
	
};

struct Animation_1dBlendSpace_Component {
	std::vector<BlendSpaceElement> blendSpaceElements;
	float value = 0;
};





namespace Animation_Systems {
	void render_System(std::shared_ptr<entt::registry> entity_registry, eeng::ForwardRendererPtr forwardRenderer);


	void animation_Basic_System(std::shared_ptr<entt::registry> entity_registry, float dT);

	void animation_1DBlendSpace_System(std::shared_ptr<entt::registry> entity_registry, float dT);

	void debug_mesh_bones_System(std::shared_ptr<entt::registry> entity_registry, ShapeRendererPtr shapeRenderer);



};

namespace AnimationHelpers {
	void drawMeshBones_Debug(const std::shared_ptr<eeng::RenderableMesh> mesh, const glm::mat4& localToWorld, ShapeRendererPtr shapeRenderer, const float boneScale);

	//void IK_Simple(const std::shared_ptr<eeng::RenderableMesh> mesh);

};


#endif

