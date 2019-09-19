#pragma once

#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include <model\positioning\PositionableElement.h>
#include "../renderModel/GeneralVertexArrayObject.h"

namespace vc {
	class Renderer2D {
		class Renderer2DShader : public VertexFragmentShaderProgram {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				int location_transformationMatrix;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				Renderer2DShader();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void loadTransformationMatrix(glm::mat4x4 transformationMatrix);
		};



		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::vector<float> const POSITIONS;
			static std::vector<float> const TEXTURE_COORDS;


		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Renderer2DShader shader;
			GeneralVertexArrayObject quad;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Renderer2D();
			Renderer2D(const Renderer2D&) = delete;
			Renderer2D& operator=(const Renderer2D&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			glm::mat4x4 computeTransformationMatrix(const std::shared_ptr<egui::PositionableElement>& elem) const;

		public:
			void render(std::shared_ptr<egui::PositionableElement> elem);
	};
}