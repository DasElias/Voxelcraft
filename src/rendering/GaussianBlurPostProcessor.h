#pragma once
#include <memory>

#include "../renderModel/FrameBufferObject.h"
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include "PostProcessor.h"

namespace vc {
	namespace rendering {
		class GaussianBlurPostProcessor : public PostProcessor {
			
			// ----------------------------------------------------------------------
			// -----------------------------CHILD-CLASS------------------------------
			// ----------------------------------------------------------------------
			private:
				class HorizontalBlurShader : public VertexFragmentShaderProgram {
					// ----------------------------------------------------------------------
					// --------------------------------FIELDS--------------------------------
					// ----------------------------------------------------------------------
					private:
						int location_fboWidth;

					// ----------------------------------------------------------------------
					// -----------------------------CONSTRUCTORS-----------------------------
					// ----------------------------------------------------------------------
					public:
						HorizontalBlurShader();

					// ----------------------------------------------------------------------
					// -------------------------------METHODS--------------------------------
					// ----------------------------------------------------------------------
					public:
						void setFboWidth(float width);
				};


				class VerticalBlurShader : public VertexFragmentShaderProgram {
					// ----------------------------------------------------------------------
					// --------------------------------FIELDS--------------------------------
					// ----------------------------------------------------------------------
					private:
						int location_fboHeight;

					// ----------------------------------------------------------------------
					// -----------------------------CONSTRUCTORS-----------------------------
					// ----------------------------------------------------------------------
					public:
						VerticalBlurShader();

					// ----------------------------------------------------------------------
					// -------------------------------METHODS--------------------------------
					// ----------------------------------------------------------------------
					public:
						void setFboHeight(float height);
				};

				class FinalRenderingShader : public VertexFragmentShaderProgram {
					// ----------------------------------------------------------------------
					// -----------------------------CONSTRUCTORS-----------------------------
					// ----------------------------------------------------------------------
					public:
						FinalRenderingShader();
				};




		
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				std::unique_ptr<vc::renderingModel::FrameBufferObject> horizontalFbo, verticalFbo, horizontalFbo2, verticalFbo2;
				HorizontalBlurShader horizontalShader;
				VerticalBlurShader verticalShader;
				FinalRenderingShader finalRenderingShader;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				GaussianBlurPostProcessor();
				~GaussianBlurPostProcessor();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				void renderHorizontalBlur(vc::renderingModel::FrameBufferObject& target, uint32_t textureId);
				void renderVerticalBlur(vc::renderingModel::FrameBufferObject& target, uint32_t textureId);

			public:
				void render(uint32_t textureId, bool updateFbos) override;
		};
	}
}
