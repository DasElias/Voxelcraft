#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DefaultChunkVao.h"

#include "BufferOffsetMacro.h"
#include <iostream>

namespace vc::renderingModel {
	namespace {
#pragma pack(push, 1)
		struct DataToUpload {
			GLfloat coordX;
			GLfloat coordY;
			GLfloat coordZ;
			GLushort texId_facing;
		};
#pragma pack(pop)
	}
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	DefaultChunkVao::DefaultChunkVao(bool shouldInitialize) {
		if (shouldInitialize) init();
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
		glGenBuffers(1, &posDataVboId);
		glBindBuffer(GL_ARRAY_BUFFER, posDataVboId);

		// allocate memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * DATA_LENGTH, nullptr, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);



		glGenBuffers(1, &texFacingVboId);
		glBindBuffer(GL_ARRAY_BUFFER, texFacingVboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint16_t) * TEXTUREID_FACING_LENGTH, nullptr, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 1, GL_UNSIGNED_SHORT, 0, 0);

		// unbind vao
		glBindVertexArray(0);
	}

	std::uint32_t DefaultChunkVao::getVertexArrayObjectId() {
		return vaoId;
	}

	std::size_t DefaultChunkVao::getIndiciesCount() {
		return indiciesCount;
	}

	void DefaultChunkVao::updateData(std::vector<float>& posData, std::vector<uint16_t>& texFacingData) {
		if (!isInitialized) init();

		std::size_t prevLength = posData.size();

		if(posData.size() < DATA_LENGTH) posData.resize(DATA_LENGTH);
		if(texFacingData.size() < TEXTUREID_FACING_LENGTH) texFacingData.resize(TEXTUREID_FACING_LENGTH);

		glBindBuffer(GL_ARRAY_BUFFER, posDataVboId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, DATA_LENGTH * sizeof(posData[0]), &posData[0]);

		glBindBuffer(GL_ARRAY_BUFFER, texFacingVboId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, TEXTUREID_FACING_LENGTH * sizeof(texFacingData[0]), &texFacingData[0]);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->indiciesCount = prevLength / 3;
	}

	int DefaultChunkVao::getPosDataLength() const {
		return DATA_LENGTH;
	}

	int DefaultChunkVao::getTexFacingDataLength() const {
		return TEXTUREID_FACING_LENGTH;
	}

}
