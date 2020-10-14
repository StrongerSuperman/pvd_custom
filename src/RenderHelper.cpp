#include "RenderHelper.h"


RenderObject CreateRenderObjectFromPxGeometry(int id, const physx::PxGeometryHolder& geomHd, const physx::PxMat44& posMat, glm::vec3 color, MeshCounter* counter)
{
	switch (geomHd.getType())
	{
	case physx::PxGeometryType::eBOX:
	{
		counter->boxNum++;

		// box data
		const physx::PxBoxGeometry& boxGeom = geomHd.box();
		float extentX = boxGeom.halfExtents.x;
		float extentY = boxGeom.halfExtents.y;
		float extentZ = boxGeom.halfExtents.z;

		// pose mat
		physx::PxMat44 mat(posMat);
		glm::mat4x4 modelMatrix = PxMat44ToGlmMatrix4x4(mat);

		// render object
		auto renderData = RenderData(color, modelMatrix);
		auto renderBuffer = CreateBoxRenderBuffer(extentX, extentY, extentZ);
		return RenderObject(id, renderData, renderBuffer);
	}
	break;
	case physx::PxGeometryType::eSPHERE:
	{
		counter->sphereNum++;
		uint slices = 10;
		uint stacks = 10;

		// sphere data
		const physx::PxSphereGeometry& sphereGeom = geomHd.sphere();
		float radius = sphereGeom.radius;

		// pose mat
		physx::PxMat44 mat(posMat);
		glm::mat4x4 modelMatrix = PxMat44ToGlmMatrix4x4(mat);

		// render object
		auto renderData = RenderData(color, modelMatrix);
		auto renderBuffer = CreateSphereRenderBuffer(radius, slices, stacks);
		return RenderObject(id, renderData, renderBuffer);
	}
	break;
	case physx::PxGeometryType::eCAPSULE:
	{
		counter->capsuleNum++;

		// capsule data
		const physx::PxCapsuleGeometry& capsuleGeom = geomHd.capsule();
		float halfHeight = capsuleGeom.halfHeight;
		float radius = capsuleGeom.radius;
		uint slices = 10;
		uint stacks = 10;

		// pose mat
		physx::PxMat44 mat(posMat);
		physx::PxMat44 rot(physx::PxQuat((float)M_PI / 2, physx::PxVec3(0, 0, 1)));
		physx::PxVec4 scale = physx::PxVec4(1, 1, 1, 1);
		mat = mat * rot;
		mat.scale(scale);
		glm::mat4x4 modelMatrix = PxMat44ToGlmMatrix4x4(mat);

		// render object
		auto renderData = RenderData(color, modelMatrix);
		auto renderBuffer = CreateCapsuleRenderBuffer(halfHeight, radius, slices, stacks);
		return RenderObject(id, renderData, renderBuffer);
	}
	break;
	case physx::PxGeometryType::eCONVEXMESH:
	{
		counter->convexMeshNum++;

		// mesh
		const physx::PxConvexMeshGeometry& convexGeom = geomHd.convexMesh();
		const physx::PxConvexMesh& mesh = *convexGeom.convexMesh;

		// vertices
		const void* vertices = reinterpret_cast<const void*>(mesh.getVertices());
		uint vertexNum = static_cast<uint>(mesh.getNbVertices());

		// indices
		const physx::PxU32 nbPolys = mesh.getNbPolygons();
		const physx::PxU8* polygons = mesh.getIndexBuffer();
		std::vector<ushort> indexes;
		physx::PxU32 numTotalTriangles = 0;
		for (physx::PxU32 i = 0; i < nbPolys; i++)
		{
			physx::PxHullPolygon data;
			mesh.getPolygonData(i, data);
			const physx::PxU32 nbTris = physx::PxU32(data.mNbVerts - 2);
			auto vref0 = static_cast<ushort>(polygons[data.mIndexBase]);
			assert(vref0 < vertexNum);
			for (physx::PxU32 j = 0; j < nbTris; j++)
			{
				auto vref1 = static_cast<ushort>(polygons[data.mIndexBase + j + 1]);
				auto vref2 = static_cast<ushort>(polygons[data.mIndexBase + j + 2]);
				assert(vref1 < vertexNum);
				assert(vref2 < vertexNum);
				indexes.push_back(vref0);
				indexes.push_back(vref1);
				indexes.push_back(vref2);
				numTotalTriangles++;
			}
		}
		const void* indices = reinterpret_cast<const void*>(&indexes[0]);
		uint indexNum = static_cast<uint>(numTotalTriangles * 3);

		// pose mat
		physx::PxMat44 mat(posMat);
		physx::PxVec4 scale = physx::PxVec4(convexGeom.scale.scale, 1);
		mat.scale(scale);
		glm::mat4x4 modelMatrix = PxMat44ToGlmMatrix4x4(mat);

		// render object
		auto renderData = RenderData(color, modelMatrix);
		auto renderBuffer = CreateRenderBuffer2(vertices, vertexNum, indices, indexNum, U16);
		return RenderObject(id, renderData, renderBuffer);
	}
	break;
	case physx::PxGeometryType::eTRIANGLEMESH:
	{
		counter->triangleMeshNum++;

		// mesh
		const physx::PxTriangleMeshGeometry& triGeom = geomHd.triangleMesh();
		const physx::PxTriangleMesh& mesh = *triGeom.triangleMesh;

		// vertices
		const void* vertices = reinterpret_cast<const void*>(mesh.getVertices());
		uint vertexNum = static_cast<uint>(mesh.getNbVertices());

		// indices
		const void* indices = mesh.getTriangles();
		uint indexNum = static_cast<uint>(mesh.getNbTriangles() * 3);
		bool has16BitIndices = mesh.getTriangleMeshFlags() & physx::PxTriangleMeshFlag::e16_BIT_INDICES;
		auto indicesType = U32;
		if (has16BitIndices)
		{
			indicesType = U16;
		}

		// pose mat
		physx::PxMat44 mat(posMat);
		physx::PxVec4 scale = physx::PxVec4(triGeom.scale.scale, 1);
		mat.scale(scale);
		glm::mat4x4 modelMatrix = PxMat44ToGlmMatrix4x4(mat);

		// render object
		auto renderData = RenderData(color, modelMatrix);
		auto renderBuffer = CreateRenderBuffer2(vertices, vertexNum, indices, indexNum, indicesType);
		return RenderObject(id, renderData, renderBuffer);
	}
	break;
	case physx::PxGeometryType::eHEIGHTFIELD:
	{
		counter->heightFieldNum++;
		assert(0), "not implement physx geometry type!";
	}
	break;
	case physx::PxGeometryType::ePLANE:
	{
		counter->planeNum++;
		assert(0), "not implement physx geometry type!";
	}
	break;
	}

	assert(geomHd.getType() == physx::PxGeometryType::eINVALID), "invalid physx geometry type!";
	return RenderObject();
}

