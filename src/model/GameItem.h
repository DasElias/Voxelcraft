#pragma once

#include <string>
#include <map>
#include <memory>

namespace vc {
	class GameItem {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::map<int, std::shared_ptr<GameItem>> ALL_ITEM_TYPES;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			virtual ~GameItem() = default;

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			static void addGameItem(const std::shared_ptr<GameItem>& itemType);

		public:
			static const std::shared_ptr<GameItem>& getGameItemById(int id);
			static const std::map<int, std::shared_ptr<GameItem>>& getAll();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual uint32_t getId() const =0;
			virtual bool canPlayerUse() const =0;
			virtual bool isBlock() const =0;
			virtual std::string getName() const =0;
			virtual int getMaxStackSize() const =0;
	};
}
