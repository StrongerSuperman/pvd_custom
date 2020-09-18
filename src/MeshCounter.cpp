#include "MeshCounter.h"


MeshCounter::MeshCounter()
{
	Reset();
}

void MeshCounter::Reset()
{
	sphereNum = 0;
	planeNum = 0;
	capsuleNum = 0;
	boxNum = 0;
	convexMeshNum = 0;
	triangleMeshNum = 0;
	heightFieldNum = 0;
}
