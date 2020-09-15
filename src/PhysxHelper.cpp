#include "PhysxHelper.h"


glm::mat4x4 PhysxHelper::PxMat44ToGlmMatrix4x4(const physx::PxMat44 &mat)
{
	return glm::mat4x4(
		PhysxHelper::PxVec4ToGlmVector4(mat.column0),
		PhysxHelper::PxVec4ToGlmVector4(mat.column1),
		PhysxHelper::PxVec4ToGlmVector4(mat.column2),
		PhysxHelper::PxVec4ToGlmVector4(mat.column3));
}

glm::vec4 PhysxHelper::PxVec4ToGlmVector4(const physx::PxVec4 &vec4)
{
	return glm::vec4(vec4.x, vec4.y, vec4.z, vec4.w);
}

glm::vec3 PhysxHelper::PxVec3ToGlmVector3(const physx::PxVec3 &vec3)
{
	return glm::vec3(vec3.x, vec3.y, vec3.z);
}

physx::PxVec4 PhysxHelper::GlmVector4ToPxVec4(const glm::vec4 &vec4)
{
	return physx::PxVec4(vec4.x, vec4.y, vec4.z, vec4.w);
}

physx::PxVec3 PhysxHelper::GlmVector3ToPxVec3(const glm::vec3 &vec3)
{
	return physx::PxVec3(vec3.x, vec3.y, vec3.z);
}

glm::quat PhysxHelper::PxPxQuatToGlmQuaternion(const physx::PxQuat &quat)
{
	return glm::quat(quat.w, quat.x, quat.y, quat.z);
}

physx::PxBounds3 PhysxHelper::CalculateAABB(const std::vector<physx::PxBounds3> &bounds)
{
	std::vector<float> boundsXMax;
	std::vector<float> boundsYMax;
	std::vector<float> boundsZMax;
	std::vector<float> boundsXMin;
	std::vector<float> boundsYMin;
	std::vector<float> boundsZMin;
	for each(auto bound in bounds)
	{
		boundsXMax.push_back(bound.maximum.x);
		boundsYMax.push_back(bound.maximum.y);
		boundsZMax.push_back(bound.maximum.z);
		boundsXMin.push_back(bound.minimum.x);
		boundsYMin.push_back(bound.minimum.y);
		boundsZMin.push_back(bound.minimum.z);
	}
	float xMax = *std::max_element(std::begin(boundsXMax), std::end(boundsXMax));
	float yMax = *std::max_element(std::begin(boundsYMax), std::end(boundsYMax));
	float zMax = *std::max_element(std::begin(boundsZMax), std::end(boundsZMax));
	float xMin = *std::max_element(std::begin(boundsXMin), std::end(boundsXMin));
	float yMin = *std::max_element(std::begin(boundsYMin), std::end(boundsYMin));
	float zMin = *std::max_element(std::begin(boundsZMin), std::end(boundsZMin));
	return physx::PxBounds3(
		physx::PxVec3(xMin, yMin, zMin),
		physx::PxVec3(xMax, yMax, zMax));
}


bool PhysxHelper::CheckFile(bool& isBinary, const char* filename)
{
	physx::PxDefaultFileInputData fileStream(filename);
	if (fileStream.getLength() == 0)
	{
		printf( "[ERROR] input file %s can't be opened!\n", filename);
		return false;
	}

	char testString[512];
	fileStream.read(testString, 18);
	testString[18] = 0;

	if (strcmp("SEBD", testString) == 0)
	{
		isBinary = true;
		return true;
	}

	if (strcmp("<PhysX30Collection", testString) == 0)
	{
		isBinary = false;
		return true;
	}

	printf( "[ERROR] input file %s seems neither an xml nor a binary serialized collection file!\n", filename);
	return false;
}

physx::PxCollection* PhysxHelper::DeserializeCollection(physx::PxInputData& inputData, bool isBinary, physx::PxCollection* sharedCollection, physx::PxSerializationRegistry* sr, physx::PxCooking* ck)
{
	physx::PxU8*	     gMemBlocks[16];
	physx::PxU32         gNbMemBlocks = 0;
	physx::PxCollection* collection = NULL;

	if(isBinary)
	{
		physx::PxU32 length = inputData.getLength();
		physx::PxU8* memBlock = static_cast<physx::PxU8*>(malloc(length+PX_SERIAL_FILE_ALIGN-1));
		gMemBlocks[gNbMemBlocks++] = memBlock;
		void* alignedBlock = reinterpret_cast<void*>((size_t(memBlock)+PX_SERIAL_FILE_ALIGN-1)&~(PX_SERIAL_FILE_ALIGN-1));
		inputData.read(alignedBlock, length);
		collection = physx::PxSerialization::createCollectionFromBinary(alignedBlock, *sr, sharedCollection);
	}
	else
	{
		collection = physx::PxSerialization::createCollectionFromXml(inputData, *ck, *sr, sharedCollection);
	}

	return collection;
}

