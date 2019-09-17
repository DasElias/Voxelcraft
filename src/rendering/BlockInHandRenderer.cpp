#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "BlockInHandRenderer.h"

#include <tl/optional.hpp>
#include <array>

#include "../model/BlockType.h"
#include "../model/TextureOrientation.h"
#include "../renderModel/BufferOffsetMacro.h"
#include "../renderModel/TextureArray.h"

#include "shaderPrograms/shaders/blockInHand_vertex.glsl"
#include "shaderPrograms/shaders/blockInHand_fragment.glsl"

using namespace vc;

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	BlockInHandRenderer::BlockInHandShader::BlockInHandShader() : 
		VertexFragmentShaderProgram(blockInHand_vertex, blockInHand_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
				this->createShaderVariable(1, "texCoords");
				this->createShaderVariable(2, "texId");
			}
		) {

		this->location_projectionMatrix = createUniformLocation("projMatrix");

	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	void BlockInHandRenderer::BlockInHandShader::loadProjectionMatrix(glm::mat4x4 projectionMatrix) {
		this->loadMatrix4f(location_projectionMatrix, projectionMatrix);
	}


	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	BlockInHandRenderer::BlockInHandVao::BlockInHandVao(const std::vector<float>& data, const std::vector<float>& texData) {
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		vertexCount = data.size() / 3;

		generateVbo(0, 3, data);
		generateVbo(1, 2, texData);
		texIdVbo = generateTexIdVbo(2, 1);

		glBindVertexArray(0);
	}

	BlockInHandRenderer::BlockInHandVao::~BlockInHandVao() {
		glDeleteVertexArrays(1, &vaoId);
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	std::uint32_t BlockInHandRenderer::BlockInHandVao::generateVbo(std::uint32_t indexInVao, std::int32_t sizePerVertex, const std::vector<float>& data) {
		if(data.size() == 0) {
			throw std::length_error("length of data mustn't be 0");
		}

		// create new object and bind it
		std::uint32_t vboId;
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		// write data
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), &data[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(indexInVao);
		glVertexAttribPointer(indexInVao, sizePerVertex, GL_FLOAT, false, 0, BUFFER_OFFSET(0));

		return vboId;
	}

	std::uint32_t BlockInHandRenderer::BlockInHandVao::generateTexIdVbo(std::uint32_t indexInVao, std::int32_t sizePerVertex) {
		std::uint32_t vboId;
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glBufferData(GL_ARRAY_BUFFER, 6 * 6 * 4, nullptr, GL_STREAM_DRAW);

		glEnableVertexAttribArray(indexInVao);
		glVertexAttribIPointer(indexInVao, sizePerVertex, GL_UNSIGNED_INT, 0, 0);

		return vboId;
	}

	std::uint32_t BlockInHandRenderer::BlockInHandVao::getVertexArrayObjectId() {
		return vaoId;
	}

	std::size_t BlockInHandRenderer::BlockInHandVao::getIndiciesCount() {
		return vertexCount;
	}

	void BlockInHandRenderer::BlockInHandVao::loadTextureIds(const std::vector<int>& texIds) {
		if(texIds.size() != 6 * 6) {
			throw std::length_error("length of data must be exactly equal to 6 * 6");
		}

		glBindBuffer(GL_ARRAY_BUFFER, texIdVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, texIds.size() * sizeof(texIds[0]), &texIds[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// ----------------------------------------------------------------------
	// ----------------------------STATIC-FIELDS-----------------------------
	// ----------------------------------------------------------------------
	namespace {
		float const MIN_X = -1;
		float const MAX_X = 1;
		float const MIN_Y = -1;
		float const MAX_Y = 1;
		float const MIN_Z = -1;
		float const MAX_Z = 1;

		std::vector<float> const CUBE_DATA = {{
			// oben
			MIN_X, MAX_Y, MIN_Z,
			MIN_X, MAX_Y, MAX_Z,
			MAX_X, MAX_Y, MAX_Z,
			MIN_X, MAX_Y, MIN_Z,
			MAX_X, MAX_Y, MAX_Z,
			MAX_X, MAX_Y, MIN_Z,

			// vorne
			MIN_X, MIN_Y, MIN_Z,
			MIN_X, MAX_Y, MIN_Z,
			MAX_X, MAX_Y, MIN_Z,
			MAX_X, MAX_Y, MIN_Z,
			MAX_X, MIN_Y, MIN_Z,
			MIN_X, MIN_Y, MIN_Z,

			// unten
			MIN_X, MIN_Y, MIN_Z,
			MAX_X, MIN_Y, MAX_Z,
			MIN_X, MIN_Y, MAX_Z,
			MIN_X, MIN_Y, MIN_Z,
			MAX_X, MIN_Y, MIN_Z,
			MAX_X, MIN_Y, MAX_Z,

			// hinten
			MIN_X, MIN_Y, MAX_Z,
			MAX_X, MAX_Y, MAX_Z,
			MIN_X, MAX_Y, MAX_Z,
			MAX_X, MAX_Y, MAX_Z,
			MIN_X, MIN_Y, MAX_Z,
			MAX_X, MIN_Y, MAX_Z,

			// links
			MIN_X, MAX_Y, MIN_Z,
			MIN_X, MIN_Y, MIN_Z,
			MIN_X, MAX_Y, MAX_Z,
			MIN_X, MAX_Y, MAX_Z,
			MIN_X, MIN_Y, MIN_Z,
			MIN_X, MIN_Y, MAX_Z,


			// rechts
			MAX_X, MAX_Y, MIN_Z,
			MAX_X, MAX_Y, MAX_Z,
			MAX_X, MIN_Y, MIN_Z,
			MAX_X, MAX_Y, MAX_Z,
			MAX_X, MIN_Y, MAX_Z,
			MAX_X, MIN_Y, MIN_Z,
		}};


		std::vector<float> const CUBE_TEXTURE = {{
			0, 1,
			0, 0,
			1, 0,
			0, 1,
			1, 0,
			1, 1,

			0, 1,
			0, 0,
			1, 0,
			1, 0,
			1, 1,
			0, 1,

			0, 1,
			1, 0,
			0, 0,
			0, 1,
			1, 1,
			1, 0,

			0, 1,
			1, 0,
			0, 0,
			1, 0,
			0, 1,
			1, 1,

			0, 0,
			0, 1,
			1, 0,
			1, 0,
			0, 1,
			1, 1,



			0, 0,
			1, 0,
			0, 1,
			1, 0,
			1, 1,
			0, 1,
		}};
	}

	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	BlockInHandRenderer::BlockInHandRenderer(TextureArray blockTextureArray) :
			blockTextureArray(blockTextureArray),
			vao(CUBE_DATA, CUBE_TEXTURE) {
	}
	BlockInHandRenderer::~BlockInHandRenderer() {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	void BlockInHandRenderer::render(const std::shared_ptr<BlockType>& type) {
		glDisable(GL_DEPTH_TEST);

		shader.startProgram();
		shader.loadProjectionMatrix(getModelProjectionMatrix());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextureArray.getTexId());

		const TextureOrientation& texOrientation = type->getTextureOrientation();
		const std::array<tl::optional<TextureFile>, 6>& files = type->getTextureFiles();
		vao.loadTextureIds({{
			files[texOrientation.getTop()].value().getGlobalTextureId(),
			files[texOrientation.getTop()].value().getGlobalTextureId(),
			files[texOrientation.getTop()].value().getGlobalTextureId(),
			files[texOrientation.getTop()].value().getGlobalTextureId(),
			files[texOrientation.getTop()].value().getGlobalTextureId(),
			files[texOrientation.getTop()].value().getGlobalTextureId(),

			files[texOrientation.getFront()].value().getGlobalTextureId(),
			files[texOrientation.getFront()].value().getGlobalTextureId(),
			files[texOrientation.getFront()].value().getGlobalTextureId(),
			files[texOrientation.getFront()].value().getGlobalTextureId(),
			files[texOrientation.getFront()].value().getGlobalTextureId(),
			files[texOrientation.getFront()].value().getGlobalTextureId(),

			files[texOrientation.getBottom()].value().getGlobalTextureId(),
			files[texOrientation.getBottom()].value().getGlobalTextureId(),
			files[texOrientation.getBottom()].value().getGlobalTextureId(),
			files[texOrientation.getBottom()].value().getGlobalTextureId(),
			files[texOrientation.getBottom()].value().getGlobalTextureId(),
			files[texOrientation.getBottom()].value().getGlobalTextureId(),

			files[texOrientation.getBack()].value().getGlobalTextureId(),
			files[texOrientation.getBack()].value().getGlobalTextureId(),
			files[texOrientation.getBack()].value().getGlobalTextureId(),
			files[texOrientation.getBack()].value().getGlobalTextureId(),
			files[texOrientation.getBack()].value().getGlobalTextureId(),
			files[texOrientation.getBack()].value().getGlobalTextureId(),

			files[texOrientation.getLeft()].value().getGlobalTextureId(),
			files[texOrientation.getLeft()].value().getGlobalTextureId(),
			files[texOrientation.getLeft()].value().getGlobalTextureId(),
			files[texOrientation.getLeft()].value().getGlobalTextureId(),
			files[texOrientation.getLeft()].value().getGlobalTextureId(),
			files[texOrientation.getLeft()].value().getGlobalTextureId(),

			files[texOrientation.getRight()].value().getGlobalTextureId(),
			files[texOrientation.getRight()].value().getGlobalTextureId(),
			files[texOrientation.getRight()].value().getGlobalTextureId(),
			files[texOrientation.getRight()].value().getGlobalTextureId(),
			files[texOrientation.getRight()].value().getGlobalTextureId(),
			files[texOrientation.getRight()].value().getGlobalTextureId(),
		}});

		glBindVertexArray(vao.getVertexArrayObjectId());
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(vao.getIndiciesCount()));

		shader.stopProgram();

		glEnable(GL_DEPTH_TEST);
	}
}


