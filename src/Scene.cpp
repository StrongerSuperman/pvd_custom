#include "Scene.h"

Scene::Scene():
	m_PhysicsWorld(new PhysicsWorld)
{
	m_PhysicsWorld->Initialize();
}

Scene::~Scene()
{
	delete m_PhysicsWorld;
}


void Scene::Load(std::vector<std::string> filenames)
{
	// load colleciton
	if (!PhysxHelper::LoadCollectionFile(filenames, m_PhysicsWorld))
	{
		return;
	}

	PhysxHelper::sphereNum = 0;
	PhysxHelper::planeNum = 0;
	PhysxHelper::capsuleNum = 0;
	PhysxHelper::boxNum = 0;
	PhysxHelper::convexMeshNum = 0;
	PhysxHelper::triangleMeshNum = 0;
	PhysxHelper::heightFieldNum = 0;

	// load actors
	auto actorsNum = m_PhysicsWorld->GetPxScene()->getNbActors(
		physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	Q_ASSERT(actorsNum);
	m_Actors.resize(actorsNum);
	m_PhysicsWorld->GetPxScene()->getActors(
		physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC,
		reinterpret_cast<physx::PxActor**>(&m_Actors[0]), actorsNum);

	// load shapesMap
	uint counter = 0;
	for (int i = 0; i < m_Actors.size(); i++)
	{
		physx::PxU32 nbShapes = m_Actors[i]->getNbShapes();
		std::vector<physx::PxShape*> shapes(nbShapes);
		m_Actors[i]->getShapes(&shapes[0], nbShapes);
		//bool sleeping = m_Actors[i]->is<physx::PxRigidDynamic>() ? m_Actors[i]->is<physx::PxRigidDynamic>()->isSleeping() : false;

		for (physx::PxU32 j = 0; j < nbShapes; j++)
		{
			std::pair<uint, physx::PxRigidActor*> shapesData;
			shapesData.first = counter++;
			shapesData.second = m_Actors[i];
			m_PxShapesMap[shapes[j]] = shapesData;
		}
	}

	std::cout << "---------------------------";
	std::cout << "[sphereNum]: " << PhysxHelper::sphereNum;
	std::cout << "[planeNum]: " << PhysxHelper::planeNum;
	std::cout << "[capsuleNum]: " << PhysxHelper::capsuleNum;
	std::cout << "[boxNum]: " << PhysxHelper::boxNum;
	std::cout << "[convexMeshNum]: " << PhysxHelper::convexMeshNum;
	std::cout << "[triangleMeshNum]: " << PhysxHelper::triangleMeshNum;
	std::cout << "[heightFieldNum]: " << PhysxHelper::heightFieldNum;
	std::cout << "---------------------------";
}

physx::PxBounds3 Scene::GetAABB() const
{
	std::vector<physx::PxBounds3> worldBounds;
	for each(auto actor in GetActors())
	{
		worldBounds.push_back(actor->getWorldBounds());
	}
	return PhysxHelper::CalculateAABB(worldBounds);
}
