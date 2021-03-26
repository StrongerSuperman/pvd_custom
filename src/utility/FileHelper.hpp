#pragma once

#include <vector>
#include <iostream>

#include "core/physics/PhysicsEngine.hpp"


bool CheckFile(bool& isBinary, const char* filename);
physx::PxCollection* DeserializeCollection(physx::PxInputData& inputData, bool isBinary, physx::PxCollection* sharedCollection, physx::PxSerializationRegistry* sr, physx::PxCooking* ck);
void SerializeCollection(physx::PxCollection& collection, physx::PxCollection* externalRefs, const char* filename, bool toBinary, physx::PxSerializationRegistry* sr);
bool DumpBinaryMetaData(const char* filename, physx::PxSerializationRegistry* sr);
bool LoadCollectionFile(const std::vector<std::string>& filenames, PhysicsEngine* physicsEngine, physx::PxScene* pxScene);