RenderBuffer CreateBoxRenderBuffer(float extentX, float extentY, float extentZ)
{
	auto x = extentX;
	auto y = extentY;
	auto z = extentZ;
	uint verticesNum = 24;
	uint trianglesNum = 12;

	// create vertices(with normal)
	std::vector<glm::vec3> vertexes(verticesNum*2);
	{
		uint offset = 0;

		vertexes[offset++] = glm::vec3(-x, y, -z);                 
		vertexes[offset++] = glm::vec3(0, 1, 0);
		vertexes[offset++] = glm::vec3(x, y, -z);
		vertexes[offset++] = glm::vec3(0, 1, 0);
		vertexes[offset++] = glm::vec3(-x, y, z);
		vertexes[offset++] = glm::vec3(0, 1, 0);
		vertexes[offset++] = glm::vec3(x, y, z);
		vertexes[offset++] = glm::vec3(0, 1, 0);

		vertexes[offset++] = glm::vec3(x, -y, z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);
		vertexes[offset++] = glm::vec3(-x, -y, z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);
		vertexes[offset++] = glm::vec3(x, -y, z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);
		vertexes[offset++] = glm::vec3(-x, -y, z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);

		vertexes[offset++] = glm::vec3(-x, y, z);
		vertexes[offset++] = glm::vec3(0, 0, 1);
		vertexes[offset++] = glm::vec3(x, y, z);
		vertexes[offset++] = glm::vec3(0, 0, 1);
		vertexes[offset++] = glm::vec3(-x, -y, z);
		vertexes[offset++] = glm::vec3(0, 0, 1);
		vertexes[offset++] = glm::vec3(x, -y, z);
		vertexes[offset++] = glm::vec3(0, 0, 1);

		vertexes[offset++] = glm::vec3(x, y, z);
		vertexes[offset++] = glm::vec3(0, 1, 0);
		vertexes[offset++] = glm::vec3(x, y, -z);
		vertexes[offset++] = glm::vec3(0, 1, 0);
		vertexes[offset++] = glm::vec3(x, -y, z);
		vertexes[offset++] = glm::vec3(0, 1, 0);
		vertexes[offset++] = glm::vec3(x, -y, -z);
		vertexes[offset++] = glm::vec3(0, 1, 0);

		vertexes[offset++] = glm::vec3(x, y, -z);
		vertexes[offset++] = glm::vec3(1, 0, 0);
		vertexes[offset++] = glm::vec3(-x, y, -z);
		vertexes[offset++] = glm::vec3(1, 0, 0);
		vertexes[offset++] = glm::vec3(x, -y, -z);
		vertexes[offset++] = glm::vec3(1, 0, 0);
		vertexes[offset++] = glm::vec3(-x, -y, -z);
		vertexes[offset++] = glm::vec3(1, 0, 0);

		vertexes[offset++] = glm::vec3(-x, y, -z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);
		vertexes[offset++] = glm::vec3(-x, y, z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);
		vertexes[offset++] = glm::vec3(-x, -y, -z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);
		vertexes[offset++] = glm::vec3(-x, -y, z);
		vertexes[offset++] = glm::vec3(-1, 0, 0);
	}

	// create indices
	std::vector<ushort> indexes(trianglesNum * 3);
	{
		uint offset = 0;

		indexes[offset++] = 0; indexes[offset++] = 2; indexes[offset++] = 1;
		indexes[offset++] = 1; indexes[offset++] = 2; indexes[offset++] = 3;

		indexes[offset++] = 4; indexes[offset++] = 6; indexes[offset++] = 5;
		indexes[offset++] = 5; indexes[offset++] = 6; indexes[offset++] = 7;

		indexes[offset++] = 8; indexes[offset++] = 10; indexes[offset++] = 9;
		indexes[offset++] = 9; indexes[offset++] = 10; indexes[offset++] = 11;

		indexes[offset++] = 12; indexes[offset++] = 14; indexes[offset++] = 13;
		indexes[offset++] = 13; indexes[offset++] = 14; indexes[offset++] = 15;

		indexes[offset++] = 16; indexes[offset++] = 18; indexes[offset++] = 17;
		indexes[offset++] = 17; indexes[offset++] = 18; indexes[offset++] = 19;

		indexes[offset++] = 20; indexes[offset++] = 22; indexes[offset++] = 21;
		indexes[offset++] = 21; indexes[offset++] = 22; indexes[offset++] = 23;
	}

	const void* vertices = reinterpret_cast<const void*>(&vertexes[0]);
	const void* indices = reinterpret_cast<const void*>(&indexes[0]);

	return CreateRenderBuffer(vertices, verticesNum, indices, trianglesNum * 3);
}

