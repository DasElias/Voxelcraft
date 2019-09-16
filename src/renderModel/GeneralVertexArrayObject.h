#pragma once

#include <vector>

#include "VertexArrayObject.h"

namespace vc {
	namespace renderingModel {
		typedef const std::vector<float>& constFloatVector;

		class GeneralVertexArrayObject : public VertexArrayObject {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				std::uint32_t vaoId;
				std::size_t vertexCount;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				GeneralVertexArrayObject(constFloatVector data, const std::vector<int>& indicies, constFloatVector textureCoords, constFloatVector normalCoords);
				GeneralVertexArrayObject(constFloatVector data, constFloatVector textureCoords, int dimensions);
				GeneralVertexArrayObject(constFloatVector data, constFloatVector textureCoords, constFloatVector normals);
				GeneralVertexArrayObject(constFloatVector data, int dimensions);
				~GeneralVertexArrayObject();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				std::uint32_t generateVbo(int indexInVao, int sizePerVertex, constFloatVector data);
				void generateIndiciesVao(const std::vector<int>& indicies);

			public:
				std::uint32_t getVertexArrayObjectId() override;
				std::size_t getIndiciesCount() override;
		};
	}
}
