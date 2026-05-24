#include "CollisionPackage.h"
#include "RenderableMesh.hpp"
#include "glmcommon.hpp"
#include "AABB.h"
#include "Log.hpp"


#include "DataComponents.h"
#include "AnimationPackage.h"


namespace CollisionPackage {


	void UpdateColliders_System(std::shared_ptr<entt::registry> entity_registry) {
		auto collider_view = entity_registry->view<Transform_Component, PhysicsCollider_Component>();
		auto mesh_view = entity_registry->view<PhysicsCollider_Component, RenderableMesh_Component>();



		for (auto entity : mesh_view) {
			auto& mesh = mesh_view.get<RenderableMesh_Component>(entity);
			auto& collider = mesh_view.get<PhysicsCollider_Component>(entity);

			collider._localTightGeometry = mesh._tight_geometry;


		}

		for (auto entity : collider_view) {
			auto& transform = collider_view.get<Transform_Component>(entity);
			auto& collider = collider_view.get<PhysicsCollider_Component>(entity);

			collider._tightGeometry = collider._localTightGeometry.post_transform(transform.GetTransform());

			auto sphere = collider._tightGeometry.getBoundingSphere();
			collider._loseGeometry = Sphere{ sphere, sphere.a };
			collider._broadPhaseCollider = collider._loseGeometry;

		}

	}

	void Gravity_System(std::shared_ptr<entt::registry> entity_registry, glm::vec3 gravity) {
		auto view = entity_registry->view<PhysicsObject_Component>();
		for (auto entity : view) {
			auto& physics = view.get<PhysicsObject_Component>(entity);

			physics._acceleration += gravity;

		}
	}



	void PhysicsUpdate_System(std::shared_ptr<entt::registry> entity_registry, float deltaTime) {
		auto view = entity_registry->view<PhysicsObject_Component, LinearVelocity_Component>();
		for (auto entity : view) {
			auto& velocity = view.get<LinearVelocity_Component>(entity);
			auto& physics = view.get<PhysicsObject_Component>(entity);

			velocity._velocity += physics._acceleration * deltaTime;
			physics._acceleration = glm_aux::vec3_000;
		}



	}


	void PlaneColission_System(std::shared_ptr<entt::registry> entity_registry) {
		auto view = entity_registry->view<Transform_Component, LinearVelocity_Component, PhysicsCollider_Component>();
		auto planeColliders = entity_registry->view<PlaneCollider_Component>();

		for (auto entity : view) {
			auto& collider =	view.get<PhysicsCollider_Component>(entity);
			auto& transform =	view.get<Transform_Component>(entity);
			auto& velocity =	view.get<LinearVelocity_Component>(entity);


			for (auto planeE : planeColliders) {
				auto& planeC = planeColliders.get<PlaneCollider_Component>(planeE);
				Intersection outIntersection;
				CollisionHelpers::AABB_Plane_Collision(collider._tightGeometry, planeC._planeColider, outIntersection);

				if (outIntersection.depth < 0)
					continue;

				CollisionHelpers::SolveCollision(transform, velocity, outIntersection);
				
			}
		}



	}

