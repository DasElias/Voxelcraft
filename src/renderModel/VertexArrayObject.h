#pragma once

#include <cstdint>

namespace vc {
	class VertexArrayObject {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			VertexArrayObject() = default;

		public:
			virtual ~VertexArrayObject() = default;
			VertexArrayObject(const VertexArrayObject&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual std::uint32_t getVertexArrayObjectId() =0;

			virtual std::size_t getIndiciesCount() =0;
	};
}
