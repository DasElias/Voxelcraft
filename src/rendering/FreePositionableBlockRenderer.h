#pragma once

#include "shaderPrograms/VertexFragmentShaderProgram.h"

#include "../model/BlockType.h"
#include "../renderModel/TextureArray.h"
#include "../renderModel/VertexArrayObject.h"

namespace vc {
	class FreePositionableBlockRenderer {

		// ----------------------------------------------------------------------
		// -----------------------------CHILD-CLASS------------------------------
		// ----------------------------------------------------------------------
		private: 
			class FreePositionableBlockShader : public VertexFragmentShaderProgram {
				// ----------------------------------------------------------------------
				// --------------------------------FIELDS--------------------------------
				// ----------------------------------------------------------------------
				private:
					int location_projectionMatrix;

				// ----------------------------------------------------------------------
				// -----------------------------CONSTRUCTORS-----------------------------
				// ----------------------------------------------------------------------
				public:
					FreePositionableBlockShader();

				// ----------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				public:
					void loadProjectionMatrix(glm::mat4x4 projectionMatrix);
			};

			class FreePositionableBlockVao : public VertexArrayObject {
				// ----------------------------------------------------------------------
				// --------------------------------FIELDS--------------------------------
				// ----------------------------------------------------------------------
				private:
					std::uint32_t vaoId;
					std::size_t vertexCount;

					std::uint32_t texIdVbo;

				// ----------------------------------------------------------------------
				// -----------------------------CONSTRUCTORS-----------------------------
				// ----------------------------------------------------------------------
				public:
					FreePositionableBlockVao(const std::vector<float>& data, const std::vector<float>& texData);
					~FreePositionableBlockVao();

				// ----------------------------------------------------------------------
				// -------------------------------METHODS--------------------------------
				// ----------------------------------------------------------------------
				private:
					std::uint32_t generateVbo(std::uint32_t indexInVao, std::int32_t sizePerVertex, const std::vector<float>& data);
					std::uint32_t generateTexIdVbo(std::uint32_t indexInVao, std::int32_t sizePerVertex);

				public:
					std::uint32_t getVertexArrayObjectId() override;
					std::size_t getIndiciesCount() override;

					void loadTextureIds(const std::vector<int>& texIds);
			};






		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			FreePositionableBlockShader shader;
			FreePositionableBlockVao vao;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			FreePositionableBlockRenderer();
			FreePositionableBlockRenderer(const FreePositionableBlockRenderer&) = delete;
			~FreePositionableBlockRenderer();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(const std::shared_ptr<BlockType>& type, const TextureArray& blockTextureArray, glm::mat4x4 modelProjectionMatrix);
	};
}