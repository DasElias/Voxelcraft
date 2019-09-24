#pragma once
#include <mutex>
#include <vector>
#include <queue>

#include "DefaultChunkVao.h"
#include "AbstractChunkVaoManager.h"

namespace vc {
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
