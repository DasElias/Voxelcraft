#pragma once
#include "ChunkVertexArrayObject.h"

namespace vc {
	namespace renderingModel {
		class AbstractChunkVaoManager {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				AbstractChunkVaoManager() = default;
				AbstractChunkVaoManager(const AbstractChunkVaoManager&) = delete;
				virtual ~AbstractChunkVaoManager() = default;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				virtual ChunkVertexArrayObject* getNewVao() =0;
				virtual void returnVao(ChunkVertexArrayObject& vao) =0;
		};
	}
}
