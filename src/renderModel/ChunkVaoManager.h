#pragma once
#include <mutex>
#include <vector>
#include <queue>

#include "AbstractChunkVaoManager.h"
#include "ChunkVertexArrayObject.h"
#include "DefaultChunkVao.h"

namespace vc {
	namespace renderingModel {
		class ChunkVaoManager : public AbstractChunkVaoManager {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				mutable std::mutex mutex;

				std::vector<std::unique_ptr<ChunkVertexArrayObject>> allVaos;
				std::queue<ChunkVertexArrayObject*> vacantVaos;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ChunkVaoManager(int size);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				DefaultChunkVao& addVao(bool init);

			public:
				ChunkVertexArrayObject* getNewVao() override;
				void returnVao(ChunkVertexArrayObject& vao) override;
		};
	}
}