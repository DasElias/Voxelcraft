#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexFragmentShaderProgram.h"

#include <boost/log/trivial.hpp>
#include <vector>
namespace vc {

	VertexFragmentShaderProgram::VertexFragmentShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode,
			std::function<void(void)> bindShaderVariables) {

		vertexShaderId = loadShader(GL_VERTEX_SHADER, vertexShaderCode);
		fragmentShaderId = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);

		bindShaderVariables();

		glLinkProgram(programId);

		glDetachShader(programId, vertexShaderId);
		glDetachShader(programId, fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		GLint isLinked = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(programId);

			BOOST_LOG_TRIVIAL(fatal) << "Couldn't link shader!";
			BOOST_LOG_TRIVIAL(fatal) << infoLog.data();
			exit(-1);
		}


		glValidateProgram(programId);
		startProgram();
	}

}
