#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "ShaderProgram.h"

#include <fstream>
#include <vector>
#include <iostream>
namespace vc::rendering {
	vc::rendering::ShaderProgram::ShaderProgram() {
		programId = glCreateProgram();
	}

	vc::rendering::ShaderProgram::~ShaderProgram() {
		glDeleteProgram(programId);
	}

	uint32_t vc::rendering::ShaderProgram::createUniformLocation(std::string variableName) {
		return glGetUniformLocation(programId, variableName.c_str());
	}

	void vc::rendering::ShaderProgram::createShaderVariable(uint32_t position, std::string variableName) {
		glBindAttribLocation(programId, position, variableName.c_str());
	}

	void vc::rendering::ShaderProgram::loadFloat(uint32_t uniformLocation, float value) {
		glUniform1f(uniformLocation, value);
	}

	void vc::rendering::ShaderProgram::loadInt(uint32_t uniformLocation, int value) {
		glUniform1i(uniformLocation, value);
	}

	void ShaderProgram::loadVector4f(uint32_t uniformLocation, glm::vec4 value) {
		glUniform4f(uniformLocation, value.x, value.y, value.z, value.a);
	}

	void vc::rendering::ShaderProgram::loadVector3f(uint32_t uniformLocation, glm::vec3 value) {
		glUniform3f(uniformLocation, value.x, value.y, value.z);
	}

	void vc::rendering::ShaderProgram::loadVector2f(uint32_t uniformLocation, glm::vec2 value) {
		glUniform2f(uniformLocation, value.x, value.y);
	}

	void vc::rendering::ShaderProgram::loadBool(uint32_t uniformLocation, bool value) {
		glUniform1i(uniformLocation, (value ? 1 : 0));
	}

	void vc::rendering::ShaderProgram::loadMatrix4f(uint32_t uniformLocation, glm::mat4x4 value) {
		float* matrix = glm::value_ptr(value);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrix);
	}

	uint32_t vc::rendering::ShaderProgram::loadShader(uint32_t type, const char* shaderCode) {
	    // send shader to GPU
	    GLuint shader = glCreateShader(type);
	    glShaderSource(shader, 1, &shaderCode, 0);

	    // compile shader
	    glCompileShader(shader);
	    GLint compiled = 0;
	    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	    if(compiled == GL_FALSE) {
	    	GLint maxLength = 0;
	    	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	    	std::vector<GLchar> infoLog(maxLength);
	    	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			std::cerr << "Couldn't compile shader!" << std::endl;
			std::cerr << &infoLog[0];
	    	glDeleteShader(shader);

			exit(-1);
	    }

	    return shader;
	}

	void vc::rendering::ShaderProgram::startProgram() {
		glUseProgram(programId);
	}


	void vc::rendering::ShaderProgram::stopProgram() {
		glUseProgram(0);
	}


}
