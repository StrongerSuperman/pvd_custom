#include "logic/Scene.hpp"


Scene::Scene():
	m_MeshCounter(new MeshCounter),
	m_PickedShapeIds({})
	
{
	m_Camera = new Camera;
	m_Renderer = new Renderer;
	m_Physics = new Physics;

	m_Physics->Initialize();
	m_Renderer->Init();
}

Scene::~Scene()
{
	delete m_Camera;
	delete m_Renderer;
	delete m_Physics;
	delete m_MeshCounter;
}


void Scene::Load(const std::vector<std::string>& filenames)
{
	// load colleciton
	if (!LoadCollectionFile(filenames, m_Physics->GetPhysicsEngine(), m_Physics->GetPxScene()))
	{
		return;
	}

	// load actors
	auto actorsNum = m_Physics->GetPxScene()->getNbActors(
		physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);

	m_Actors.resize(actorsNum);
	m_Physics->GetPxScene()->getActors(
		physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC,
		reinterpret_cast<physx::PxActor**>(&m_Actors[0]), actorsNum);

	// create scene objects
	m_Renderer->Clear();
	m_SceneObjects.clear();
	m_MeshCounter->Reset();
	int counter = 0;
	for (int i = 0; i < m_Actors.size(); i++)
	{
		physx::PxU32 nbShapes = m_Actors[i]->getNbShapes();
		std::vector<physx::PxShape*> shapes(nbShapes);
		m_Actors[i]->getShapes(&shapes[0], nbShapes);
		//bool sleeping = m_Actors[i]->is<physx::PxRigidDynamic>() ? m_Actors[i]->is<physx::PxRigidDynamic>()->isSleeping() : false;

		for (physx::PxU32 j = 0; j < nbShapes; j++)
		{
			auto id = counter++;
			auto shapePos = physx::PxShapeExt::getGlobalPose(*shapes[j], *m_Actors[i]);
			auto geomHd = shapes[j]->getGeometry();

			SceneObject sceneObject;
			sceneObject.id = id;
			sceneObject.physicsData.shape = shapes[j];
			sceneObject.physicsData.parentActor = m_Actors[i];
			sceneObject.physicsData.simulationFilterData = shapes[j]->getSimulationFilterData();
			sceneObject.physicsData.queryFilterData = shapes[j]->getQueryFilterData();
			sceneObject.renderData = CreateRenderObjectFromPxGeometry(id, geomHd, shapePos, m_MeshCounter);

			m_SceneObjects.push_back(sceneObject);
			m_ShapeMap[shapes[j]] = sceneObject;
		}
	}
	pintMeshCounter();

	// zoom to scene box
	m_Camera->ZoomToBox(GetAABB());
}

physx::PxBounds3 Scene::GetAABB() const
{
	std::vector<physx::PxBounds3> worldBounds;
	for each(auto actor in GetActors())
	{
		worldBounds.push_back(actor->getWorldBounds());
	}
	return CalculateAABB(worldBounds);
}

void Scene::Render()
{
	for each (auto &object in m_SceneObjects)
	{
		m_Renderer->Render(object.renderData, *m_Camera);
	}
	for each (auto &object in m_RenderObjectsLine)
	{
		m_Renderer->Render(object, *m_Camera);
	}
}


void Scene::SetShadeType(ShadeType shadeType)
{
	m_ShadeType = shadeType;
	shadeObjectByShadeType();
}

void Scene::SetFilterType(FilterType filterType)
{
	m_FilterType = filterType;
}

void Scene::ShadeObjectByBitOp(std::vector<int>& words, BitOpType logicOpType)
{
	assert(words.size() == 4);
	shadeObjectByBitOpType(words, logicOpType);
}


void Scene::SetPickedShapeIds(std::vector<int>& ids)
{
	m_PickedShapeIds = ids;
	m_Renderer->SetPickedRenderObjectIds(ids);
}

void Scene::SetPickedShapes(ShapesList& shapes)
{
	std::vector<int> ids;
	for each (auto &shape in shapes)
	{
		ids.push_back(m_ShapeMap[shape].id);
	}
	SetPickedShapeIds(ids);
}

physx::PxShape* Scene::OnCameraRayCast()
{
	physx::PxRaycastBuffer hitinfo;
	auto isHit = RayCast(m_Camera->GetMouseClickRay(), GetPhysics()->GetPxScene(), hitinfo);
	if (isHit)
	{
		m_PickedShapeIds = { static_cast<int>(m_ShapeMap[hitinfo.block.shape].id) };
	}
	else
	{
		m_PickedShapeIds = {};
	}
	SetPickedShapeIds(m_PickedShapeIds);

	// for debug ray pickup function
	//genRenderObjectRay(m_Camera->GetMouseClickRay());

	return hitinfo.block.shape;
}


void Scene::shadeObjectByShadeType()
{
	FilterDataMap filterDataList;
	for each(auto &object in m_SceneObjects)
	{
		if (m_ShadeType == SimulationGroup or m_ShadeType == SimulationColor)
		{
			filterDataList[object.id] = object.physicsData.simulationFilterData;
		}
		else if (m_ShadeType == QueryGroup or m_ShadeType == QueryColor)
		{
			filterDataList[object.id] = object.physicsData.queryFilterData;
		}
	}
	auto colorMap = m_Physics->CalcShadeTypeColorMap(filterDataList, m_ShadeType);
	for each(auto &object in m_SceneObjects)
	{
		if (colorMap.find(object.id) != colorMap.end())
		{
			object.renderData.renderData.color = colorMap[object.id];
		}
	}
}

void Scene::shadeObjectByBitOpType(std::vector<int>& words, BitOpType logicOpType)
{
	FilterDataMap filterDataList;
	for each(auto &object in m_SceneObjects)
	{
		if (m_FilterType == Simulation)
		{
			filterDataList[object.id] = object.physicsData.simulationFilterData;
		}
		else if (m_FilterType == Query)
		{
			filterDataList[object.id] = object.physicsData.queryFilterData;
		}
	}
	auto colorMap = m_Physics->CalcBitOpTypeColorMap(filterDataList, words, logicOpType);
	for each(auto &object in m_SceneObjects)
	{
		if (colorMap.find(object.id) != colorMap.end())
		{
			object.renderData.renderData.color = colorMap[object.id];
		}
	}
}


void Scene::genRenderObjectRay(const Ray& ray)
{
	m_RenderObjectsLine.clear();
	m_RenderObjectsLine.push_back(CreateRenderObjectFromRay(ray));
}

void Scene::pintMeshCounter()
{
	qDebug() << "---------------------------";
	qDebug() << "[sphereNum]: " << m_MeshCounter->sphereNum;
	qDebug() << "[planeNum]: " << m_MeshCounter->planeNum;
	qDebug() << "[capsuleNum]: " << m_MeshCounter->capsuleNum;
	qDebug() << "[boxNum]: " << m_MeshCounter->boxNum;
	qDebug() << "[convexMeshNum]: " << m_MeshCounter->convexMeshNum;
	qDebug() << "[triangleMeshNum]: " << m_MeshCounter->triangleMeshNum;
	qDebug() << "[heightFieldNum]: " << m_MeshCounter->heightFieldNum;
	qDebug() << "---------------------------";
}
