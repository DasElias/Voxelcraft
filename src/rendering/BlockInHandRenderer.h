#pragma once

#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include "InHandRenderer.h"
#include "../model/BlockType.h"
#include "../renderModel/TextureArray.h"
#include "../renderModel/VertexArrayObject.h"

namespace vc {
	namespace rendering {
		class BlockInHandRenderer : public InHandRenderer {

			// ----------------------------------------------------------------------
			// -----------------------------CHILD-CLASS------------------------------
			// ----------------------------------------------------------------------
			private: 
				class BlockInHandShader : public VertexFragmentShaderProgram {
					// ----------------------------------------------------------------------
					// --------------------------------FIELDS--------------------------------
					// ----------------------------------------------------------------------
					private:
						int location_projectionMatrix;

					// ----------------------------------------------------------------------
					// -----------------------------CONSTRUCTORS-----------------------------
					// ----------------------------------------------------------------------
					public:
						BlockInHandShader();

					// ----------------------------------------------------------------------
					// -------------------------------METHODS--------------------------------
					// ----------------------------------------------------------------------
					public:
						void loadProjectionMatrix(glm::mat4x4 projectionMatrix);
				};

				class BlockInHandVao : public vc::renderingModel::VertexArrayObject {
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
						BlockInHandVao(const std::vector<float>& data, const std::vector<float>& texData);
						~BlockInHandVao();

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
				BlockInHandShader shader;
				vc::renderingModel::TextureArray blockTextureArray;
				BlockInHandVao vao;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			private:
				BlockInHandRenderer(const BlockInHandRenderer&) = delete;

			public:
				BlockInHandRenderer(vc::renderingModel::TextureArray blockTextureArray);
				~BlockInHandRenderer();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			void render(const std::shared_ptr<vc::model::BlockType>& type);
		};
	}
}