void PhysxHelper::SerializeCollection(physx::PxCollection& collection, physx::PxCollection* externalRefs, const char* filename, bool toBinary, physx::PxSerializationRegistry* sr)
{
	physx::PxDefaultFileOutputStream outputStream(filename);
	if (!outputStream.isValid())
	{
		printf( "[ERROR] Could not open file %s!\n", filename);
		return;
	}

	bool bret;
	if (toBinary)
	{
		bret = physx::PxSerialization::serializeCollectionToBinary(outputStream, collection, *sr, externalRefs);
	}
	else
	{
		bret = physx::PxSerialization::serializeCollectionToXml(outputStream, collection, *sr, NULL, externalRefs);
	}

	if(bret)
		printf( "Generated: \"%s\"\n", filename);
	else
		printf( "[ERROR] Failure when generating %s!\n", filename);
}

bool PhysxHelper::DumpBinaryMetaData(const char* filename, physx::PxSerializationRegistry* sr)
{
	physx::PxDefaultFileOutputStream s(filename);
	physx::PxSerialization::dumpBinaryMetaData(s, *sr);
	return true;
}

bool PhysxHelper::LoadCollectionFile(std::vector<std::string>& filenames, PhysicsWorld* physicsWorld)
{
	physx::PxCooking*               ck = physicsWorld->GetPhysicsEngine()->GetCooking();
	physx::PxSerializationRegistry* sr = physicsWorld->GetPhysicsEngine()->GetSerializationRegistry();

	// collection that may have shared objects
	physx::PxCollection*            sharedCollection = NULL;

	for (int i = 0; i < filenames.size(); i++)
	{
		const char*	filename = filenames[i].c_str();

		bool isBinary;
		bool validFile = PhysxHelper::CheckFile(isBinary, filename);

		if (!validFile)
			return false;

		physx::PxDefaultFileInputData inputStream(filename);
		physx::PxCollection* collection = PhysxHelper::DeserializeCollection(inputStream, isBinary, sharedCollection, sr, ck);
		if (!collection)
		{
			printf("[ERROR] deserialization failure! filename: %s\n", filename);
			return false;
		}
		else
		{
			printf("Loaded: \"%s\"\n", filename);
		}

		physicsWorld->GetPxScene()->addCollection(*collection);

		if (i == 0)
		{
			sharedCollection = collection;
		}
		else
		{
			collection->release();
		}
	}

	if (sharedCollection)
	{
		sharedCollection->release();
	}

	return true;
}


void PhysxHelper::CreateRenderObjectFromShapes(std::vector<RenderObject> &objects, const std::map<physx::PxShape*, std::pair<uint, physx::PxRigidActor*>> &pxShapesMap)
{
	for (auto iter = pxShapesMap.begin(); iter != pxShapesMap.end(); iter++)
	{
		auto shape = iter->first;
		auto id = iter->second.first;
		auto parentActor = iter->second.second;

		auto shapePos = physx::PxShapeExt::getGlobalPose(*shape, *parentActor);
		auto posMat(shapePos);
		auto geomHd = shape->getGeometry();

		auto simulationFilterData = shape->getSimulationFilterData();
		auto queryFilterData = shape->getQueryFilterData();
		glm::vec3 color = PhysxHelper::CastPhysxFilterDataToColor(simulationFilterData);

		objects.push_back(PhysxHelper::CreateRenderObjectFromPxGeometry(id, geomHd, posMat, color));
	}
}