	void DynamicColission_System(std::shared_ptr<entt::registry> entity_registry, EventP::EventQueue& eventDispatcher) {
		auto view = entity_registry->view<Transform_Component, LinearVelocity_Component, PhysicsCollider_Component>();
		auto colliders_view = entity_registry->view<PhysicsCollider_Component>();

		std::vector<Sphere*> sphere_list;

		std::map<Sphere*, entt::entity> sphere_entity_map;

		for (auto collider_entity : colliders_view) {
			auto& collider = colliders_view.get<PhysicsCollider_Component>(collider_entity);

			auto nSphere = new Sphere(collider._broadPhaseCollider);
			sphere_list.emplace_back(nSphere);
			sphere_entity_map[nSphere] = collider_entity;
		}
		auto BVH = BVH::BuildBVHBottomUp(sphere_list, 30);



		/*
		for (auto entity : view) {
			auto& transform = view.get<Transform_Component>(entity);
			auto& velocity = view.get<LinearVelocity_Component>(entity);

			auto& collider = view.get<PhysicsCollider_Component>(entity);
			auto possibleColliders = BVH::IntersectBVH(BVH, collider._broadPhaseCollider);

	


			for (auto pCollider : possibleColliders) {
				auto collider_entity = sphere_entity_map[pCollider];

				if (entity == collider_entity)
					continue;

				auto& other_collider = colliders_view.get<PhysicsCollider_Component>(collider_entity);

				if (collider.isTrigger && other_collider.isTrigger)
					continue;

				if (!CollisionHelpers::Sphere_Sphere_Overlap(collider._loseGeometry, other_collider._loseGeometry))
					continue;

				if (!CollisionHelpers::AABB_AABB_Overlap(collider._tightGeometry, other_collider._tightGeometry))
					continue;
				
				Intersection outIntersection;

				

				CollisionHelpers::Sphere_Sphere_Collision(collider._loseGeometry, other_collider._loseGeometry, outIntersection);
				
				
				if (glm::abs(outIntersection.normal.x) >= glm::abs(outIntersection.normal.y)) {
					if (glm::abs(outIntersection.normal.x) >= glm::abs(outIntersection.normal.z)) {
						outIntersection.normal *= glm_aux::vec3_100;
					}
					else {
						outIntersection.normal *= glm_aux::vec3_001;
					}
				}
				else {
					outIntersection.normal *= glm_aux::vec3_010;
				}



				



				auto args = new Collision_Args{
					"EVENT_Collision",
					entity,
					collider_entity,
					outIntersection
				};

				eventDispatcher.EnqueueEvent(args);
				
				outIntersection.depth *= 0.01f;
				
				CollisionHelpers::SolveCollision(transform, velocity, outIntersection);
			}

		}*/
		

		for (auto entity : colliders_view) {
			auto& this_collider = view.get<PhysicsCollider_Component>(entity);

			auto possibleColliders = BVH::IntersectBVH(BVH, this_collider._broadPhaseCollider);

			for (auto pCollider : possibleColliders) {
				auto collider_entity = sphere_entity_map[pCollider];

				if (entity == collider_entity)
					continue;

				auto& other_collider = view.get<PhysicsCollider_Component>(collider_entity);


				if (other_collider.isTrigger)
					continue;

				if (!CollisionHelpers::Sphere_Sphere_Overlap(this_collider._loseGeometry, other_collider._loseGeometry))
					continue;

				if (!CollisionHelpers::AABB_AABB_Overlap(this_collider._tightGeometry, other_collider._tightGeometry))
					continue;


				Intersection outIntersection;
				CollisionHelpers::Sphere_Sphere_Collision(this_collider._loseGeometry, other_collider._loseGeometry, outIntersection);

				if (outIntersection.normal != glm_aux::vec3_000) {
					if (glm::abs(outIntersection.normal.x) >= glm::abs(outIntersection.normal.y)) {
						if (glm::abs(outIntersection.normal.x) >= glm::abs(outIntersection.normal.z)) {
							outIntersection.normal *= glm_aux::vec3_100;
						}
						else {
							outIntersection.normal *= glm_aux::vec3_001;
						}
					}
					else {
						outIntersection.normal *= glm_aux::vec3_010;
					}
					outIntersection.normal = glm::normalize(outIntersection.normal);
				}


				this_collider.currentCollisions.insert(collider_entity);


				if (this_collider.isTrigger) {


					auto args = new Collision_Args{
					"EVENT_TriggerCollision",
					entity,
					collider_entity,
					outIntersection
					};

					//eventDispatcher.EnqueueEvent(args);

					continue;
				}

				outIntersection.depth *= 0.01f;

				auto args = new Collision_Args{
				"EVENT_Collision",
				entity,
				collider_entity,
				outIntersection
				};

				//eventDispatcher.EnqueueEvent(args);

				if (entity_registry->all_of<Transform_Component>(entity) && entity_registry->all_of<LinearVelocity_Component>(entity)) {
					auto& transform = view.get<Transform_Component>(entity);
					auto& velocity = view.get<LinearVelocity_Component>(entity);

					CollisionHelpers::SolveCollision(transform, velocity, outIntersection);
					
				}


			}

		}


		
		
		sphere_list.clear();
		sphere_entity_map.clear();

		BVH->DeleteRecursive();

		
	}


