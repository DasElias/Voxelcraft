#include "KeyAssignmentsImpl.h"
#include "input/InputHandler.h"
#include <sstream>

#include "../model/utils/FileUtils.h"

namespace vc {
	KeyAssignmentsImpl::KeyAssignmentsImpl() {

		SI_Error err = properties.LoadFile(getFile().c_str());
		if(err < 0) throw std::runtime_error("Couldn't load key assignments file.");

		bool save = false;

		save |= setProperty("MOVE_FORWARD", KEY_W, true);
		save |= setProperty("MOVE_LEFT", KEY_A, true);
		save |= setProperty("MOVE_RIGHT", KEY_D, true);
		save |= setProperty("MOVE_BACKWARD", KEY_S, true);

		save |= setProperty("JUMP", KEY_SPACE, true);
		save |= setProperty("BREAK_BLOCK", MOUSE_BUTTON_2, true);
		save |= setProperty("PLACE_BLOCK", MOUSE_BUTTON_1, true);

		if(save) {
			properties.SaveFile(getFile().c_str());
		}
	}

	std::string KeyAssignmentsImpl::getFile() {
		std::string file = getApplicationFolder().append("\\config.ini");
		if(! doesExist(file)) createFile(file);

		return file;
	}

	void KeyAssignmentsImpl::save() {
		properties.SaveFile(getFile().c_str());
	}

	int KeyAssignmentsImpl::getProperty(std::string key) {
		long fromProperties = properties.GetLongValue("voxelcraft", key.c_str(), -1);

		if(fromProperties == -1) {
			throw std::invalid_argument("the key doesn't exist");
		} else return int(fromProperties);
	}

	bool KeyAssignmentsImpl::setProperty(std::string key, int value, bool preventReplace = false) {
		bool shouldInsert = true;
		if(preventReplace && properties.GetLongValue("voxelcraft", key.c_str(), -1) != -1) {
			shouldInsert = false;
		}

		if(shouldInsert) return properties.SetLongValue("voxelcraft", key.c_str(), value) == SI_INSERTED;
		else return false;
	}
}
