#pragma once

#include <array>
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include "../model/Player.h"
#include "../model/particles/ParticleList.h"
#include "../renderModel/GeneralVertexArrayObject.h"
#include "../renderModel/InstancedRenderingVbo.h"


namespace vc {
	namespace rendering {
		class ParticleRenderer {

			// ----------------------------------------------------------------------
			// -----------------------------CHILD-CLASS------------------------------
			// ----------------------------------------------------------------------
			private:
				class ParticleShader : public VertexFragmentShaderProgram {
					// ----------------------------------------------------------------------
					// --------------------------------FIELDS--------------------------------
					// ----------------------------------------------------------------------
					private:
						int location_projectionMatrix;
						int location_textureRowCounts;

					// ----------------------------------------------------------------------
					// -----------------------------CONSTRUCTORS-----------------------------
					// ----------------------------------------------------------------------
					public:
						ParticleShader();
						~ParticleShader();

					// ----------------------------------------------------------------------
					// -------------------------------METHODS--------------------------------
					// ---------------------------------------------------------------------
					public:
						void loadProjectionMatrix(glm::mat4x4 projectionMatrix);
						void loadTextureRowsCount(int rows);
				};



		
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			static std::array<float, 8> const VERTICES;
			static int const MAX_PARTICLES = 10000;
			static int const DATA_SIZE_PER_PARTICLE = 21;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				ParticleShader shader;
				vc::renderingModel::GeneralVertexArrayObject vao;
				vc::renderingModel::InstancedRenderingVbo instancedRenderingVbo;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ParticleRenderer();
				ParticleRenderer(const ParticleRenderer&);
				~ParticleRenderer();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void render(vc::model::ParticleList& particleList, vc::model::Player& player);


		};
	}
}
