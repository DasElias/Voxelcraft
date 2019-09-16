#include "../utils/Cleanable.h"

#include <stdexcept>
namespace vc {
	void Cleanable::cleanUp() {
		if(isAlreadyCleaned) throw std::runtime_error("Object has already been cleaned up!");
	}

}