	void CollisionStartEnd_System(std::shared_ptr<entt::registry> entity_registry, EventP::EventQueue& eventDispatcher) {
		auto view = entity_registry->view<PhysicsCollider_Component>();

		std::string EVENT_CollisionStart = "EVENT_CollisionStart";
		std::string EVENT_CollisionEnd = "EVENT_CollisionEnd";

		std::string EVENT_TriggerStart = "EVENT_TriggerStart";
		std::string EVENT_TriggerEnd = "EVENT_TriggerEnd";

		for (auto entity : view) {
			auto& collider = view.get<PhysicsCollider_Component>(entity);

			auto& current = collider.currentCollisions;
			auto& previous = collider.previousCollisions;

			if (current.empty() && previous.empty())
				continue;

			std::vector<entt::entity> gainedDiff (current.size());
			std::vector<entt::entity> lostDiff (previous.size());

			auto gainIt = std::set_difference(current.begin(), current.end(), previous.begin(), previous.end(), gainedDiff.begin());

			auto lossIt = std::set_difference(previous.begin(), previous.end(), current.begin(), current.end(), lostDiff.begin());


			for (auto it = gainedDiff.begin(); it != gainIt;) {
				auto current = *it;
				it++;

				auto args = new Collision_Args{
				collider.isTrigger ? EVENT_TriggerStart : EVENT_CollisionStart,
				entity,
				current
				};

				eventDispatcher.EnqueueEvent(args);
				
			}


			for (auto it = lostDiff.begin(); it != lossIt;) {
				auto current = *it;
				it++;

				auto args = new Collision_Args{
				collider.isTrigger ? EVENT_TriggerEnd : EVENT_CollisionEnd,
				entity,
				current
				};

				eventDispatcher.EnqueueEvent(args);

			}

			std::swap(current, previous);
			
			current.clear();
		}


	}


	void DebugColliders_System(std::shared_ptr<entt::registry> entity_registry, ShapeRendererPtr shapeRenderer) {
		auto view = entity_registry->view<PhysicsCollider_Component>();
		
		shapeRenderer->push_states(ShapeRendering::Color4u{ 0xFFE61A80 });
		for (auto entity : view) {
			auto& colliders = view.get<PhysicsCollider_Component>(entity);
			shapeRenderer->push_AABB(colliders._tightGeometry.min, colliders._tightGeometry.max);
			
		}
		shapeRenderer->pop_states<ShapeRendering::Color4u>();



		shapeRenderer->push_states(ShapeRendering::Color4u{ 0xFFc4ff0e });
		for (auto entity : view) {
			auto& colliders = view.get<PhysicsCollider_Component>(entity);
			
			auto s = colliders._loseGeometry;

			auto r = s.GetRadius();

			auto mat = glm_aux::TRS(s.GetPosition(), 0, glm_aux::vec3_001, { r,r,r });

			shapeRenderer->push_states(mat);

			shapeRenderer->push_sphere_wireframe(1, 1);

			shapeRenderer->pop_states<glm::mat4>();

		}
		shapeRenderer->pop_states<ShapeRendering::Color4u>();

	}



	namespace CollisionHelpers {


		void SolveCollision(Transform_Component& transform, LinearVelocity_Component& velocity, Intersection& collision) {
			transform._position += collision.normal * collision.depth;

			float dot = glm::dot(velocity._velocity, collision.normal);

			if(dot <= 0)
				velocity._velocity -= dot * collision.normal;


		}





		std::vector<glm::ivec2> CollisionHelpers::FindMinMaxValues(const glm::vec3(&points)[], int pointCount) {
			glm::ivec2 minMaxX{ 0, 0 }, minMaxY{ 0, 0 }, minMaxZ{ 0, 0 };

			for (int i = 0; i != pointCount; ++i) {
				auto p = points[i];

				if (p.x < points[minMaxX.x].x)
					minMaxX.x = i;
				if (p.x > points[minMaxX.y].x)
					minMaxX.y = i;

				if (p.y < points[minMaxY.x].y)
					minMaxY.x = i;
				if (p.y > points[minMaxY.y].y)
					minMaxY.y = i;

				if (p.z < points[minMaxZ.x].z)
					minMaxZ.x = i;
				if (p.z > points[minMaxZ.y].z)
					minMaxZ.y = i;
			}

			return std::vector<glm::ivec2> {
				minMaxX,
					minMaxY,
					minMaxZ,
			};
		}

		glm::ivec2 CollisionHelpers::FindMostDistantPoints(const std::vector<glm::ivec2>& minMaxPoints, const glm::vec3(&points)[]) {
			float maxDistanceSqr = -1;
			glm::ivec2 maxDistancePoints{ 0,0 };

			for (int i = 0; i < minMaxPoints.size(); ++i) {
				auto vec = points[minMaxPoints[i].y] - points[minMaxPoints[i].x];
				auto distSqr = glm::dot(vec, vec);

				if (distSqr < maxDistanceSqr)
					continue;

				maxDistancePoints = minMaxPoints[i];
				maxDistanceSqr = distSqr;

			}

			return maxDistancePoints;
		}

