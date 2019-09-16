#pragma once
#include "../../renderModel/ChunkVertexArrayObject.h"

namespace vc {
	namespace renderingModel {
		class AbstractChunkVaoManager {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				virtual ~AbstractChunkVaoManager() =0;
				AbstractChunkVaoManager(const AbstractChunkVaoManager&) = delete;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			virtual ChunkVertexArrayObject& getNewVao() =0;
			virtual void returnVao(ChunkVertexArrayObject& vao) =0;
		};
	}
}
