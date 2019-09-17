#include "ChunkVaoData.h"

namespace vc::renderingModel {
	ChunkVaoData::ChunkVaoData(float coordX, float coordY, float coordZ, short texId_facing) :
			coordX(coordX), 
			coordY(coordY),
			coordZ(coordZ),
			texId_facing(texId_facing) {
	}

}