RenderBuffer CreateSphereRenderBuffer(float radius, uint slices, uint stacks)
{
	//uint halfStacks = stacks / 2;
	//uint verticesNum = slices * (halfStacks - 1) * 2 + 2;
	//uint trianglesNum = slices * (halfStacks - 1) * 4;

	//// create vertices
	//std::vector<glm::vec3> vertexes(verticesNum * 2);

	//// create indices
	//std::vector<ushort> indexes(trianglesNum * 3);

	//const void* vertices = reinterpret_cast<const void*>(&vertexes[0]);
	//const void* indices = reinterpret_cast<const void*>(&indexes[0]);

	//return CreateRenderBuffer(vertices, verticesNum, indices, trianglesNum * 3);
	return RenderBuffer();
}

RenderBuffer CreateCapsuleRenderBuffer(float halfHeight, float radius, uint slices, uint stacks)
{
	if (stacks % 2 > 0)
	{
		stacks += 1;
	}
	uint halfStacks = stacks / 2;
	uint verticesNum = slices * (halfStacks - 1) * 2 + 2;
	uint trianglesNum = slices * (halfStacks - 1) * 4;

	// create vertices(with normal)
	std::vector<glm::vec3> vertexes(verticesNum * 2);
	{
		uint offset = 0;
		// top point
		glm::vec3 topPoint(0, halfHeight + radius, 0);
		glm::vec3 topNormal = glm::normalize(glm::vec3(0, radius, 0));
		vertexes[offset++] = topPoint;
		vertexes[offset++] = topNormal;

		float* pSinList = new float[slices];
		float* pCosList = new float[slices];
		float angleXZ;
		for (uint j = 0; j < slices; j++)
		{
			angleXZ = M_PI * 2 * j / slices;
			pSinList[j] = sin(angleXZ);
			pCosList[j] = cos(angleXZ);
		}

		// top hemi-sphere¡¢top-hemi-cylinder
		for (uint i = 1; i < halfStacks; i++)
		{
			float angleY = M_PI / 2 * i / (halfStacks - 1);
			float radiusXZ = radius * sin(angleY);
			float posY = radius * cos(angleY);
			float posX, posZ;

			for (uint j = 0; j < slices; j++)
			{
				posX = radiusXZ * pSinList[j % slices];
				posZ = radiusXZ * pCosList[j % slices];

				glm::vec3 upperPoint(posX, posY + halfHeight, posZ);
				glm::vec3 upperNormal = glm::normalize(glm::vec3(posX, posY, posZ));
				vertexes[offset++] = upperPoint;
				vertexes[offset++] = upperNormal;
			}
		}

		// lower-hemi-cylinder¡¢bottom hemi-sphere
		for (uint i = halfStacks - 1; i > 0; i--)
		{
			float angleY = M_PI / 2 * i / (halfStacks - 1);
			float radiusXZ = radius * sin(angleY);
			float posY = radius * cos(angleY);
			float posX, posZ;

			for (uint j = 0; j < slices; j++)
			{
				posX = radiusXZ * pSinList[j % slices];
				posZ = radiusXZ * pCosList[j % slices];

				glm::vec3 lowerPoint(posX, -(posY + halfHeight), posZ);
				glm::vec3 lowerNormal = glm::normalize(glm::vec3(posX, -posY, posZ));
				vertexes[offset++] = lowerPoint;
				vertexes[offset++] = lowerNormal;
			}
		}

		// bottom point
		glm::vec3 bottomPoint(0, -(halfHeight + radius), 0);
		glm::vec3 bottomNormal = glm::normalize(glm::vec3(0, -radius, 0));
		vertexes[offset++] = bottomPoint;
		vertexes[offset++] = bottomNormal;

		delete[] pSinList;
		delete[] pCosList;
	}

	// create indices
	std::vector<ushort> indexes(trianglesNum * 3);
	{
		uint offset = 0;
		for (uint i = 0; i < stacks - 1; i++)
		{
			if (i == 0)
			{
				for (uint j = 0; j < slices; j++)
				{
					indexes[offset++] = 0;
					indexes[offset++] = 1 + j;
					indexes[offset++] = 1 + (j + 1) % slices;
				}
			}
			else if (i > 0 && i < stacks - 2)
			{
				for (uint j = 0; j < slices; j++)
				{
					indexes[offset++] = 1 + slices * (i - 1) + j;
					indexes[offset++] = 1 + slices * i + j;
					indexes[offset++] = 1 + slices * i + (j + 1) % slices;

					indexes[offset++] = 1 + slices * (i - 1) + j;
					indexes[offset++] = 1 + slices * i + (j + 1) % slices;
					indexes[offset++] = 1 + slices * (i - 1) + (j + 1) % slices;
				}
			}
			else if (i == stacks - 2)
			{
				for (uint j = 0; j < slices; j++)
				{
					indexes[offset++] = 1 + slices * (i - 1) + j;
					indexes[offset++] = 1 + slices * i;
					indexes[offset++] = 1 + slices * (i - 1) + (j + 1) % slices;
				}
			}
		}
	}

	const void* vertices = reinterpret_cast<const void*>(&vertexes[0]);
	const void* indices = reinterpret_cast<const void*>(&indexes[0]);

	return CreateRenderBuffer(vertices, verticesNum, indices, trianglesNum * 3);
}

glm::vec3 CastPhysxFilterDataToColor(physx::PxFilterData data)
{
	auto word0 = data.word0;
	auto word1 = data.word1;
	auto word2 = data.word2;
	auto word3 = data.word3;
	(void)(word3);

	// TODO: cast filter data to color
	glm::vec3 color(0.6f, 0, 0);
	color += glm::vec3(word0 % 255 / 255.0f, 0, 0);
	color += glm::vec3(0, word1 % 255 / 255.0f, 0);
	color += glm::vec3(0, 0, word2 % 255 / 255.0f);

	return color;
}
