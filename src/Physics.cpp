#include "Physics.h"

Physics::Physics()
{
}

Physics::~Physics()
{
}


ColorMap Physics::CalcFilterDataColorMap(FilterDataMap& filterDataMap, ShadeWay shadeWay)
{
	std::set<int> word0Set;
	std::set<int> word1Set;
	std::set<int> word2Set;
	std::set<int> word3Set;
	for (auto iter = filterDataMap.begin(); iter != filterDataMap.end(); iter++)
	{
		auto filterData = iter->second;
		word0Set.insert(filterData.word0);
		word1Set.insert(filterData.word1);
		word2Set.insert(filterData.word2);
		word3Set.insert(filterData.word3);
	}

	WordColorMap wordColorMap;
	switch (shadeWay)
	{
	case SimulateGroup:
	{
		wordColorMap = CalcWordColorMap(word0Set, word1Set);
	}
	break;
	case SimulateColor:
	{
		wordColorMap = CalcWordColorMap(word2Set, word3Set);
	}
	break;
	case QueryGroup:
	{
		wordColorMap = CalcWordColorMap(word0Set, word1Set);
	}
	break;
	case QueryColor:
	{
		wordColorMap = CalcWordColorMap(word2Set, word3Set);
	}
	break;
	}

	ColorMap ret;
	for (auto iter = filterDataMap.begin(); iter != filterDataMap.end(); iter++)
	{
		auto id = iter->first;
		auto filterData = iter->second;
		auto word0 = filterData.word0;
		auto word1 = filterData.word1;
		auto word2 = filterData.word2;
		auto word3 = filterData.word3;

		glm::vec3 color;
		switch (shadeWay)
		{
		case SimulateGroup:
		{
			std::pair<int, int> wordPair(word0, word1);
			color = wordColorMap[wordPair];
		}
		break;
		case SimulateColor:
		{
			std::pair<int, int> wordPair(word2, word3);
			color = wordColorMap[wordPair];
		}
		break;
		case QueryGroup:
		{
			std::pair<int, int> wordPair(word0, word1);
			color = wordColorMap[wordPair];
		}
		break;
		case QueryColor:
		{
			std::pair<int, int> wordPair(word2, word3);
			color = wordColorMap[wordPair];
		}
		break;
		}

		ret[id] = color;
	}

	/*ColorMap ret;
	for (auto iter = filterDataMap.begin(); iter != filterDataMap.end(); iter++)
	{
		auto id = iter->first;
		auto filterData = iter->second;
		auto word0 = filterData.word0;
		auto word1 = filterData.word1;
		auto word2 = filterData.word2;
		auto word3 = filterData.word3;

		glm::vec3 color(0.6f, 0, 0);
		color += glm::vec3(word0 % 255 / 255.0f, 0, 0);
		color += glm::vec3(0, word1 % 255 / 255.0f, 0);
		color += glm::vec3(0, 0, word2 % 255 / 255.0f);

		ret[id] = color;
	}*/
	return ret;
}

WordColorMap Physics::CalcWordColorMap(std::set<int>& word0Set, std::set<int>& word1Set)
{
	auto word0Size = word0Set.size();
	auto word1Size = word1Set.size();

	// lab color to rgb color
	auto minL = 10.0f;
	auto maxL = 80.0f;
	auto rangeL = maxL - minL;
	auto stepL = rangeL / (word0Size*word1Size);

	auto minA = -120.0f;
	auto maxA = 120.0f;
	auto rangeA = maxA - minA;
	auto stepA = rangeA / (word0Size*word1Size);

	auto minB = -120.0f;
	auto maxB = 120.0f;
	auto rangeB = maxB - minB;
	auto stepB = rangeB / (word0Size*word1Size);
	
	std::map<std::pair<int, int>, glm::vec3> wordColorMap;
	for (auto i = 0; i < word0Size; i++)
	{
		for (auto j = 0; j < word1Size; j++)
		{
			// key
			std::set<int>::iterator itWord0 = word0Set.begin();
			std::advance(itWord0, i);
			std::set<int>::iterator itWord1 = word1Set.begin();
			std::advance(itWord1, j);
			std::pair<int, int> wordPair(*itWord0, *itWord1);

			// value
			auto _l = minL + (i* word1Size + j)*stepL;
			auto _a = minA + (i* word1Size + j)*stepA;
			auto _b = minB + (i* word1Size + j)*stepB;
			auto r = abs(3.24f* _l + -1.54f*_a + -0.5f*_b) / 255.0f;
			auto g = abs(-0.97f* _l + 1.88f*_a + 0.04f*_b) / 255.0f;
			auto b = abs(0.06f* _l + -0.20f*_a + 1.06f*_b) / 255.0f;
			auto color = glm::vec3(r, g, b);

			wordColorMap[wordPair] = color;
		}
	}
	return wordColorMap;
}


/* 
   word0: group
   word1: mask
   word2: [camp | color | color | color]
   word3: [flag | color_mask | color_mask | color_mask]
*/
physx::PxFilterFlags Physics::SimulateShader(const physx::PxFilterData& filter0, const physx::PxFilterData& filter1)
{

	// group and mask
	if (!((filter0.word0 & filter1.word1) && (filter1.word0 & filter0.word1)))
		return physx::PxFilterFlag::eSUPPRESS;

	// TODO: any other way to handle camp?
	auto camp0 = filter0.word2 & 0xff000000;
	auto camp1 = filter1.word2 & 0xff000000;
	if (camp0 && camp0 == camp1)
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}

	// Only deal with colored objs, to ignore all please use group_mask
	auto color0 = filter0.word2 & 0x00ffffff;
	auto color1 = filter1.word2 & 0x00ffffff;
	auto color_mask0 = filter0.word3 & 0x00ffffff;
	auto color_mask1 = filter1.word3 & 0x00ffffff;
	if ((color0 && !(color0 & color_mask1)) || (color1 && !(color1 & color_mask0)))
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}

	return physx::PxFilterFlag::eDEFAULT;
}
