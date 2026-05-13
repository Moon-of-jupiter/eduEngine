#include "CollisionPackage.h"
#include "RenderableMesh.hpp"
#include "glmcommon.hpp"
#include "AABB.h"

namespace CollisionPackage {

	std::vector<glm::ivec2> CollisionHelpers::FindMinMaxValues(const glm::vec3(&points)[], int pointCount) {
		glm::ivec2 minMaxX{ 0, 0}, minMaxY{ 0, 0}, minMaxZ{ 0, 0};

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




	bool CollisionHelpers::Sphere_Plane_Overlap(const Sphere& A, const Plane& B) {
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
	}




	glm::vec3 CollisionHelpers::AABBHalfWidths(const eeng::AABB& aabb) {
		return (aabb.max - aabb.min) * 0.5f;
	}

	glm::vec3 CollisionHelpers::AABBCenterPoint(const eeng::AABB& aabb) {
		return aabb.min * 0.5f + aabb.max * 0.5f;
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


		std::vector<Sphere*> IntersectBVH(SphereNode* rootNode, Sphere* sphere) {
			std::vector<Sphere*> intersectingLeaves;

			if (!sphere || !rootNode)
				return intersectingLeaves;

			if (!CollisionHelpers::Sphere_Sphere_Overlap(*rootNode->sphere, *sphere))
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



