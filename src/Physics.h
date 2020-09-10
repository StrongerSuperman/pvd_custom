#pragma once

#include <iostream>

#include <foundation/PxFoundation.h>
#include <foundation/PxFoundationVersion.h>
#include <foundation/PxErrors.h>
#include <foundation/PxErrorCallback.h>
#include <foundation/PxAllocatorCallback.h>
#include <PxPhysicsVersion.h>
#include <PxPhysics.h>
#include <PxSceneDesc.h>
#include <PxScene.h>
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

class Physics
{
public:
	explicit Physics();
	virtual ~Physics();

	void InitPhysics();
	void ResetPhysics();

	inline physx::PxPhysics*                 GetPhysics()                 const { return m_Physics;}
	inline physx::PxCooking*                 GetCooking()                 const { return m_Cooking; }
	inline physx::PxSerializationRegistry*   GetSerializationRegistry()   const { return m_SerializationRegistry;}
	inline physx::PxAllocatorCallback*       GetAllocator()                     { return &m_AllocatorCallback; }
	inline void SetActiveScene(IPhysicsSceneProvider* scene)                    { m_ActiveScene = scene; }

	void StartSimulate();

private:
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
