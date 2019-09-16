#pragma once

#include "ParticleSystem.h"

namespace vc {
	namespace model {
		class ParticleManager {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			private:
				ParticleManager();

			public:
				ParticleManager(const ParticleManager&) = delete;

			// ----------------------------------------------------------------------
			// ---------------------------STATIC-METHODS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ParticleManager& getInstance() {
					static ParticleManager instance;

					return instance;
				}

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void add(ParticleSystem& particleSystem);
				void remove(ParticleSystem& particleSystem);
				void update(float delta);
		};

	}
}
