#pragma once

#include <vector>
#include <cstdint>

namespace vc {
	class InstancedRenderingVbo {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::uint32_t const targetVao;
			std::uint32_t vboId;
			int maxVboAttributeIndex = 0;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			InstancedRenderingVbo(std::uint32_t targetVao, int amountOfFloats);
			InstancedRenderingVbo(const InstancedRenderingVbo&) = delete;
			~InstancedRenderingVbo();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void addInstancedAttribute(std::int32_t dataSize, std::int32_t instancedDataLength, std::int32_t offset);
			void updateVbo(std::vector<float>& data);


	};

}
