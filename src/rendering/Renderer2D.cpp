#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer2D.h"

#include "shaderPrograms/shaders/2d_vertex.glsl"
#include "shaderPrograms/shaders/2d_fragment.glsl"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


namespace vc {
	std::vector<float> const Renderer2D::POSITIONS = { {-1, 1, -1, -1, 1, 1, 1, -1} };
	std::vector<float> const Renderer2D::TEXTURE_COORDS = {{0, 0, 0, 1, 1, 0, 1, 1}};

	Renderer2D::Renderer2DShader::Renderer2DShader() :
			VertexFragmentShaderProgram(
				twod_vertex,
				twod_fragment,
				[this]() {
					this->createShaderVariable(0, "position");
					this->createShaderVariable(1, "texCoords");
			}) {
		
		this->location_transformationMatrix = createUniformLocation("transformationMatrix");
	}

	void Renderer2D::Renderer2DShader::loadTransformationMatrix(glm::mat4x4 transformationMatrix) {

		loadMatrix4f(location_transformationMatrix, transformationMatrix);
	}

	Renderer2D::Renderer2D() :
			quad(POSITIONS, TEXTURE_COORDS, 2) {
	}

	glm::mat4x4 Renderer2D::computeTransformationMatrix(const std::shared_ptr<egui::PositionableElement>& elem) const {
		//TODO compute transformation matrix
		glm::mat4x4 mat(1);
		mat = glm::scale(mat, {elem->getComputedWidth(), elem->getComputedHeight(), 0});
		mat = glm::translate(mat, {elem->getAbsXMargin(), elem->getAbsYMargin(), 0});

		return mat;
	}

	void Renderer2D::render(std::shared_ptr<egui::PositionableElement> elem) {
		shader.startProgram();

		shader.loadTransformationMatrix(computeTransformationMatrix(elem));
		glBindVertexArray(quad.getVertexArrayObjectId());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.getIndiciesCount());
		shader.stopProgram();
	}
}
