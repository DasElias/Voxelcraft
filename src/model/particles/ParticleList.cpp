#include "ParticleList.h"

#include <Eigen/Core>
#include <algorithm>
#include <iterator>


namespace vc::model {

	void ParticleList::update(float delta, glm::vec3 playerPosition) {
		auto mapIterator = particles.cbegin();

		while(mapIterator != particles.cend()) {
			auto listIterator = mapIterator->second.begin();

			// rmeove dead particles and particles which are too far away from the player
			while(listIterator != mapIterator->second.end()) {
				bool stillAlive = listIterator->update(delta, playerPosition);

				if(! stillAlive || listIterator->getDistanceFromCamera() >= MAX_DISTANCE) {
					listIterator = mapIterator->second.erase(listIterator);
				} else {
					++listIterator;
				}
			}

			if(mapIterator->second.empty()) {
				particles.erase(mapIterator);
			} else {
				std::sort(mapIterator->second.begin(), mapIterator->second.end(), [](const Particle& a, const Particle& b) -> bool {
					// this lambda returns true if the first argument should before the second one
					// we want to sort the list in descending order
					// => return true if a > b

					return a.getDistanceFromCamera() > b.getDistanceFromCamera();
				});

				++mapIterator;
			}
		}
	}

	void ParticleList::add(Particle& p) {
		if(particles.find(p.getTexture()) == particles.end()) {
			// key doesn't exist in map yet

			std::vector<Particle> v;
			v.push_back(p);
			particles[p.getTexture()] = v;
		} else {
			particles.at(p.getTexture()).push_back(p);
		}
	}

	const ParticleMap& ParticleList::get() const {
		return particles;
	}

}
