#include "PhysicsScene.h"

PhysicsScene::PhysicsScene() :
	m_Physics(new Physics),
	m_PxScene(nullptr),
	m_DefaultMaterial(nullptr)
{
}

PhysicsScene::~PhysicsScene()
{
	Deinitialize();
	delete m_Physics;
}


void PhysicsScene::Initialize()
{
	physx::PxSceneDesc sceneDescriptor(m_Physics->GetPhysics()->getTolerancesScale());
	sceneDescriptor.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDescriptor.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(0);
	sceneDescriptor.filterShader = &physx::PxDefaultSimulationFilterShader;

	m_PxScene = m_Physics->GetPhysics()->createScene(sceneDescriptor);
	m_Physics->SetActiveScene(this);

	m_DefaultMaterial = m_Physics->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);
}

void PhysicsScene::Deinitialize()
{
	m_Physics->SetActiveScene(nullptr);

	m_DefaultMaterial->release();
	m_PxScene->release();
}

void PhysicsScene::Update()
{
}
