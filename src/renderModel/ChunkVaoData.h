#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace vc {
	namespace renderingModel {

		#pragma pack(push, 1)
		struct ChunkVaoData {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			public:
				GLfloat coordX;
				GLfloat coordY;
				GLfloat coordZ;
				GLushort texId_facing;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ChunkVaoData(float coordX = 0, float coordY = 0, float coordZ = 0, short texId_facing = 0);
		};
		#pragma pack(pop)
	}
}