#pragma once
#include <cstdint>

#include "../model/utils/Cleanable.h"

namespace vc {
	class Texture : public Cleanable{
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static float const ANISOTROPIC_FILTERING_AMOUNT;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Texture() = default;
			virtual ~Texture() = default;
		
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual uint32_t getTexId() const =0;
	};
}
