
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Frustum.h"

#include "Player.h"

#include "utils/VectorUtils.h"


using namespace glm;

namespace vc::model {
	const float Frustum::NEAR_PLANE_DISTANCE = 0.1f;
	const float Frustum::FAR_PLANE_DISTANCE = 600;
	const float Frustum::FAR_PLANE_DISTANCE_CULLING = 300;

	Frustum::Plane::Plane(float a, float b, float c, float d) {
		set(a, b, c, d);
	}

	Frustum::Plane::Plane(vec4 point1, vec4 point2, vec4 point3) {
		set(point1, point2, point3);
	}

	Frustum::Plane::Plane(vec3 point1, vec3 point2, vec3 point3) {
		set(point1, point2, point3);
	}

	Frustum::Plane& Frustum::Plane::set(float a, float b, float c, float d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;

		updatePlaneLength();

		return *this;
	}

	Frustum::Plane& Frustum::Plane::set(vec4 point1, vec4 point2, vec4 point3) {
		vec3 a(point1.x, point1.y, point1.z);
		vec3 b(point2.x, point2.y, point2.z);
		vec3 c(point3.x, point3.y, point3.z);

		set(a, b, c);

		return *this;
	}

	Frustum::Plane& Frustum::Plane::set(const vec3 point1, const vec3 point2, const vec3 point3) {
		vec3 normal = glm::cross(point2 - point1, point3 - point1);

		this->a = normal.x;
		this->b = normal.y;
		this->c = normal.z;
		this->d = - glm::dot(normal, point1);

		updatePlaneLength();

		return *this;
	}

	void Frustum::Plane::updatePlaneLength() {
		//todo must be multiplied with d?
		this->planeLength = sqrtf(a * a + b * b + c * c);
	}


	void Frustum::Plane::setA(float value) {
		this->a = value;
		updatePlaneLength();
	}

	void Frustum::Plane::setB(float value) {
		this->b = value;
		updatePlaneLength();
	}

	void Frustum::Plane::setC(float value) {
		this->c = value;
		updatePlaneLength();
	}

	void Frustum::Plane::setD(float value) {
		this->d = value;
		updatePlaneLength();
	}

	float Frustum::Plane::computeDistanceToPoint(const vec3& point) const {
		float numerator = a * point.x + b * point.y + c * point.z + d;
		return numerator / planeLength;
	}

	float Frustum::Plane::computeAbsDistanceToPoint(const vec3& point) const {
		return abs(computeDistanceToPoint(point));
	}

	vec3 Frustum::Plane::getNormal() const {
		return vec3(a, b, c);
	}

	Frustum::Frustum(Player& player) :
			fov(95),
			player(player),
			projectionMatrix(fov, NEAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE),
			cullingProjectionMatrix(fov, NEAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE_CULLING) {
		update();
	}

	void Frustum::update() {
		projectionMatrix.update(fov);
		cullingProjectionMatrix.update(fov);

		// reset the position of the frustum edges
		set(frustumEdges[0], -1, -1, -1, 1);
		set(frustumEdges[1], 1, -1, -1, 1);
		set(frustumEdges[2], 1, 1, -1, 1);
		set(frustumEdges[3], -1, 1, -1, 1);

		set(frustumEdges[4], -1, -1, 1, 1);
		set(frustumEdges[5], 1, -1, 1, 1);
		set(frustumEdges[6], 1, 1, 1, 1);
		set(frustumEdges[7], -1, 1, 1, 1);

		// compute inverse ProjectionView-Matrix
		glm::mat4x4 inv = glm::inverse(cullingProjectionMatrix.getProjectionMatrix() * player.computeViewMatrix());

		for(unsigned int counter = 0; counter < frustumEdges.size(); counter++) {
			frustumEdges[counter] = inv * frustumEdges[counter];

			frustumEdges[counter].x /= frustumEdges[counter].a;
			frustumEdges[counter].y /= frustumEdges[counter].a;
			frustumEdges[counter].z /= frustumEdges[counter].a;
			frustumEdges[counter].a = 1;
		}

		frustumPlanes[0].set(frustumEdges[0], frustumEdges[1], frustumEdges[2]);	//front
		frustumPlanes[1].set(frustumEdges[4], frustumEdges[6], frustumEdges[5]);	//back
		frustumPlanes[2].set(frustumEdges[0], frustumEdges[3], frustumEdges[4]);	//left
		frustumPlanes[3].set(frustumEdges[1], frustumEdges[5], frustumEdges[2]);
		frustumPlanes[4].set(frustumEdges[2], frustumEdges[6], frustumEdges[3]);
		frustumPlanes[5].set(frustumEdges[0], frustumEdges[4], frustumEdges[1]);
	}

	bool Frustum::isPointInFrustum_impl(const vec4& a4f, const vec4& b4f, const vec4& c4f, const vec3& point) const {
		vec3 a(a4f.x, a4f.y, a4f.z);
		vec3 b(b4f.x, b4f.y, b4f.z);
		vec3 c(c4f.x, c4f.y, c4f.z);

		vec3 normal = glm::cross(b - a, c - a);
		return !(glm::dot(point - a, normal) < 0);
	}

	bool Frustum::isInFrustum_point(const vec3 point) const {
		return isPointInFrustum_impl(frustumEdges[0], frustumEdges[2], frustumEdges[1], point) &&	//front
				isPointInFrustum_impl(frustumEdges[4], frustumEdges[5], frustumEdges[6], point) &&	//back
				isPointInFrustum_impl(frustumEdges[0], frustumEdges[4], frustumEdges[3], point) &&	//left
				isPointInFrustum_impl(frustumEdges[1], frustumEdges[2], frustumEdges[5], point) &&	//right
				isPointInFrustum_impl(frustumEdges[2], frustumEdges[3], frustumEdges[6], point) &&	//top
				isPointInFrustum_impl(frustumEdges[0], frustumEdges[1], frustumEdges[4], point);	//bottom
	}

	bool Frustum::isInFrustum_aabb(float minX, float minY, float minZ, float width, float height, float depth) {
		for(auto& p : frustumPlanes) {
			vec3 planeNormal = p.getNormal();
			float vMinX, vMinY, vMinZ;

			//x
			if(p.getA() > 0) {
				vMinX = minX;
			} else {
				vMinX = minX + width;
			}

			//y
			if(p.getB() > 0) {
				vMinY = minY;
			} else {
				vMinY = minY + height;
			}

			//z
			if(p.getC() > 0) {
				vMinZ = minZ;
			} else {
				vMinZ = minZ + depth;
			}

			if(vMinX * planeNormal.x + vMinY * planeNormal.y + vMinZ * planeNormal.z + p.getD() > 0) return false;
		}

		return true;
	}

	float Frustum::getFov() const {
		return fov;
	}

	void Frustum::setFov(float fov) {
		this->fov = fov;
		update();
	}

	mat4x4 Frustum::getProjectionMatrix() const {
		return projectionMatrix.getProjectionMatrix();
	}
}
