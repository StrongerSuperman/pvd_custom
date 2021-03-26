#include "core/physics/PhysicsEngine.hpp"


void ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	std::cout << code << message << file << line;
}

void* AllocatorCallback::allocate(size_t size, const char* typeName, const char* filename, int line)
{
	(void)typeName;
	(void)filename;
	(void)line;

	size_t           alignment    = 16;
	size_t           expandedSize = size + alignment;
	size_t           unaligned    = reinterpret_cast<size_t>(malloc(expandedSize));
	size_t           mask         = (alignment - 1);
	size_t           misalignment = (unaligned & mask);
	size_t           adjustment   = alignment - misalignment;
	size_t           aligned      = unaligned + adjustment;
	unsigned char*   alignedBytes = reinterpret_cast<unsigned char*>(aligned);
	alignedBytes[-1]              = static_cast<unsigned char>(adjustment);

	return reinterpret_cast<void*>(alignedBytes);
}

void AllocatorCallback::deallocate(void* ptr)
{
	unsigned char*  alignedBytes = reinterpret_cast<unsigned char*>(ptr);
	size_t          aligned      = reinterpret_cast<size_t>(ptr);
	size_t          adjustment   = static_cast<size_t>(alignedBytes[-1]);
	size_t          unaligned    = aligned - adjustment;

	free(reinterpret_cast<void*>(unaligned));
}


PhysicsEngine* PhysicsEngine::GetInstance()
{
	static PhysicsEngine*  m_Instance;
	static std::mutex      m_Mutex;

	if (!m_Instance)
	{
		m_Mutex.lock();
		if (!m_Instance)
		{
			m_Instance = new PhysicsEngine;
		}
		m_Mutex.unlock();
	}

	return m_Instance;
}


PhysicsEngine::PhysicsEngine() :
	m_Foundation(nullptr),
	m_Physics(nullptr),
	m_Cooking(nullptr),
	m_SerializationRegistry(nullptr),
	m_ActiveScene(nullptr),
	m_DeltaTime(1.0f/60.f),
	m_AdditionalDelta(0.01f)
{
	InitPhysics();
}

PhysicsEngine::~PhysicsEngine()
{
	ResetPhysics();
}

void PhysicsEngine::InitPhysics()
{
	m_Foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_AllocatorCallback, m_ErrorCallback);
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale());
	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(m_Physics->getTolerancesScale()));
	m_SerializationRegistry = physx::PxSerialization::createSerializationRegistry(*m_Physics);
	PxInitExtensions(*m_Physics, nullptr);
}

void PhysicsEngine::ResetPhysics()
{
	PxCloseExtensions();
	m_Physics->release();
	m_Foundation->release();
	m_SerializationRegistry->release();
}

void PhysicsEngine::StartSimulate()
{
	simulate();
}

void PhysicsEngine::simulate()
{
	if (m_ActiveScene == nullptr)
		return;

	m_ActiveScene->GetPxScene()->simulate(m_DeltaTime + m_AdditionalDelta);
	m_ActiveScene->GetPxScene()->fetchResults(true);
	m_ActiveScene->Update();
}
