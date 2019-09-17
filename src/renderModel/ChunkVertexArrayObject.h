#pragma once

#include <vector>

#include "VertexArrayObject.h"
#include "ChunkVaoData.h"

namespace vc {
	namespace renderingModel {
		class ChunkVertexArrayObject : public VertexArrayObject {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				virtual ~ChunkVertexArrayObject() = default;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				virtual void updateData(std::vector<ChunkVaoData>& data) =0;
		};
	}
}