		Sphere CollisionHelpers::BuildSphereFromPoints(const glm::vec3(&points)[], int pointCount) {
			auto minMaxPoints = FindMinMaxValues(points, pointCount);

			auto mostDistantPair = FindMostDistantPoints(minMaxPoints, points);

			Sphere s;

			s.SetPosition(points[mostDistantPair.x] + points[mostDistantPair.y] * 0.5f);

			auto diff = points[mostDistantPair.y] - s.GetPosition();

			s.SetRadius(glm::sqrt(glm::dot(diff, diff)));

			return s;
		}



		//bool CollisionPackage::CollisionHelpers::Sphere_Sphere_Intersection(const Sphere& collider, const Sphere& obsticle, float threshold, Intersection& intersection) {
		//
		//	auto centerDif = collider.GetPosition() - obsticle.GetPosition();
		//
		//	float centerDistSqr = glm::dot(centerDif, centerDif);
		//
		//	float colliderR = collider.GetRadius();
		//	float obsticleR = obsticle.GetRadius();
		//
		//	float edgeDistSqr = centerDistSqr - (colliderR + obsticleR) * (colliderR + obsticleR);
		//	
		//	if (edgeDistSqr > 0) {
		//		intersection.intersection = 1;
		//		return false;
		//	}
		//
		//	intersection.depth = -(glm::sqrt(centerDistSqr) - colliderR - obsticleR);
		//
		//	intersection.intersection = intersection.depth > threshold ? -1 : 0;
		//
		//	intersection.normal = glm::normalize(centerDif);
		//
		//	intersection.position = obsticle.GetPosition() + intersection.normal * obsticleR;
		//
		//	return true;
		//}


		//bool CollisionPackage::CollisionHelpers::Sphere_Plane_Intersection(const Sphere& collider, const Plane& obsticle, float threshold, Intersection& intersection) {
		//	
		//
		//}


		bool CollisionHelpers::Sphere_Sphere_Overlap(const Sphere& A, const Sphere& B) {

			auto centerDif = A.GetPosition() - B.GetPosition();

			float centerDistSqr = glm::dot(centerDif, centerDif);

			float radiusSum = A.GetRadius() + B.GetRadius();

			return centerDistSqr <= radiusSum * radiusSum;
		}

		bool CollisionHelpers::AABB_AABB_Overlap(const eeng::AABB& A, const eeng::AABB& B) {
			return A.intersect(B);
		}




		/*bool CollisionHelpers::Sphere_Plane_Overlap(const Sphere& A, const Plane& B) {
			return glm::dot(A.GetPosition(), B.normal) - B.distanceToOrigin < A.GetRadius();
		}

		bool CollisionHelpers::AABB_Plane_Overlap(const eeng::AABB& A, const Plane& B) {
			auto halfWidths = AABBHalfWidths(A);
			auto position = AABBCenterPoint(A);

			auto r =	halfWidths.x * glm::abs(B.normal.x) +
						halfWidths.y * glm::abs(B.normal.y) +
						halfWidths.z * glm::abs(B.normal.z);

			auto distance = glm::dot(B.normal, position) - B.distanceToOrigin;

			return distance <= r;
		}*/

		void Sphere_Sphere_Collision(const Sphere& A, const Sphere& B, Intersection& outIntersectionA) {
			auto diff = A.GetPosition() - B.GetPosition();
			float distCenter = glm::length(diff);
			float distEdge = distCenter - A.GetRadius() - B.GetRadius();

			outIntersectionA.depth = -distEdge;


			if (distCenter == 0) {
				outIntersectionA.normal = glm_aux::vec3_000;
				return;
			}
			outIntersectionA.normal = diff / distCenter;
		}



		void AABB_Plane_Collision(const eeng::AABB& A, const Plane& B, Intersection& outIntersectionA) {
			outIntersectionA.normal = glm_aux::vec3_010;

			outIntersectionA.depth = B.distanceToOrigin - A.min.y;
		}


		glm::vec3 CollisionHelpers::AABBHalfWidths(const eeng::AABB& aabb) {
			return (aabb.max - aabb.min) * 0.5f;
		}

		glm::vec3 CollisionHelpers::AABBCenterPoint(const eeng::AABB& aabb) {
			return aabb.min * 0.5f + aabb.max * 0.5f;
		}


	}
	namespace BVH {



