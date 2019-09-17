#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DefaultChunkVao.h"

#include "BufferOffsetMacro.h"
#include <iostream>

namespace vc::renderingModel {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	DefaultChunkVao::DefaultChunkVao(bool shouldInitialize) {
		if(shouldInitialize) init();
	}

	DefaultChunkVao::~DefaultChunkVao() {
		glDeleteVertexArrays(1, &vaoId);
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	void DefaultChunkVao::init() {
		isInitialized = true;

		// create and bind vao
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		// create vbo
		glGenBuffers(1, &dataVboId);
		glBindBuffer(GL_ARRAY_BUFFER, dataVboId);

		// allocate memory
		glBufferData(GL_ARRAY_BUFFER, (4 * DATA_LENGTH) + TEXTUREID_FACING_LENGTH, nullptr, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * 4 + 2, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 1, GL_UNSIGNED_SHORT, 3 * 4 + 2, BUFFER_OFFSET(3 * 4));

		// unbind vao
		glBindVertexArray(0);
	}

	std::uint32_t DefaultChunkVao::getVertexArrayObjectId() {
		return vaoId;
	}

	std::size_t DefaultChunkVao::getIndiciesCount() {
		return indiciesCount;
	}

	void DefaultChunkVao::updateData(std::vector<ChunkVaoData>& data) {
		if(!isInitialized) init();


		data.resize(vc::model::Chunk::CHUNK_SIZE * vc::model::Chunk::CHUNK_SIZE * vc::model::Chunk::CHUNK_SIZE);

		glBindBuffer(GL_ARRAY_BUFFER, dataVboId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(data[0]), &data[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->indiciesCount = data.size();
	}

}
