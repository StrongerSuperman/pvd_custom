#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <PxPhysics.h>
#include <foundation/PxBounds3.h>
#include <foundation/PxMat44.h> 
#include <geometry/PxTriangleMesh.h>
#include <geometry/PxTriangleMeshGeometry.h>
#include <geometry/PxConvexMesh.h>
#include <geometry/PxConvexMeshGeometry.h>
#include "PxPhysicsAPI.h"

#include "RenderObject.h"
#include "RenderBuffer.h"
#include "PhysicsWorld.h"
#include "MeshCounter.h"
#include "Ray.h"

# define M_PI 3.14159265358979323846
using shapesMap = std::map<physx::PxShape*, std::pair<uint, physx::PxRigidActor*>>;


namespace PhysxHelper
{
	glm::mat4x4 PxMat44ToGlmMatrix4x4(const physx::PxMat44 &mat);
	glm::vec4 PxVec4ToGlmVector4(const physx::PxVec4 &vec4);
	glm::vec3 PxVec3ToGlmVector3(const physx::PxVec3 &vec3);
	physx::PxVec4 GlmVector4ToPxVec4(const glm::vec4 &vec4);
	physx::PxVec3 GlmVector3ToPxVec3(const glm::vec3 &vec3);
	glm::quat PxPxQuatToGlmQuaternion(const physx::PxQuat &quat);
	physx::PxBounds3 CalculateAABB(const std::vector<physx::PxBounds3> &bounds);

	bool CheckFile(bool& isBinary, const char* filename);
	physx::PxCollection* DeserializeCollection(physx::PxInputData& inputData, bool isBinary, physx::PxCollection* sharedCollection, physx::PxSerializationRegistry* sr, physx::PxCooking* ck);
	void SerializeCollection(physx::PxCollection& collection, physx::PxCollection* externalRefs, const char* filename, bool toBinary, physx::PxSerializationRegistry* sr);
	bool DumpBinaryMetaData(const char* filename, physx::PxSerializationRegistry* sr);
	bool LoadCollectionFile(std::vector<std::string>& filenames, PhysicsWorld* physicsWorld);

	void CreateRenderObjectFromShapes(std::vector<RenderObject> &objects, const shapesMap &pxShapesMap, MeshCounter* counter);
	RenderObject CreateRenderObjectFromPxGeometry(uint id, const physx::PxGeometryHolder& geomHd, const physx::PxMat44& posMat, glm::vec3 color, MeshCounter* counter);
	RenderBuffer CreateCapsuleRenderBuffer(float halfHeight, float radius, uint slices, uint stacks);
	glm::vec3 CastPhysxFilterDataToColor(physx::PxFilterData data);

	bool RayCast(const Ray &ray, physx::PxScene* pxScene, physx::PxRaycastBuffer& hitinfo);
};