		void FindMinMaxSpherePoints(const Sphere* A, const Sphere* B, glm::vec3& minOut, glm::vec3& maxOut) {
			minOut.x = std::fminf(A->x() - A->r(), B->x() - B->r());
			maxOut.x = std::fmaxf(A->x() + A->r(), B->x() + B->r());

			minOut.y = std::fminf(A->y() - A->r(), B->y() - B->r());
			maxOut.y = std::fmaxf(A->y() + A->r(), B->y() + B->r());

			minOut.z = std::fminf(A->z() - A->r(), B->z() - B->r());
			maxOut.z = std::fmaxf(A->z() + A->r(), B->z() + B->r());
		}

		float SphereDistance(const Sphere* A, const Sphere* B) {
			auto centerDist = glm::length((B->GetPosition() - A->GetPosition()));

			float distance = centerDist - (A->r() + B->r());

			return std::fmaxf(0.0f, distance);
		}


		SphereNode* BuildNodeFromSpheres(Sphere* left, Sphere* right) {
			glm::vec3 minPoint, maxPoint;
			FindMinMaxSpherePoints(left, right, minPoint, maxPoint);

			auto midPoint = minPoint * 0.5f + maxPoint * 0.5f;
			auto radius = glm::distance(midPoint, maxPoint);

			return new SphereNode{ new Sphere{ midPoint,radius}, nullptr, nullptr };
		}

		SphereNode* BuildNodeFromSphere(Sphere* sphere) {
			return new SphereNode{ sphere, nullptr, nullptr };
		}



		std::vector<std::pair<SphereNode*, SphereNode*>> FindNodePairs_Greedy(std::vector<SphereNode*> openList, float maxDistance) {

			std::vector<std::pair<SphereNode*, SphereNode*>> foundPairs;
			std::vector<SphereNode*>  availableNodes = openList;


			while (!availableNodes.empty()) {
				SphereNode* current = availableNodes.back();
				availableNodes.pop_back();

				float closedDist = maxDistance;
				SphereNode* bestMatch = nullptr;
				int bestIndex = -1;

				for (int j = 0; j < availableNodes.size(); ++j) {
					float dist = SphereDistance(current->sphere, availableNodes[j]->sphere);

					if (dist >= closedDist)
						continue;
					closedDist = dist;
					bestMatch = availableNodes[j];
					bestIndex = j;

				}

				if (bestMatch) {
					availableNodes.erase(availableNodes.begin() + bestIndex);
				}
				foundPairs.push_back({ current,bestMatch });
			}

			return foundPairs;

		}


		SphereNode* BuildBVHBottomUp(std::vector<Sphere*> spheres, float maxLeafPairDistance) {
			if (spheres.empty())
				return nullptr;


			std::vector<SphereNode*> openList;
			for (Sphere* sphere : spheres) {
				openList.push_back(BuildNodeFromSphere(sphere));
			}

			while (openList.size() != 1) {
				auto pairs = FindNodePairs_Greedy(openList, maxLeafPairDistance);
				openList.clear();

				for (auto pair : pairs) {
					SphereNode* node = pair.second ? 
						BuildNodeFromSpheres(pair.first->sphere, pair.second->sphere) : 
						BuildNodeFromSphere(pair.first->sphere);
					
					node->leftChild = pair.first;
					node->rightChild = pair.second;
					
					openList.push_back(node);
				}
				maxLeafPairDistance = std::numeric_limits<float>::max();
			}

			return openList[0];
		}


		std::vector<Sphere*> IntersectBVH(SphereNode* rootNode,const Sphere& sphere) {
			std::vector<Sphere*> intersectingLeaves;

			if (!rootNode)
				return intersectingLeaves;

			if (!CollisionHelpers::Sphere_Sphere_Overlap(*rootNode->sphere, sphere))
				return intersectingLeaves;
			
			if (!rootNode->leftChild && !rootNode->rightChild) {
				intersectingLeaves.push_back(rootNode->sphere);
				return intersectingLeaves;
			}

			auto childIntersection = IntersectBVH(rootNode->leftChild, sphere);
			intersectingLeaves.insert(intersectingLeaves.begin(), childIntersection.begin(), childIntersection.end());

			childIntersection = IntersectBVH(rootNode->rightChild, sphere);
			intersectingLeaves.insert(intersectingLeaves.begin(), childIntersection.begin(), childIntersection.end());

			return intersectingLeaves;

		}
	}
	#pragma endregion

}



