#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "PostProcessor.h"

namespace vc::rendering {
	// ----------------------------------------------------------------------
	// ----------------------------STATIC-FIELDS-----------------------------
	// ----------------------------------------------------------------------
	std::vector<float> const PostProcessor::POSITIONS = {{-1, 1, -1, -1, 1, 1, 1, -1}};
	PostProcessor::PostProcessor() :
			quad(POSITIONS, 2) {
	}

	void PostProcessor::render(uint32_t textureId, bool updateFbos) {
		glBindTexture(GL_TEXTURE_2D, textureId);
		glBindVertexArray(quad.getVertexArrayObjectId());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

}

