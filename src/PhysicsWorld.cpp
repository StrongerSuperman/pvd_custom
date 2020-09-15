#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() :
	m_PxScene(nullptr),
	m_DefaultMaterial(nullptr)
{
}

PhysicsWorld::~PhysicsWorld()
{
	Deinitialize();
}


void PhysicsWorld::Initialize()
{
	physx::PxSceneDesc sceneDescriptor(GetPhysicsEngine()->GetPhysics()->getTolerancesScale());
	sceneDescriptor.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDescriptor.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(0);
	sceneDescriptor.filterShader = &physx::PxDefaultSimulationFilterShader;

	m_PxScene = GetPhysicsEngine()->GetPhysics()->createScene(sceneDescriptor);
	GetPhysicsEngine()->SetActiveScene(this);

	m_DefaultMaterial = GetPhysicsEngine()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);
}

void PhysicsWorld::Deinitialize()
{
	GetPhysicsEngine()->SetActiveScene(nullptr);

	m_DefaultMaterial->release();
	m_PxScene->release();
}

void PhysicsWorld::Update()
{
}
