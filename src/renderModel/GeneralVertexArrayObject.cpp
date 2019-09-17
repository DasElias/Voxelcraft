#include "GeneralVertexArrayObject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BufferOffsetMacro.h"
#include "VertexArrayObject.h"

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	GeneralVertexArrayObject::GeneralVertexArrayObject(constFloatVector data, const std::vector<int>& indicies, constFloatVector textureCoords, constFloatVector normalCoords) {
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		vertexCount = data.size() / 3;

		generateIndiciesVao(indicies);
		generateVbo(0, 3, data);
		generateVbo(1, 2, textureCoords);
		generateVbo(2, 3, normalCoords);

		glBindVertexArray(0);
	}

	GeneralVertexArrayObject::GeneralVertexArrayObject(constFloatVector data, constFloatVector textureCoords, int dimensions) {
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		vertexCount = data.size() / dimensions;

		generateVbo(0, dimensions, data);
		generateVbo(1, 2, textureCoords);

		glBindVertexArray(0);
	}

	GeneralVertexArrayObject::GeneralVertexArrayObject(constFloatVector data, constFloatVector textureCoords, constFloatVector normals) {
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		vertexCount = data.size() / 3;

		generateVbo(0, 3, data);
		generateVbo(1, 2, textureCoords);
		generateVbo(2, 3, normals);

		glBindVertexArray(0);
	}

	GeneralVertexArrayObject::GeneralVertexArrayObject(constFloatVector data, int dimensions) {
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		vertexCount = data.size() / dimensions;

		generateVbo(0, dimensions, data);

		glBindVertexArray(0);
	}

	GeneralVertexArrayObject::~GeneralVertexArrayObject() {
		glDeleteVertexArrays(1, &vaoId);
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	std::uint32_t GeneralVertexArrayObject::generateVbo(int indexInVao, int sizePerVertex, constFloatVector data) {
		std::uint32_t vboId;
		glGenBuffers(1, &vboId);

		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(indexInVao);
		glVertexAttribPointer(indexInVao, sizePerVertex, GL_FLOAT, false, 0, BUFFER_OFFSET(0));

		return vboId;
	}

	void GeneralVertexArrayObject::generateIndiciesVao(const std::vector<int>& indicies) {
		std::uint32_t vboId;
		glGenBuffers(1, &vboId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(indicies[0]), &indicies[0], GL_STATIC_DRAW);
	}

	std::uint32_t GeneralVertexArrayObject::getVertexArrayObjectId() {
		return vaoId;
	}

	std::size_t GeneralVertexArrayObject::getIndiciesCount() {
		return vertexCount;
	}
}


