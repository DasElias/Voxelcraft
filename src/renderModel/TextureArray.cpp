#define NOMINMAX
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextureArray.h"

#include <algorithm>
#include <stb_image.h>
#include <iostream>

namespace vc {

	TextureArray::TextureArray(std::vector<std::string>& textures) :
			width(64),
			height(64) {

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texId);


		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);


		// set anisotropic filtering
		if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
			GLfloat max;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
			max = std::min(max, ANISOTROPIC_FILTERING_AMOUNT);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, max);
		}



		// TODO use multisampling?
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, GLsizei(textures.size()), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		for(uint32_t counter = 0; counter < textures.size(); counter++) {
			//TODO does this method query the width/height?
			unsigned char* img = stbi_load(textures[counter].c_str(), &width, &height, 0, STBI_rgb_alpha);
			if(img == nullptr) {
				throw std::runtime_error(("there is no image at "+textures[counter]).c_str());
			}
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, counter, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, img);
		}


		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	uint32_t TextureArray::getTexId() const {
		return texId;
	}

	int32_t TextureArray::getWidth() const {
		return width;
	}

	int32_t TextureArray::getHeight() const {
		return height;
	}

	void TextureArray::cleanUp() {
		glDeleteTextures(1, &texId);
	}

}
