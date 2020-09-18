#pragma once


class MeshCounter
{
public:
	explicit MeshCounter();

	void Reset();

	int sphereNum;
	int planeNum;
	int capsuleNum;
	int boxNum;
	int convexMeshNum;
	int triangleMeshNum;
	int heightFieldNum;
};
