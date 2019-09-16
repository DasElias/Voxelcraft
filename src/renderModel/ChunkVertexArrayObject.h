#pragma once

#include <vector>

#include "VertexArrayObject.h"

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
				virtual void updateData(std::vector<float>& posData, std::vector<uint16_t>& texFacingData) =0;

				virtual int getPosDataLength() const =0;
				virtual int getTexFacingDataLength() const =0;
		};
	}
}
