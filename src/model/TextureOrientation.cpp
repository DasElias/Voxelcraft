#include "TextureOrientation.h"

#include <utility>
#include <stdexcept>


#define BM_TOP 0x7
#define BM_FRONT 0x38
#define BM_BOTTOM 0x1C0
#define BM_LEFT 0x7000
#define BM_RIGHT 0x38000
#define BM_BACK 0xE00

namespace vc {
	namespace model {
		/**
		 * We use a unsigned 32 bit number to representate the combined values of all texture orientation. Every face
		 * needs 3 bit in this number, since there are only 6 possible values and 3 bits can 2^3 = 8 values. 
		 *
		 * To access the particular texture orientations, we use bit manipulation. 
		 * BM_TOP: 0x7
		 * BM_FRONT: 0x38
		 * BM_BOTTOM: 0x1C0
		 * BM_BACK: 0xE00
		 * BM_LEFT: 0x7000
		 * BM_RIGHT: 0x38000
		 */

		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		std::map<std::uint32_t, TextureOrientation> TextureOrientation::ALL_VALUES;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		TextureOrientation::TextureOrientation(std::uint8_t top, std::uint8_t front, std::uint8_t bottom, std::uint8_t back, std::uint8_t left, std::uint8_t right) :
				top(top),
				front(front),
				bottom(bottom),
				back(back),
				left(left),
				right(right) {

			/**
			 * Only the first three bits must be used, since there are only six possible values for the texture orientation per face,
			 * therefore we don't need the others and can use it to store other values. 
			 */
			uint8_t const bitmask = 0xF8;
			if(top & bitmask || front & bitmask || bottom & bitmask || back & bitmask || left & bitmask || right & bitmask) {
				throw std::logic_error("The int8_t arguments for TextureOrientation mustn't use the last 5 bits!");
			}
		}

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		const TextureOrientation& TextureOrientation::getStoredTextureOrientation(std::uint32_t value) {
			if(ALL_VALUES.find(value) == ALL_VALUES.end()) {
				std::uint8_t top = value & BM_TOP;
				std::uint8_t front = value & BM_FRONT;
				std::uint8_t bottom = value & BM_BOTTOM;
				std::uint8_t back = value & BM_BACK;
				std::uint8_t left = value & BM_LEFT;
				std::uint8_t right = value & BM_RIGHT;


				// key does not exist yet
				ALL_VALUES.insert({value, TextureOrientation(top, front, bottom, back, left, right)});
			}

			return ALL_VALUES.at(value);
		}

		const TextureOrientation& TextureOrientation::getTextureOrientation(Face top, Face front, Face bottom, Face back, Face left, Face right) {
			std::uint32_t hash = generateHash(top, front, bottom, back, left, right);
			if(ALL_VALUES.find(hash) == ALL_VALUES.end()) {
				// key does not exist yet
				ALL_VALUES.insert({hash, TextureOrientation(top, front, bottom, back, left, right)});
			}

			return ALL_VALUES.at(hash);
		}

		std::uint32_t TextureOrientation::generateHash(std::uint8_t top, std::uint8_t front, std::uint8_t bottom, std::uint8_t back, std::uint8_t left, std::uint8_t right) {
			std::uint32_t toReturn = 0;

			toReturn |= (top << 0 * 3);
			toReturn |= (front << 1 * 3);
			toReturn |= (bottom << 2 * 3);
			toReturn |= (back << 3 * 3);
			toReturn |= (left << 4 * 3);
			toReturn |= (right << 5 * 3);

			return toReturn;
		}


		std::uint8_t TextureOrientation::getTop() const {
			return top;
		}

		std::uint8_t TextureOrientation::getFront() const {
			return front;
		}
		
		std::uint8_t TextureOrientation::getBottom() const {
			return bottom;
		}

		std::uint8_t TextureOrientation::getBack() const {
			return back;
		}

		std::uint8_t TextureOrientation::getLeft() const {
			return left;
		}

		std::uint8_t TextureOrientation::getRight() const {
			return right;
		}

		std::uint32_t TextureOrientation::getValue() const {
			return generateHash(top, front, bottom, back, left, right);
		}

		
	}
}