RenderObject PhysxHelper::CreateRenderObjectFromPxGeometry(uint id, const physx::PxGeometryHolder& geomHd, const physx::PxMat44& posMat, glm::vec3 color)
{
	switch (geomHd.getType())
	{
	case physx::PxGeometryType::eSPHERE:
	{
		PhysxHelper::sphereNum++;
		// TODO
	}
	break;
	case physx::PxGeometryType::ePLANE:
	{
		PhysxHelper::planeNum++;
		// TODO
	}
	break;
	case physx::PxGeometryType::eCAPSULE:
	{
		PhysxHelper::capsuleNum++;

		const physx::PxCapsuleGeometry& capsuleGeom = geomHd.capsule();
		float halfHeight = capsuleGeom.halfHeight;
		float radius = capsuleGeom.radius;
		uint slices = 20;
		uint stacks = 20;
		
		physx::PxMat44 mat(posMat);
		physx::PxMat44 rot(physx::PxQuat((float)M_PI / 2, physx::PxVec3(0, 0, 1)));
		physx::PxVec4 scale = physx::PxVec4(1, 1, 1, 1);
		mat = mat * rot;
		mat.scale(scale);
		glm::mat4x4 modelMatrix = PhysxHelper::PxMat44ToGlmMatrix4x4(mat);

		glm::vec3 capsuleColor(0, 0.8f, 0);

		return PhysxHelper::CreateCapsuleRenderObject(id, halfHeight, radius, slices, stacks, modelMatrix, capsuleColor);
	}
	break;
	case physx::PxGeometryType::eBOX:
	{
		PhysxHelper::boxNum++;
		// TODO
	}
	break;
	case physx::PxGeometryType::eCONVEXMESH:
	{
		PhysxHelper::convexMeshNum++;
		// TODO
		/*const physx::PxConvexMeshGeometry& convexGeom = geomHd.convexMesh();
		const physx::PxConvexMesh& mesh = *convexGeom.convexMesh;

		const physx::PxVec3 scale = convexGeom.scale.scale;

		const physx::PxU32 nbPolys = mesh.getNbPolygons();
		const physx::PxU8* polygons = mesh.getIndexBuffer();
		const physx::PxVec3* verts = mesh.getVertices();
		physx::PxU32 nbVerts = mesh.getNbVertices();

		physx::PxU32 numTotalTriangles = 0;
		for (physx::PxU32 i = 0; i < nbPolys; i++)
		{
			physx::PxHullPolygon data;
			mesh.getPolygonData(i, data);
		}*/
	}
	break;
	case physx::PxGeometryType::eTRIANGLEMESH:
	{
		PhysxHelper::triangleMeshNum++;

		const physx::PxTriangleMeshGeometry& triGeom = geomHd.triangleMesh();
		const physx::PxTriangleMesh& mesh = *triGeom.triangleMesh;

		const void* vertices = reinterpret_cast<const void*>(mesh.getVertices());
		uint vertexNum = static_cast<uint>(mesh.getNbVertices());

		const void* indices = mesh.getTriangles();
		uint indexNum = static_cast<uint>(mesh.getNbTriangles() * 3);
		bool has16BitIndices = mesh.getTriangleMeshFlags() & physx::PxTriangleMeshFlag::e16_BIT_INDICES;

		physx::PxMat44 mat(posMat);
		physx::PxVec4 scale = physx::PxVec4(triGeom.scale.scale, 1);
		mat.scale(scale);
		glm::mat4x4 modelMatrix = PhysxHelper::PxMat44ToGlmMatrix4x4(mat);

		return CreateRenderObjectWithGenNormal(id, vertices, vertexNum, indices, indexNum, modelMatrix, color, has16BitIndices);
	}
	break;
	case physx::PxGeometryType::eHEIGHTFIELD:
	{
		PhysxHelper::heightFieldNum++;
		// TODO
	}
	break;
	}

	return RenderObject();
}

RenderObject PhysxHelper::CreateCapsuleRenderObject(uint id, float halfHeight, float radius, uint slices, uint stacks, const glm::mat4x4& posMat, glm::vec3 color)
{
	if (stacks % 2 > 0)
	{
		stacks += 1;
	}
	uint halfStacks = stacks / 2;
	uint verticesNum = slices * (halfStacks - 1) * 2 + 2;
	uint trianglesNum = slices * (halfStacks - 1) * 4;

	// create vertices
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
	return CreateRenderObject(id, vertices, verticesNum, indices, trianglesNum * 3, posMat, color);
}

glm::vec3 PhysxHelper::CastPhysxFilterDataToColor(physx::PxFilterData data)
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


bool PhysxHelper::RayCast(const Ray &ray, physx::PxScene* pxScene, physx::PxRaycastBuffer& hitinfo)
{
	auto origin = PhysxHelper::GlmVector3ToPxVec3(ray.GetOrigin());
	auto direction = PhysxHelper::GlmVector3ToPxVec3(ray.GetDirection());
	auto maxDistance = 10000;

	return pxScene->raycast(origin, direction, maxDistance, hitinfo);
}
