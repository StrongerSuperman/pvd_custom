#include "FileHelper.h"


bool CheckFile(bool& isBinary, const char* filename)
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

physx::PxCollection* DeserializeCollection(physx::PxInputData& inputData, bool isBinary, physx::PxCollection* sharedCollection, physx::PxSerializationRegistry* sr, physx::PxCooking* ck)
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

void SerializeCollection(physx::PxCollection& collection, physx::PxCollection* externalRefs, const char* filename, bool toBinary, physx::PxSerializationRegistry* sr)
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

bool DumpBinaryMetaData(const char* filename, physx::PxSerializationRegistry* sr)
{
	physx::PxDefaultFileOutputStream s(filename);
	physx::PxSerialization::dumpBinaryMetaData(s, *sr);
	return true;
}

bool LoadCollectionFile(const std::vector<std::string>& filenames, PhysicsWorld* physicsWorld)
{
	physx::PxCooking*               ck = physicsWorld->GetPhysicsEngine()->GetCooking();
	physx::PxSerializationRegistry* sr = physicsWorld->GetPhysicsEngine()->GetSerializationRegistry();

	// collection that may have shared objects
	physx::PxCollection*            sharedCollection = NULL;

	for (int i = 0; i < filenames.size(); i++)
	{
		const char*	filename = filenames[i].c_str();

		bool isBinary;
		bool validFile = CheckFile(isBinary, filename);

		if (!validFile)
			return false;

		physx::PxDefaultFileInputData inputStream(filename);
		physx::PxCollection* collection = DeserializeCollection(inputStream, isBinary, sharedCollection, sr, ck);
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
