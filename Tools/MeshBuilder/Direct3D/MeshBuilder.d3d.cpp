#include "../cMeshBuilder.h"

void eae6320::Assets::cMeshBuilder::AdjustWindingOrderAndUVs(uint16_t vertexCount, eae6320::Graphics::VertexFormats::sMesh  * vertexData, uint16_t indexCount, uint16_t * indexData)
{
	//Index data is being assigned
	for (size_t i = 0; i < indexCount; i++)
	{
		//Swapp the values
		if (i % 3 == 1)
		{
			uint16_t value1 = indexData[i];
			indexData[i] = indexData[i + 1];
			indexData[i + 1] = value1;
			i += 1;
		}

	}


	for (size_t i = 0; i < vertexCount; i++)
	{
		//Alter the v value
		vertexData[i].v = 1.0f - vertexData[i].v;
	}

}