#pragma once
#include <cstdint>
#include <map>
#include <unordered_map>
#include <memory>

#include "Face.h"
namespace vc {
	namespace model {
		class TextureOrientation {

			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			public:
				static std::map<std::uint32_t, TextureOrientation> ALL_VALUES;

			// ----------------------------------------------------------------------
			// ---------------------------STATIC-METHODS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static std::uint32_t generateHash(std::uint8_t top, std::uint8_t front, std::uint8_t bottom, std::uint8_t back, std::uint8_t left, std::uint8_t right);
			public:
				static const TextureOrientation& getStoredTextureOrientation(std::uint32_t value);

				static const TextureOrientation& getTextureOrientation(Face top, Face front, Face bottom, Face back, Face left, Face right);

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				std::uint8_t top, front, bottom, back, left, right;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			private:
				TextureOrientation(std::uint8_t top, std::uint8_t front, std::uint8_t bottom, std::uint8_t back, std::uint8_t left, std::uint8_t right);


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:	
				std::uint32_t getValue() const;

				std::uint8_t getTop() const;
				std::uint8_t getFront() const;
				std::uint8_t getBottom() const;
				std::uint8_t getBack() const;
				std::uint8_t getLeft() const;
				std::uint8_t getRight() const;


		};
	}
}
