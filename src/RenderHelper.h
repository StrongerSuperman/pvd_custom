#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <PxPhysics.h>
#include "PxPhysicsAPI.h"
#include <foundation/PxMat44.h> 
#include <geometry/PxTriangleMesh.h>
#include <geometry/PxTriangleMeshGeometry.h>
#include <geometry/PxConvexMesh.h>
#include <geometry/PxConvexMeshGeometry.h>

#include "RenderObject.h"
#include "RenderBuffer.h"
#include "SceneObject.h"
#include "MathHelper.h"


# define M_PI 3.14159265358979323846


RenderObject CreateRenderObjectFromPxGeometry(int id, const physx::PxGeometryHolder& geomHd, const physx::PxMat44& posMat, glm::vec3 color, MeshCounter* counter);
RenderBuffer CreateCapsuleRenderBuffer(float halfHeight, float radius, uint slices, uint stacks);
glm::vec3 CastPhysxFilterDataToColor(physx::PxFilterData data);
