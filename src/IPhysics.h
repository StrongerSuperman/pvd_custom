#pragma once

#include "PhysicsEngine.h"


class IPhysics : public IPhysicsSceneProvider
{
public:
	explicit IPhysics();
	virtual ~IPhysics();

	inline PhysicsEngine*  GetPhysicsEngine() const          { return PhysicsEngine::GetInstance(); };
	inline physx::PxScene* GetPxScene()       const override { return m_PxScene;                    };

	virtual void Initialize() override;
	virtual void Deinitialize() override;
	virtual void Update() override;

protected:
	physx::PxScene*    m_PxScene;
	physx::PxMaterial* m_DefaultMaterial;
};
