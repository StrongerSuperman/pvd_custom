#include "PhysicsScene.h"

PhysicsScene::PhysicsScene() :
	m_PxScene(nullptr),
	m_DefaultMaterial(nullptr)
{
}

PhysicsScene::~PhysicsScene()
{
	Deinitialize();
}


void PhysicsScene::Initialize()
{
	physx::PxSceneDesc sceneDescriptor(GetPhysics()->GetPhysics()->getTolerancesScale());
	sceneDescriptor.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDescriptor.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(0);
	sceneDescriptor.filterShader = &physx::PxDefaultSimulationFilterShader;

	m_PxScene = GetPhysics()->GetPhysics()->createScene(sceneDescriptor);
	GetPhysics()->SetActiveScene(this);

	m_DefaultMaterial = GetPhysics()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);
}

void PhysicsScene::Deinitialize()
{
	GetPhysics()->SetActiveScene(nullptr);

	m_DefaultMaterial->release();
	m_PxScene->release();
}

void PhysicsScene::Update()
{
}
