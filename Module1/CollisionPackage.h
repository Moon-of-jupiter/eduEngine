#ifndef CollisionPackage_hpp
#define CollisionPackage_hpp
#pragma once

#include <entt/entt.hpp>
#include "RenderableMesh.hpp"
#include "glmcommon.hpp"
#include "AABB.h"
#include "glmcommon.hpp"
#include "ShapeRenderer.hpp"
#include "EventPackage.h"

#include "DataComponents.h"



namespace CollisionPackage {
	

	struct Sphere {
		glm::vec3 _position{0,0,0};
		float _r{ 0 };

		glm::vec3 GetPosition() const {
			return _position;
		}

		void SetPosition(const glm::vec3 position) {
			_position = position;
		}

		float GetRadius() const {
			return _r;
		}

		void SetRadius(const float radius) {
			_r = radius;
		}

		float x() const {
			return _position.x;
		}
		
		float y() const {
			return _position.y;
		}

		float z() const {
			return _position.z;
		}

		float r() const {
			return _r;
		}
	};

	struct Plane {
		//glm::vec3 normal{0,1,0};
		float distanceToOrigin;

	};

	



	struct Intersection {
		glm::vec3 normal;

		float depth;
	};


	class Collision_Args : public EventP::EventArgs {
	public:
		entt::entity otherCollider;
		Intersection intersection;
	};



	struct PhysicsObject_Component {
		glm::vec3 _acceleration = glm_aux::vec3_000;
	};
	
	

	struct PhysicsCollider_Component {
		bool isTrigger{ false };
		eeng::AABB _localTightGeometry;

		Sphere _broadPhaseCollider;
		Sphere _loseGeometry;
		eeng::AABB _tightGeometry;

		

	};

	struct PlaneCollider_Component {
		Plane _planeColider{ 0 };
	};

	
	void UpdateColliders_System(std::shared_ptr<entt::registry> entity_registry);

	void Gravity_System(std::shared_ptr<entt::registry> entity_registry, glm::vec3 gravity);

	void PhysicsUpdate_System(std::shared_ptr<entt::registry> entity_registry, float deltaTime);


	void PlaneColission_System(std::shared_ptr<entt::registry> entity_registry);

	void DynamicColission_System(std::shared_ptr<entt::registry> entity_registry, EventP::EventQueue& eventDispatcher);

	void DebugColliders_System(std::shared_ptr<entt::registry> entity_registry, ShapeRendererPtr shapeRenderer);


	


	namespace BVH {
		struct SphereNode {
			Sphere* sphere;
			SphereNode* leftChild;
			SphereNode* rightChild;

			void DeleteRecursive() {
				
				
				if (!leftChild && !rightChild) {
					delete sphere;
					return;
					
				}	

				if (leftChild) {
					leftChild->DeleteRecursive();
				}

				if (rightChild) {
					delete sphere;
					rightChild->DeleteRecursive();
				}

				delete this;
			}
		};


		void FindMinMaxSpherePoints(const Sphere* A, const Sphere* B, glm::vec3& minOut, glm::vec3& maxOut);
		float SphereDistance(const Sphere* A, const Sphere* B);

		SphereNode* BuildNodeFromSpheres(Sphere* left, Sphere* right);
		SphereNode* BuildNodeFromSphere(Sphere* sphere);

		
		std::vector<std::pair<SphereNode*, SphereNode*>> FindNodePairs_Greedy(std::vector<SphereNode*> openList, float maxDistance);

		SphereNode* BuildBVHBottomUp(std::vector<Sphere*> spheres, float maxLeafPairDistance);

		std::vector<Sphere*> IntersectBVH(SphereNode* treeRoot,const Sphere& sphere);
	}

	namespace CollisionHelpers {

		void SolveCollision(Transform_Component& transform, LinearVelocity_Component& velocity, Intersection& collision);







		std::vector<glm::ivec2> FindMinMaxValues(const glm::vec3 (&points)[], int pointCount);

		glm::ivec2 FindMostDistantPoints(const std::vector<glm::ivec2> &minMaxPoints, const glm::vec3(&points)[]);

		Sphere BuildSphereFromPoints(const glm::vec3(&points)[], int pointCount);





		bool Sphere_Sphere_Overlap(	const Sphere& A, const Sphere& B);

		//bool Sphere_Plane_Overlap(	const Sphere& A, const Plane&  B);

		bool AABB_AABB_Overlap(	const eeng::AABB& A, const eeng::AABB& B);

		//bool AABB_Plane_Overlap(const eeng::AABB& A, const Plane& B);


		void Sphere_Sphere_Collision(const Sphere& A, const Sphere& B, Intersection& outIntersectionA);



		void AABB_Plane_Collision(const eeng::AABB& A, const Plane& B, Intersection& outIntersectionA);

		
		glm::vec3 AABBHalfWidths(const eeng::AABB& A);

		glm::vec3 AABBCenterPoint(const eeng::AABB& A);

	}
}




#endif

