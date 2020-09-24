#pragma once

#include <set>
#include <map>
#include <vector>

#include <glm/glm.hpp>

#include "PhysicsWorld.h"


using ColorMap = std::map<int, glm::vec3>;
using FilterDataMap = std::map<int, physx::PxFilterData>;
using WordColorMap = std::map<std::pair<int, int>, glm::vec3>;

enum ShadeWay
{
	SimulateGroup,
	SimulateColor,
	QueryGroup,
	QueryColor,
};

class Physics : public PhysicsWorld
{
public:
	explicit Physics();
	virtual ~Physics();

	ColorMap CalcFilterDataColorMap(FilterDataMap& filterDataMap, ShadeWay shadeWay);
	WordColorMap CalcWordColorMap(std::set<int>& word0Set, std::set<int>& word1Set);
	physx::PxFilterFlags SimulateShader(const physx::PxFilterData& filter0, const physx::PxFilterData& filter1);
};
