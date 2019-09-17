#include "MousePicker.h"
#include "Player.h"
#include "input/IOHandler.h"

#include <glm/glm.hpp>

using namespace egui;
using namespace glm;

namespace vc {
	MousePicker::MousePicker(Player& player) :
			player(player) {
	}

	void MousePicker::update() {
		//coords of the click on the screen
		float mouseX = getDisplayHandler().getWidth() / 2.0f;
		float mouseY = getDisplayHandler().getHeight() / 2.0f;

		// normalized device coords (in the range from -1 to 1)
		float ndc_x = (2.0f * mouseX) / getDisplayHandler().getWidth() - 1;
		float ndc_y = (2.0f * mouseY) / getDisplayHandler().getHeight() - 1;

		// vector should point into the screen
		vec4 clipCoords(ndc_x, ndc_y, -1, 1);

		// inverted projection matrix
		mat4x4 invertedProjectionMatrix = inverse(player.getFrustum().getProjectionMatrix());
		vec4 eyeCoords = invertedProjectionMatrix * clipCoords;
		eyeCoords.z = -1;
		eyeCoords.a = 0;

		// inverted view matrix
		mat4x4 invertedViewMatrix = inverse(player.computeViewMatrix());
		vec4 worldCoords = invertedViewMatrix * eyeCoords;

		ray.x = worldCoords.x;
		ray.y = worldCoords.y;
		ray.z = worldCoords.z;
		ray = normalize(ray);

		rayStart.x = player.getPosition().x;
		rayStart.y = player.getPosition().y + 1.5f;
		rayStart.z = player.getPosition().z;

	}

	glm::vec3 MousePicker::getRay() {
		return ray;
	}

	glm::vec3 MousePicker::getRayStart() {
		return rayStart;
	}

}
