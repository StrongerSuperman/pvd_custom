#pragma once

#include <mutex>
#include <iostream>

#include <foundation/PxFoundation.h>
#include <foundation/PxFoundationVersion.h>
#include <foundation/PxErrors.h>
#include <foundation/PxErrorCallback.h>
#include <foundation/PxAllocatorCallback.h>
#include <foundation/PxBounds3.h>
#include <foundation/PxMat44.h> 

#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>
#include <PxPhysicsVersion.h>
#include "PxPhysicsAPI.h"
#include <PxPhysics.h>
#include <PxSceneDesc.h>
#include <PxScene.h>
#include <PxMaterial.h>

#include <geometry/PxTriangleMesh.h>
#include <geometry/PxTriangleMeshGeometry.h>
#include <geometry/PxConvexMesh.h>
#include <geometry/PxConvexMeshGeometry.h>

#include <common/PxTolerancesScale.h>

#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxExtensionsAPI.h>


class ErrorCallback : public physx::PxErrorCallback
{
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
};

class AllocatorCallback : public physx::PxAllocatorCallback
{
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line);
	virtual void  deallocate(void* ptr);
};

class ISceneProvider
{
public:

	virtual ~ISceneProvider() {}

	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;
	virtual void Update() = 0;
};

class IPhysicsSceneProvider : public ISceneProvider
{
public:
	virtual physx::PxScene* GetPxScene() const = 0;
};

class PhysicsEngine
{
public:
	static PhysicsEngine* GetInstance();
	

	void InitPhysics();
	void ResetPhysics();

	inline physx::PxPhysics*                 GetPhysics()                 const { return m_Physics;}
	inline physx::PxCooking*                 GetCooking()                 const { return m_Cooking; }
	inline physx::PxSerializationRegistry*   GetSerializationRegistry()   const { return m_SerializationRegistry;}
	inline physx::PxAllocatorCallback*       GetAllocator()                     { return &m_AllocatorCallback; }
	inline void SetActiveScene(IPhysicsSceneProvider* scene)                    { m_ActiveScene = scene; }

	void StartSimulate();

private:
	PhysicsEngine();
	PhysicsEngine(const PhysicsEngine &) = delete;
	virtual PhysicsEngine& operator=(const PhysicsEngine &) = delete;
	~PhysicsEngine();

	static PhysicsEngine*                m_Instance;
	static std::mutex                    m_Mutex;

	physx::PxFoundation*                 m_Foundation;
	physx::PxPhysics*                    m_Physics;
	physx::PxCooking*                    m_Cooking;
	physx::PxSerializationRegistry*      m_SerializationRegistry;

	IPhysicsSceneProvider*               m_ActiveScene;

	float                                m_DeltaTime;
	float                                m_AdditionalDelta;

	ErrorCallback                        m_ErrorCallback;
	AllocatorCallback                    m_AllocatorCallback;

	void simulate();
};
