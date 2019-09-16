#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <functional>


namespace vc {
	namespace model {
		typedef std::function<std::size_t(const glm::vec3&)> 					vec3f_hashType;
		typedef std::function<bool(const glm::vec3&, const glm::vec3&)> vec3f_equalsType;
		typedef std::function<std::size_t(const glm::ivec3&)> 					vec3i_hashType;
		typedef std::function<bool(const glm::ivec3&, const glm::ivec3&)> vec3i_equalsType;

		typedef std::function<std::size_t(const glm::vec2&)> 					vec2f_hashType;
		typedef std::function<bool(const glm::vec2&, const glm::vec2&)> vec2f_equalsType;
		typedef std::function<std::size_t(const glm::ivec2&)> 					vec2i_hashType;
		typedef std::function<bool(const glm::ivec2&, const glm::ivec2&)> vec2i_equalsType;
		typedef std::function<bool(const glm::ivec2&, const glm::ivec2&)> vec2i_compareType;

		//Vector3f
		const vec3f_hashType VECTOR3F_HASH = [](const glm::vec3& n) {
			std::hash<float> hf;
			int const prime = 31;
			std::size_t result = 1;
			result = prime * result + hf(n.x);
			result = prime * result + hf(n.y);
			result = prime * result + hf(n.z);
			return result;
		};
		const vec3f_equalsType VECTOR3F_EQUAL = [](const glm::vec3& l, const glm::vec3& r){
			return l.x == r.x && l.y == r.y && l.z == r.z;
		};



		//Vector3i
		const vec3i_hashType VECTOR3I_HASH = [](const glm::ivec3& n) {
			std::hash<int> hf;
			int const prime = 31;
			std::size_t result = 1;
			result = prime * result + hf(n.x);
			result = prime * result + hf(n.y);
			result = prime * result + hf(n.z);
			return result;
		};
		const vec3i_equalsType VECTOR3I_EQUAL = [](const glm::ivec3& l, const glm::ivec3& r){
			return l.x == r.x && l.y == r.y && l.z == r.z;
		};



		//Vector2f
		const vec2f_hashType VECTOR2F_HASH = [](const glm::vec2& n) {
			std::hash<float> hf;
			int const prime = 31;
			std::size_t result = 1;
			result = prime * result + hf(n.x);
			result = prime * result + hf(n.y);
			return result;
		};
		const vec2f_equalsType VECTOR2F_EQUAL = [](const glm::vec2& l, const glm::vec2& r){
			return l.x == r.x && l.y == r.y;
		};



		//Vector2i
		const vec2i_hashType VECTOR2I_HASH = [](const glm::ivec2& n) {
			std::hash<int> hf;
			int const prime = 31;
			std::size_t result = 1;
			result = prime * result + hf(n.x);
			result = prime * result + hf(n.y);
			return result;
		};
		const vec2i_equalsType VECTOR2I_EQUAL = [](const glm::ivec2& l, const glm::ivec2& r){
			return l.x == r.x && l.y == r.y;
		};
		const vec2i_compareType VECTOR2I_COMPARE = [](const glm::ivec2& l, const glm::ivec2& r) {
			return VECTOR2I_HASH(l) < VECTOR2I_HASH(r);
		};
	}
}
