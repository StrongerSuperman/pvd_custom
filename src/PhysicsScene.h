#pragma once

#include <PxMaterial.h>
#include <common/PxTolerancesScale.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxDefaultSimulationFilterShader.h>

#include "Physics.h"

class PhysicsScene : public IPhysicsSceneProvider
{
public:
	explicit PhysicsScene();
	virtual ~PhysicsScene();

	inline Physics*                GetPhysics() const { return m_Physics; };
	inline virtual physx::PxScene* GetPxScene() const { return m_PxScene; };

	virtual void Initialize();
	virtual void Deinitialize();
	virtual void Update();

protected:
	Physics*           m_Physics;
	physx::PxScene*    m_PxScene;
	physx::PxMaterial* m_DefaultMaterial;
};
