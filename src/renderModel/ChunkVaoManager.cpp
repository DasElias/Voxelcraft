#include "ChunkVaoManager.h"

#include <iostream>

namespace vc::renderingModel {
	ChunkVaoManager::ChunkVaoManager(int size) {
		for(int counter = 0; counter < size; ++counter) {
			vacantVaos.push(&addVao(true));
		}
	}

	DefaultChunkVao& ChunkVaoManager::addVao(bool init) {
		DefaultChunkVao* insertedVao = new DefaultChunkVao(init);
		allVaos.push_back(std::make_unique<DefaultChunkVao>(insertedVao));
		return *insertedVao;
	}

	ChunkVertexArrayObject* ChunkVaoManager::getNewVao() {
		std::lock_guard<std::mutex> lock(mutex);

		if(vacantVaos.empty()) return &addVao(false);

		ChunkVertexArrayObject* toReturn = vacantVaos.front();
		vacantVaos.pop();
		return toReturn;
	}

	void ChunkVaoManager::returnVao(ChunkVertexArrayObject& vao) {
		std::lock_guard<std::mutex> lock(mutex);
		vacantVaos.push(&vao);
	}

}

