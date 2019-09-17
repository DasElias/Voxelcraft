#include "Cancellable.h"

namespace vc {
	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	void Cancellable::cancel() {
		this->field_isCancelled = true;
	}

	bool Cancellable::isCancelled() const {
		return field_isCancelled;
	}
}
