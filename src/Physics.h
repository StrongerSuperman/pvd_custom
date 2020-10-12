#pragma once

#include <set>
#include <map>
#include <vector>

#include <glm/glm.hpp>

#include "PhysicsWorld.h"


using ColorMap = std::map<int, glm::vec3>;
using FilterDataMap = std::map<int, physx::PxFilterData>;
using WordColorMap = std::map<std::pair<int, int>, glm::vec3>;

enum ShadeType
{
	SimulationGroup,
	SimulationColor,
	QueryGroup,
	QueryColor,
};

enum FilterType
{
	Simulation,
	Query,
};

enum LogicOpType
{
	Or,
	And,
};

class Physics : public PhysicsWorld
{
public:
	explicit Physics();
	virtual ~Physics();

	ColorMap CalcShadeTypeColorMap(FilterDataMap& filterDataMap, ShadeType shadeType);
	WordColorMap CalcWordColorMap(std::set<int>& word0Set, std::set<int>& word1Set);

	ColorMap CalcLoicOpTypeColorMap(FilterDataMap& filterDataMap, std::vector<int>& words, LogicOpType logicOpType);

	physx::PxFilterFlags SimulateShader(const physx::PxFilterData& filter0, const physx::PxFilterData& filter1);
};
