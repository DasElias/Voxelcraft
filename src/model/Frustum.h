#pragma once

#include <array>
#include "ProjectionMatrix.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace vc {
	namespace model {
		class Player;
		class Plane;

		class Frustum {
			// ----------------------------------------------------------------------
			// -----------------------------CHILD-CLASS------------------------------
			// ----------------------------------------------------------------------
			private:
				class Plane {
					// ----------------------------------------------------------------------
					// --------------------------------FIELDS--------------------------------
					// ----------------------------------------------------------------------
					private:
						float a, b, c, d;

						float planeLength;

					// ----------------------------------------------------------------------
					// -----------------------------CONSTRUCTORS-----------------------------
					// ----------------------------------------------------------------------
					public:
						Plane(float a = 0, float b = 0, float c = 0, float d = 0);
						Plane(glm::vec4 point1, glm::vec4 point2, glm::vec4 point3);
						Plane(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);

					// ----------------------------------------------------------------------
					// -------------------------------METHODS--------------------------------
					// ----------------------------------------------------------------------
					private:
						void updatePlaneLength();

					public:
						Plane& set(float a, float b, float c, float d);
						Plane& set(glm::vec4 point1, glm::vec4 point2, glm::vec4 point3);
						Plane& set(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
						inline float getA() const { return a; }
						void setA(float value);
						inline float getB() const { return b; }
						void setB(float value);
						inline float getC() const { return c; }
						void setC(float value);
						inline float getD() const { return d; }
						void setD(float value);

						float computeDistanceToPoint(const glm::vec3& point) const;
						float computeAbsDistanceToPoint(const glm::vec3& point) const;

						glm::vec3 getNormal() const;


				};
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			public:
				static float const NEAR_PLANE_DISTANCE;
				static float const FAR_PLANE_DISTANCE;
				static float const FAR_PLANE_DISTANCE_CULLING;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				float fov;
				Player& player;
				ProjectionMatrix projectionMatrix;
				ProjectionMatrix cullingProjectionMatrix;
				std::array<glm::vec4, 8> frustumEdges = {{
							//near plane
							glm::vec4(-1, -1, -1, -1), glm::vec4(1, -1, -1, 1), glm::vec4(1, 1, -1, 1), glm::vec4(-1, 1, -1, 1),

							//far plane
							glm::vec4(-1, -1, 1, 1), glm::vec4(1, -1, 1, 1), glm::vec4(1, 1, 1, 1), glm::vec4(-1, 1, 1, 1)
						}};
				std::array<Plane, 6> frustumPlanes;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				Frustum(Player& player);


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				bool isPointInFrustum_impl(const glm::vec4& a4f, const glm::vec4& b4f, const glm::vec4& c4f, const glm::vec3& point) const;

			public:
				void update();

				float getFov() const;
				void setFov(float fov);
				glm::mat4x4 getProjectionMatrix() const;
				bool isInFrustum_point(glm::vec3 const point) const;
				bool isInFrustum_aabb(float minX, float minY, float minZ, float width, float height, float depth);

		};
	}
}
